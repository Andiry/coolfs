/*
 * BRIEF DESCRIPTION
 *
 * Garbage collection methods
 *
 * Copyright 2015-2016 Regents of the University of California,
 * UCSD Non-Volatile Systems Lab, Andiry Xu <jix024@cs.ucsd.edu>
 * Copyright 2012-2013 Intel Corporation
 * Copyright 2009-2011 Marco Stornelli <marco.stornelli@gmail.com>
 * Copyright 2003 Sony Corporation
 * Copyright 2003 Matsushita Electric Industrial Co., Ltd.
 * 2003-2004 (c) MontaVista Software, Inc. , Steve Longerbeam
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include "nova.h"
#include "inode.h"

static bool curr_log_entry_invalid(struct super_block *sb,
	struct nova_inode *pi, struct nova_inode_info_header *sih,
	u64 curr_p, size_t *length)
{
	struct nova_file_write_entry *entry;
	struct nova_dentry *dentry;
	struct nova_setattr_logentry *setattr_entry;
	struct nova_link_change_entry *linkc_entry;
	void *entryc;
	u8 type;
	bool ret = true;

	entryc = (void *)nova_get_block(sb, curr_p);
	type = nova_get_entry_type(entryc);

	switch (type) {
	case SET_ATTR:
		setattr_entry = (struct nova_setattr_logentry *) entryc;
		if (setattr_entry->invalid == 0)
			ret = false;
		*length = sizeof(struct nova_setattr_logentry);
		break;
	case LINK_CHANGE:
		linkc_entry = (struct nova_link_change_entry *) entryc;
		if (linkc_entry->invalid == 0)
			ret = false;
		*length = sizeof(struct nova_link_change_entry);
		break;
	case FILE_WRITE:
		entry = (struct nova_file_write_entry *) entryc;
		if (entry->num_pages != entry->invalid_pages)
			ret = false;
		*length = sizeof(struct nova_file_write_entry);
		break;
	case DIR_LOG:
		dentry = (struct nova_dentry *) entryc;
		if (dentry->invalid == 0)
			ret = false;
		if (sih->last_dentry == curr_p)
			ret = false;
		*length = le16_to_cpu(dentry->de_len);
		break;
	case NEXT_PAGE:
		/* No more entries in this page */
		*length = PAGE_SIZE - ENTRY_LOC(curr_p);
		break;
	default:
		nova_dbg("%s: unknown type %d, 0x%llx\n",
					__func__, type, curr_p);
		NOVA_ASSERT(0);
		*length = PAGE_SIZE - ENTRY_LOC(curr_p);
		break;
	}

	return ret;
}

static bool curr_page_invalid(struct super_block *sb,
	struct nova_inode *pi, struct nova_inode_info_header *sih,
	u64 page_head)
{
	struct nova_inode_log_page *curr_page;
	struct nova_inode_page_tail page_tail;
	unsigned int num_entries;
	unsigned int invalid_entries;
	bool ret;
	timing_t check_time;
	int rc;

	NOVA_START_TIMING(check_invalid_t, check_time);

	curr_page = (struct nova_inode_log_page *)
					nova_get_block(sb, page_head);
	rc = memcpy_mcsafe(&page_tail, &curr_page->page_tail,
					sizeof(struct nova_inode_page_tail));
	if (rc) {
		nova_err(sb, "check page failed\n");
		return false;
	}

	num_entries = le32_to_cpu(page_tail.num_entries);
	invalid_entries = le32_to_cpu(page_tail.invalid_entries);

	ret = (invalid_entries == num_entries);
	if (!ret) {
		sih->num_entries += num_entries;
		sih->valid_entries += num_entries - invalid_entries;
	}

	NOVA_END_TIMING(check_invalid_t, check_time);
	return ret;
}

static void free_curr_page(struct super_block *sb,
	struct nova_inode_info_header *sih,
	struct nova_inode_log_page *curr_page,
	struct nova_inode_log_page *last_page, u64 curr_head)
{
	u8 btype = sih->i_blk_type;

	nova_set_next_page_address(sb, last_page,
			curr_page->page_tail.next_page, 1);
	nova_free_log_blocks(sb, sih,
			nova_get_blocknr(sb, curr_head, btype), 1);
}

static int nova_gc_assign_file_entry(struct super_block *sb,
	struct nova_inode_info_header *sih,
	struct nova_file_write_entry *old_entry,
	struct nova_file_write_entry *new_entry)
{
	struct nova_file_write_entry *temp;
	void **pentry;
	unsigned long start_pgoff = old_entry->pgoff;
	unsigned int num = old_entry->num_pages;
	unsigned long curr_pgoff;
	int i;
	int ret = 0;

	for (i = 0; i < num; i++) {
		curr_pgoff = start_pgoff + i;
repeat:
		pentry = radix_tree_lookup_slot(&sih->tree, curr_pgoff);
		if (pentry) {
			temp = radix_tree_deref_slot(pentry);

			if (radix_tree_exception(temp)) {
				if (radix_tree_deref_retry(temp))
					goto repeat;
				continue;
			}

			if (temp == old_entry) {
				lock_write_entry(temp);
				radix_tree_replace_slot(&sih->tree, pentry,
							new_entry);
				unlock_write_entry(temp);
			}
		}
	}

	return ret;
}

static int nova_gc_assign_dentry(struct super_block *sb,
	struct nova_inode_info_header *sih, struct nova_dentry *old_dentry,
	struct nova_dentry *new_dentry)
{
	struct nova_range_node *ret_node = NULL;
	unsigned long hash;
	int found = 0;
	int ret = 0;

	hash = BKDRHash(old_dentry->name, old_dentry->name_len);
	nova_dbgv("%s: assign %s hash %lu\n", __func__,
			old_dentry->name, hash);

	/* FIXME: hash collision ignored here */
	found = nova_find_range_node(&sih->rb_tree, hash,
				NODE_DIR, &ret_node);
	if (found == 1 && hash == ret_node->hash) {
		if (ret_node->direntry == old_dentry)
			ret_node->direntry = new_dentry;
	}

	return ret;
}

static int nova_gc_assign_new_entry(struct super_block *sb,
	struct nova_inode *pi, struct nova_inode_info_header *sih,
	u64 curr_p, u64 new_curr)
{
	struct nova_file_write_entry *old_entry, *new_entry;
	struct nova_dentry *old_dentry, *new_dentry;
	void *addr, *new_addr;
	u8 type;
	int ret = 0;

	addr = (void *)nova_get_block(sb, curr_p);
	type = nova_get_entry_type(addr);
	switch (type) {
	case SET_ATTR:
		sih->last_setattr = new_curr;
		break;
	case LINK_CHANGE:
		sih->last_link_change = new_curr;
		break;
	case FILE_WRITE:
		new_addr = (void *)nova_get_block(sb, new_curr);
		old_entry = (struct nova_file_write_entry *)addr;
		new_entry = (struct nova_file_write_entry *)new_addr;
		ret = nova_gc_assign_file_entry(sb, sih, old_entry, new_entry);
		break;
	case DIR_LOG:
		new_addr = (void *)nova_get_block(sb, new_curr);
		old_dentry = (struct nova_dentry *)addr;
		new_dentry = (struct nova_dentry *)new_addr;
		if (sih->last_dentry == curr_p)
			sih->last_dentry = new_curr;
		ret = nova_gc_assign_dentry(sb, sih, old_dentry, new_dentry);
		break;
	default:
		nova_dbg("%s: unknown type %d, 0x%llx\n",
					__func__, type, curr_p);
		NOVA_ASSERT(0);
		break;
	}

	return ret;
}

/* Copy live log entries to the new log and atomically replace the old log */
static unsigned long nova_inode_log_thorough_gc(struct super_block *sb,
	struct nova_inode *pi, struct nova_inode_info_header *sih,
	unsigned long blocks, unsigned long checked_pages)
{
	struct nova_inode_log_page *curr_page = NULL;
	size_t length;
	u64 curr_p, new_curr;
	u64 old_curr_p;
	u64 tail_block;
	u64 old_head;
	u64 new_head = 0;
	u64 next;
	int allocated;
	int extended = 0;
	int ret;
	timing_t gc_time;

	NOVA_START_TIMING(thorough_gc_t, gc_time);

	curr_p = sih->log_head;
	old_curr_p = curr_p;
	old_head = sih->log_head;
	nova_dbg_verbose("Log head 0x%llx, tail 0x%llx\n",
				curr_p, sih->log_tail);
	if (curr_p == 0 && sih->log_tail == 0)
		goto out;

	if (curr_p >> PAGE_SHIFT == sih->log_tail >> PAGE_SHIFT)
		goto out;

	allocated = nova_allocate_inode_log_pages(sb, sih, blocks,
					&new_head, ANY_CPU, 0);
	if (allocated != blocks) {
		nova_err(sb, "%s: ERROR: no inode log page available\n",
					__func__);
		goto out;
	}

	new_curr = new_head;
	while (curr_p != sih->log_tail) {
		old_curr_p = curr_p;
		if (goto_next_page(sb, curr_p))
			curr_p = next_log_page(sb, curr_p);

		if (curr_p >> PAGE_SHIFT == sih->log_tail >> PAGE_SHIFT) {
			/* Don't recycle tail page */
			break;
		}

		length = 0;
		ret = curr_log_entry_invalid(sb, pi, sih, curr_p, &length);
		if (!ret) {
			extended = 0;
			new_curr = nova_get_append_head(sb, pi, sih,
						new_curr, length, MAIN_LOG,
						1, &extended);
			if (extended)
				blocks++;
			/* Copy entry to the new log */
			memcpy_to_pmem_nocache(nova_get_block(sb, new_curr),
				nova_get_block(sb, curr_p), length);
			nova_inc_page_num_entries(sb, new_curr);
			nova_gc_assign_new_entry(sb, pi, sih, curr_p, new_curr);
			new_curr += length;
		}

		curr_p += length;
	}

	/* Step 1: Link new log to the tail block */
	tail_block = BLOCK_OFF(sih->log_tail);
	curr_page = (struct nova_inode_log_page *)nova_get_block(sb,
							BLOCK_OFF(new_curr));
	next = next_log_page(sb, new_curr);
	if (next > 0)
		nova_free_contiguous_log_blocks(sb, sih, next);

	nova_set_next_page_flag(sb, new_curr);
	nova_set_next_page_address(sb, curr_page, tail_block, 0);

	/* Step 2: Atomically switch to the new log */
	pi->log_head = new_head;
	nova_persist_inode(pi);
	nova_flush_buffer(pi, sizeof(struct nova_inode), 1);
	sih->log_head = new_head;

	/* Step 3: Unlink the old log */
	curr_page = (struct nova_inode_log_page *)nova_get_block(sb,
							BLOCK_OFF(old_curr_p));
	next = next_log_page(sb, old_curr_p);
	if (next != tail_block)
		nova_err(sb, "Old log error: old curr_p 0x%lx, next 0x%lx ",
			"curr_p 0x%lx, tail block 0x%lx\n", old_curr_p,
			next, curr_p, tail_block);

	nova_set_next_page_address(sb, curr_page, 0, 1);

	/* Step 4: Free the old log */
	nova_free_contiguous_log_blocks(sb, sih, old_head);

	sih->log_pages = sih->log_pages + blocks - checked_pages;
	NOVA_STATS_ADD(thorough_gc_pages, checked_pages - blocks);
	NOVA_STATS_ADD(thorough_checked_pages, checked_pages);
out:
	NOVA_END_TIMING(thorough_gc_t, gc_time);
	return blocks;
}


/*
 * Scan pages in the log and remove those with no valid log entries.
 */
int nova_inode_log_fast_gc(struct super_block *sb,
	struct nova_inode *pi, struct nova_inode_info_header *sih,
	u64 curr_tail, u64 new_block,
	int num_pages, int force_thorough)
{
	u64 curr, next, possible_head = 0;
	int found_head = 0;
	struct nova_inode_log_page *last_page = NULL;
	struct nova_inode_log_page *curr_page = NULL;
	int first_need_free = 0;
	int num_logs;
	u8 btype = sih->i_blk_type;
	unsigned long blocks;
	unsigned long checked_pages = 0;
	int freed_pages = 0;
	timing_t gc_time;

	NOVA_START_TIMING(fast_gc_t, gc_time);
	curr = sih->log_head;
	sih->valid_entries = 0;
	sih->num_entries = 0;

	num_logs = 1;

	nova_dbgv("%s: log head 0x%llx, tail 0x%llx\n",
				__func__, curr, curr_tail);
	while (1) {
		if (curr >> PAGE_SHIFT == sih->log_tail >> PAGE_SHIFT) {
			/* Don't recycle tail page */
			if (found_head == 0) {
				possible_head = cpu_to_le64(curr);
			}
			break;
		}

		curr_page = (struct nova_inode_log_page *)
					nova_get_block(sb, curr);
		next = next_log_page(sb, curr);
		if (next < 0)
			break;

		nova_dbg_verbose("curr 0x%llx, next 0x%llx\n", curr, next);
		if (curr_page_invalid(sb, pi, sih, curr)) {
			nova_dbg_verbose("curr page %p invalid\n", curr_page);
			if (curr == sih->log_head) {
				/* Free first page later */
				first_need_free = 1;
				last_page = curr_page;
			} else {
				nova_dbg_verbose("Free log block 0x%llx\n",
						curr >> PAGE_SHIFT);
				free_curr_page(sb, sih, curr_page, last_page,
						curr);
			}
			NOVA_STATS_ADD(fast_gc_pages, 1);
			freed_pages++;
		} else {
			if (found_head == 0) {
				possible_head = cpu_to_le64(curr);
				found_head = 1;
			}
			last_page = curr_page;
		}

		curr = next;
		checked_pages++;
		if (curr == 0)
			break;
	}

	NOVA_STATS_ADD(fast_checked_pages, checked_pages);
	nova_dbgv("checked pages %lu, freed %d\n", checked_pages, freed_pages);
	checked_pages -= freed_pages;

	// TODO:  I think this belongs in nova_extend_inode_log.
	if (num_pages > 0) {
		curr = BLOCK_OFF(curr_tail);
		curr_page = (struct nova_inode_log_page *)
						  nova_get_block(sb, curr);

		nova_set_next_page_address(sb, curr_page, new_block, 1);
	}

	curr = sih->log_head;

	pi->log_head = possible_head;
	nova_persist_inode(pi);
	sih->log_head = possible_head;
	nova_dbgv("%s: %d new head 0x%llx\n", __func__,
					found_head, possible_head);
	sih->log_pages += (num_pages - freed_pages) * num_logs;
	/* Don't update log tail pointer here */
	nova_flush_buffer(&pi->log_head, CACHELINE_SIZE, 1);

	if (first_need_free) {
		nova_dbg_verbose("Free log head block 0x%llx\n",
					curr >> PAGE_SHIFT);
		nova_free_log_blocks(sb, sih,
				nova_get_blocknr(sb, curr, btype), 1);
	}

	NOVA_END_TIMING(fast_gc_t, gc_time);

	if (sih->num_entries == 0)
		return 0;

	/* Estimate how many pages worth of valid entries the log contains.
	 *
	 * If it is less than half the number pages that remain in the log,
	 * compress them with thorough gc.
	 */
	blocks = (sih->valid_entries * checked_pages) / sih->num_entries;
	if ((sih->valid_entries * checked_pages) % sih->num_entries)
		blocks++;

	if (force_thorough || (blocks && blocks * 2 < checked_pages)) {
		nova_dbgv("Thorough GC for inode %lu: checked pages %lu, valid pages %lu\n",
				sih->ino,
				checked_pages, blocks);
		blocks = nova_inode_log_thorough_gc(sb, pi, sih,
							blocks, checked_pages);
	}

	return 0;
}

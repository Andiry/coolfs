#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x647fa70b, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2dd9d008, __VMLINUX_SYMBOL_STR(kmem_cache_destroy) },
	{ 0x49f4c40d, __VMLINUX_SYMBOL_STR(iget_failed) },
	{ 0xff562007, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x405c1144, __VMLINUX_SYMBOL_STR(get_seconds) },
	{ 0xa776f4e8, __VMLINUX_SYMBOL_STR(drop_nlink) },
	{ 0xb3abd64c, __VMLINUX_SYMBOL_STR(up_read) },
	{ 0x8434076c, __VMLINUX_SYMBOL_STR(make_bad_inode) },
	{ 0x2d416001, __VMLINUX_SYMBOL_STR(generic_file_llseek) },
	{ 0x627bd717, __VMLINUX_SYMBOL_STR(__mark_inode_dirty) },
	{ 0xd6ee688f, __VMLINUX_SYMBOL_STR(vmalloc) },
	{ 0xce31b8a1, __VMLINUX_SYMBOL_STR(pv_lock_ops) },
	{ 0xb0ff812d, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xa6e82d2d, __VMLINUX_SYMBOL_STR(generic_write_checks) },
	{ 0x9143f5b7, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x188a3dfb, __VMLINUX_SYMBOL_STR(timespec_trunc) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x60a13e90, __VMLINUX_SYMBOL_STR(rcu_barrier) },
	{ 0x815b5dd4, __VMLINUX_SYMBOL_STR(match_octal) },
	{ 0xda5e7434, __VMLINUX_SYMBOL_STR(generic_fh_to_parent) },
	{ 0x1db7706b, __VMLINUX_SYMBOL_STR(__copy_user_nocache) },
	{ 0x79aa04a2, __VMLINUX_SYMBOL_STR(get_random_bytes) },
	{ 0x2d1be642, __VMLINUX_SYMBOL_STR(single_release) },
	{ 0x70137a1, __VMLINUX_SYMBOL_STR(seq_puts) },
	{ 0xff00176, __VMLINUX_SYMBOL_STR(memcpy_mcsafe_unrolled) },
	{ 0xea17fd1d, __VMLINUX_SYMBOL_STR(is_bad_inode) },
	{ 0x80acdda4, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0xdfbb3720, __VMLINUX_SYMBOL_STR(generic_file_open) },
	{ 0x4a405c6, __VMLINUX_SYMBOL_STR(touch_atime) },
	{ 0xc0a3d105, __VMLINUX_SYMBOL_STR(find_next_bit) },
	{ 0x10651218, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0xce5c17f5, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x6729d3df, __VMLINUX_SYMBOL_STR(__get_user_4) },
	{ 0x44e9a829, __VMLINUX_SYMBOL_STR(match_token) },
	{ 0x5e5edb62, __VMLINUX_SYMBOL_STR(inc_nlink) },
	{ 0xbf98f02, __VMLINUX_SYMBOL_STR(init_user_ns) },
	{ 0xbf63e976, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xcefec384, __VMLINUX_SYMBOL_STR(mount_bdev) },
	{ 0x85df9b6c, __VMLINUX_SYMBOL_STR(strsep) },
	{ 0xe6c4ce5c, __VMLINUX_SYMBOL_STR(generic_read_dir) },
	{ 0x999e8297, __VMLINUX_SYMBOL_STR(vfree) },
	{ 0x36aec3d8, __VMLINUX_SYMBOL_STR(radix_tree_gang_lookup_slot) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0x27000b29, __VMLINUX_SYMBOL_STR(crc32c) },
	{ 0x26948d96, __VMLINUX_SYMBOL_STR(copy_user_enhanced_fast_string) },
	{ 0x92bf8260, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0x254c531b, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0xe7694524, __VMLINUX_SYMBOL_STR(down_read) },
	{ 0xdae7e53e, __VMLINUX_SYMBOL_STR(kthread_bind) },
	{ 0xece784c2, __VMLINUX_SYMBOL_STR(rb_first) },
	{ 0x933ae209, __VMLINUX_SYMBOL_STR(make_kgid) },
	{ 0x19ad6985, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xb4684f0d, __VMLINUX_SYMBOL_STR(PDE_DATA) },
	{ 0xfe7c4287, __VMLINUX_SYMBOL_STR(nr_cpu_ids) },
	{ 0xe8ef0ff0, __VMLINUX_SYMBOL_STR(current_kernel_time64) },
	{ 0xff8ea28d, __VMLINUX_SYMBOL_STR(inode_owner_or_capable) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xeab12d78, __VMLINUX_SYMBOL_STR(__cpu_possible_mask) },
	{ 0x5fce5c6, __VMLINUX_SYMBOL_STR(from_kuid) },
	{ 0x56fe39d9, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0x90164edc, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xf0c2bfce, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x41110c09, __VMLINUX_SYMBOL_STR(d_obtain_alias) },
	{ 0x477e59a3, __VMLINUX_SYMBOL_STR(__cpu_online_mask) },
	{ 0x64f517aa, __VMLINUX_SYMBOL_STR(dax_writeback_mapping_range) },
	{ 0xafb8c6ff, __VMLINUX_SYMBOL_STR(copy_user_generic_string) },
	{ 0x7c1372e8, __VMLINUX_SYMBOL_STR(panic) },
	{ 0xb8ec8959, __VMLINUX_SYMBOL_STR(file_remove_privs) },
	{ 0x479c3c86, __VMLINUX_SYMBOL_STR(find_next_zero_bit) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x4d9b652b, __VMLINUX_SYMBOL_STR(rb_erase) },
	{ 0xf3341268, __VMLINUX_SYMBOL_STR(__clear_user) },
	{ 0xc99738b, __VMLINUX_SYMBOL_STR(from_kgid) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0x4b8b3239, __VMLINUX_SYMBOL_STR(vprintk) },
	{ 0x8b2bda43, __VMLINUX_SYMBOL_STR(kmem_cache_free) },
	{ 0x74aed678, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x288cff21, __VMLINUX_SYMBOL_STR(set_nlink) },
	{ 0xe85c92ce, __VMLINUX_SYMBOL_STR(file_update_time) },
	{ 0xc0fb88, __VMLINUX_SYMBOL_STR(setattr_copy) },
	{ 0x5de48262, __VMLINUX_SYMBOL_STR(insert_inode_locked) },
	{ 0x63c9e731, __VMLINUX_SYMBOL_STR(truncate_pagecache) },
	{ 0x4e3567f7, __VMLINUX_SYMBOL_STR(match_int) },
	{ 0xa4da2130, __VMLINUX_SYMBOL_STR(up_write) },
	{ 0xe7e70ac9, __VMLINUX_SYMBOL_STR(down_write) },
	{ 0x952664c5, __VMLINUX_SYMBOL_STR(do_exit) },
	{ 0xfe487975, __VMLINUX_SYMBOL_STR(init_wait_entry) },
	{ 0x7e880422, __VMLINUX_SYMBOL_STR(radix_tree_delete) },
	{ 0xbc7f8b04, __VMLINUX_SYMBOL_STR(inode_init_once) },
	{ 0x72a98fdb, __VMLINUX_SYMBOL_STR(copy_user_generic_unrolled) },
	{ 0x848f1f07, __VMLINUX_SYMBOL_STR(mnt_drop_write_file) },
	{ 0xc6cbbc89, __VMLINUX_SYMBOL_STR(capable) },
	{ 0x6958ae23, __VMLINUX_SYMBOL_STR(dax_get_by_host) },
	{ 0x40a9b349, __VMLINUX_SYMBOL_STR(vzalloc) },
	{ 0x834bda33, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x86235596, __VMLINUX_SYMBOL_STR(radix_tree_replace_slot) },
	{ 0xb2fd5ceb, __VMLINUX_SYMBOL_STR(__put_user_4) },
	{ 0x17bdccc6, __VMLINUX_SYMBOL_STR(make_kuid) },
	{ 0x16311bce, __VMLINUX_SYMBOL_STR(radix_tree_lookup_slot) },
	{ 0xf82ec573, __VMLINUX_SYMBOL_STR(rb_prev) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x8ddd8aad, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x5bfb2020, __VMLINUX_SYMBOL_STR(unlock_new_inode) },
	{ 0xf9a48ec9, __VMLINUX_SYMBOL_STR(mnt_want_write_file) },
	{ 0xb664682c, __VMLINUX_SYMBOL_STR(kill_block_super) },
	{ 0x6b2dc060, __VMLINUX_SYMBOL_STR(dump_stack) },
	{ 0xad1ab841, __VMLINUX_SYMBOL_STR(inode_newsize_ok) },
	{ 0x39538740, __VMLINUX_SYMBOL_STR(dax_direct_access) },
	{ 0x9c3584c3, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xec018b66, __VMLINUX_SYMBOL_STR(__radix_tree_insert) },
	{ 0x123f82f3, __VMLINUX_SYMBOL_STR(getrawmonotonic64) },
	{ 0xad3b2f79, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xe259ae9e, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x5ecfeec6, __VMLINUX_SYMBOL_STR(__per_cpu_offset) },
	{ 0xa5526619, __VMLINUX_SYMBOL_STR(rb_insert_color) },
	{ 0x73e8f167, __VMLINUX_SYMBOL_STR(kmem_cache_create) },
	{ 0x7ed8cf06, __VMLINUX_SYMBOL_STR(register_filesystem) },
	{ 0x5b205e34, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x7a2b37e3, __VMLINUX_SYMBOL_STR(dax_iomap_fault) },
	{ 0x69e43cfd, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x339adbed, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x246d43b3, __VMLINUX_SYMBOL_STR(seq_lseek) },
	{ 0xc5ed6547, __VMLINUX_SYMBOL_STR(iput) },
	{ 0x16d816e9, __VMLINUX_SYMBOL_STR(dax_iomap_rw) },
	{ 0xadb87297, __VMLINUX_SYMBOL_STR(generic_file_fsync) },
	{ 0xa1489164, __VMLINUX_SYMBOL_STR(invalidate_inode_pages2_range) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xda25f6c2, __VMLINUX_SYMBOL_STR(inode_dio_wait) },
	{ 0x5e7f7a9e, __VMLINUX_SYMBOL_STR(ihold) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0xff992afc, __VMLINUX_SYMBOL_STR(__sb_end_write) },
	{ 0x643e0ce5, __VMLINUX_SYMBOL_STR(call_rcu_sched) },
	{ 0x6bf39a71, __VMLINUX_SYMBOL_STR(mcsafe_key) },
	{ 0xa2186f11, __VMLINUX_SYMBOL_STR(current_time) },
	{ 0x625001ea, __VMLINUX_SYMBOL_STR(d_splice_alias) },
	{ 0x13ab4a95, __VMLINUX_SYMBOL_STR(__sb_start_write) },
	{ 0x715e3fa8, __VMLINUX_SYMBOL_STR(d_make_root) },
	{ 0xced6bcd8, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x63c4d61f, __VMLINUX_SYMBOL_STR(__bitmap_weight) },
	{ 0xca9360b5, __VMLINUX_SYMBOL_STR(rb_next) },
	{ 0xb262c4f2, __VMLINUX_SYMBOL_STR(unregister_filesystem) },
	{ 0x7dbf8142, __VMLINUX_SYMBOL_STR(init_special_inode) },
	{ 0xaf2c6eaa, __VMLINUX_SYMBOL_STR(new_inode) },
	{ 0xb893cc47, __VMLINUX_SYMBOL_STR(__bdev_dax_supported) },
	{ 0x7e4e070b, __VMLINUX_SYMBOL_STR(noop_fsync) },
	{ 0x7a380c0d, __VMLINUX_SYMBOL_STR(generic_fh_to_dentry) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x3f39723a, __VMLINUX_SYMBOL_STR(clear_inode) },
	{ 0x9b7d6800, __VMLINUX_SYMBOL_STR(d_instantiate) },
	{ 0xea4217ba, __VMLINUX_SYMBOL_STR(__init_rwsem) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x75f2efef, __VMLINUX_SYMBOL_STR(clear_nlink) },
	{ 0x4b58cca0, __VMLINUX_SYMBOL_STR(iget_locked) },
	{ 0x431b0559, __VMLINUX_SYMBOL_STR(vfs_fsync_range) },
	{ 0x11677758, __VMLINUX_SYMBOL_STR(setattr_prepare) },
	{ 0xe0c5087c, __VMLINUX_SYMBOL_STR(generic_fillattr) },
	{ 0xf827c5c0, __VMLINUX_SYMBOL_STR(inode_init_owner) },
	{ 0xe433dad1, __VMLINUX_SYMBOL_STR(truncate_inode_pages) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=libcrc32c";


MODULE_INFO(srcversion, "E9A6779F1EF7DA856C3B5CD");

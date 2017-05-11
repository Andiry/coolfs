# NOVA: NOn-Volatile memory Accelerated log-structured file system

## Introduction
NOVA is a log-structured file system designed for byte-addressable non-volatile memories, developed by
the [Non-Volatile Systems Laboratory][NVSL], University of California, San Diego.

NOVA extends ideas of LFS to leverage NVMM, yielding a simpler, high-performance file system that supports fast and efficient garbage collection and quick recovery from system failures.
NOVA has passed the [Linux POSIX test suite][POSIXtest], and existing applications need not be modified to run on NOVA. NOVA bypasses the block layer and OS page cache, writes to NVM directly and reduces the software overhead.

NOVA provides strong data consistency guanrantees:

* Atomic metadata update: each directory operation is atomic.
* Atomic data update; for each `write` operation, the file data and the inode are updated in a transactional way.
* DAX-mmap: NOVA supports DAX-mmap which maps NVMM pages directly to the user space.

With atomicity guarantees, NOVA is able to recover from system failures and restore to a consistent state.

For more details about the design and implementation of NOVA, please see this paper:

**NOVA: A Log-structured File system for Hybrid Volatile/Non-volatile Main Memories**<br>
[PDF](http://cseweb.ucsd.edu/~swanson/papers/FAST2016NOVA.pdf)<br>
*Jian Xu and Steven Swanson, University of California, San Diego*<br>
Published in FAST 2016

## Building NOVA
NOVA works on the 4.10-rc8 version of x86-64 Linux kernel.

To build NOVA, first apply patches under kernel-patches directory to your kernel source and compile the kernel,
and then in nova directory run a

~~~
#make
~~~

command.

## Running NOVA
NOVA runs on a physically contiguous memory region that is not used by the Linux kernel, and relies on the kernel NVDIMM support.

To run NOVA, first build up your kernel with NVDIMM support enabled (`CONFIG_BLK_DEV_PMEM`), and then you can
reserve the memory space by booting the kernel with `memmap` command line option.

For instance, adding `memmap=16G!8G` to the kernel boot parameters will reserve 16GB memory starting from 8GB address, and the kernel will create a `pmem0m` block device under the `/dev` directory.

After the OS has booted, you can initialize a NOVA instance with the following commands:


~~~
#insmod nova.ko
#mount -t NOVA -o init /dev/pmem0m /mnt/ramdisk
~~~

The above commands create a NOVA instance on pmem0m device, and mount on `/mnt/ramdisk`.

To recover an existing NOVA instance, mount NOVA without the init option, for example:

~~~
#mount -t NOVA /dev/pmem0m /mnt/ramdisk
~~~

There are two scripts provided in the source code, `setup-nova.sh` and `remount-nova.sh` to help setup NOVA.

## Snapshot support
NOVA is a snapshot (checkpointing) file system. It provides a consistent view of the entire file system at a particular time, so that users can restore files that are mistakenly overwritten or deleted.

To create a snapshot:

~~~
#echo 1 > /proc/fs/NOVA/<device>/create_snapshot
~~~

To list the current snapshots:

~~~
#cat /proc/fs/NOVA/<device>/snapshots
~~~

To delete a snapshot, specify the snapshot index which is given by the previous command:

~~~
#echo <index> > /proc/fs/NOVA/<device>/delete_snapshot
~~~

To mount a snapshot, mount NOVA and specifying the snapshot index, for example:

~~~
#mount -t NOVA -o snapshot=<index> /dev/pmem0m /mnt/ramdisk
~~~

Users should not write to the file system after mounting a snapshot.

## Current limitations

* NOVA only works on x86-64 kernels.
* NOVA does not currently support extended attributes or ACL.
* NOVA requires the underlying block device to support DAX (Direct Access) feature.

[NVSL]: http://nvsl.ucsd.edu/ "http://nvsl.ucsd.edu"
[POSIXtest]: http://www.tuxera.com/community/posix-test-suite/ 

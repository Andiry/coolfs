#!/bin/sh

umount /mnt/ramdisk
umount /mnt/scratch
rmmod nova
insmod nova.ko measure_timing=0 inplace_data_updates=1 replica_inode=0 replica_log=0

sleep 1

mount -t NOVA -o init /dev/pmem0 /mnt/ramdisk
mount -t NOVA -o init /dev/pmem1 /mnt/scratch


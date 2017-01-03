#!/bin/sh
set -x

rm -f /tmp/tfile1.txt

make
insmod trfs.ko

mount -t ext4 /dev/sda1 /mnt/lower/
mount -t trfs -o tfile=/tmp/tfile1.txt /mnt/lower/ /mnt/trfs/


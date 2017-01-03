set -x

umount /mnt/trfs/
umount /mnt/lower/

rmmod trfs
make clean
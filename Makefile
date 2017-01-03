TRFS_VERSION="1.0"

EXTRA_CFLAGS += -DTRFS_VERSION=\"$(TRFS_VERSION)\"

obj-m += trfs.o

trfs-y := dentry.o file.o inode.o main.o super.o lookup.o mmap.o

default:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

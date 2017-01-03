# /usr/src/hw2-mengyang/hw2/trctl /mnt/trfs

# /usr/src/hw2-mengyang/hw2/trctl none /mnt/trfs

# /usr/src/hw2-mengyang/hw2/trctl all /mnt/trfs

# only open can be record
# /usr/src/hw2-mengyang/hw2/trctl 0x00000001 /mnt/trfs

# only close can be record
# /usr/src/hw2-mengyang/hw2/trctl 0x00000002 /mnt/trfs

# only read can be record
# /usr/src/hw2-mengyang/hw2/trctl 0x00000004 /mnt/trfs

# only write can be record
# /usr/src/hw2-mengyang/hw2/trctl 0x00000008 /mnt/trfs

# only create can be record
# /usr/src/hw2-mengyang/hw2/trctl 0x00000010 /mnt/trfs

echo "test test.">>/mnt/trfs/home/student/a.txt
cat /mnt/trfs/home/student/a.txt

echo "I am testing.">>/mnt/trfs/home/student/b.txt

rm -f /mnt/trfs/home/student/a.txt
rm -f /mnt/trfs/home/student/b.txt


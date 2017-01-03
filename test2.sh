# test only create and write
/usr/src/hw2-mengyang/hw2/trctl 0x00000018 /mnt/trfs

echo "test test.">>/mnt/trfs/home/student/a.txt
cat /mnt/trfs/home/student/a.txt

echo "I am testing.">>/mnt/trfs/home/student/b.txt

rm -f /mnt/trfs/home/student/a.txt
rm -f /mnt/trfs/home/student/b.txt
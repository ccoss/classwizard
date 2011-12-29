#!/bin/sh

revertpart=null
if [ $1 != "" ]; then revertpart=$1; fi
revertpart=/dev/$1
echo "$revertpart"
umount /mnt/dev
umount /mnt/sys
umount /mnt/proc
mkdir /revert
umount $revertpart
mkfs.ext3 $revertpart
mount $revertpart /revert

tar zxvf /home/revert/revertsystem.tar.gz -C /revert
mkdir /revert/home/copysystem
cp -rf /mnt/* /revert/home/copysystem
#cd /mnt

#tar zcvf copy.tar.gz *
#mv copy.tar.gz /revert/home/copysystem

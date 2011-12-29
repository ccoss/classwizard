#!/bin/bash

user=

if [ $1 != "" ]; then user=$1; fi
addLine=$user
sed '/root\tALL=(ALL) ALL/a\'$addLine'	ALL=(ALL) ALL' /mnt/etc/sudoers >/mnt/etc/sudoers.tmp
rm /mnt/etc/sudoers
mv /mnt/etc/sudoers.tmp /mnt/etc/sudoers
chroot /mnt chmod 0440 /etc/sudoers

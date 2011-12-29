#!/bin/bash

hostname=

if [ "$1" != "" ]; then hostname=$1; fi

echo "rm /mnt/*.back"
rm -rfv /mnt/*.back
echo "${hostname}">hostname.tmp
mv hostname.tmp /mnt
chroot /mnt mv hostname.tmp /etc/hostname
sed 's/kubuntu/'$hostname'/' /mnt/etc/hosts > /mnt/etc/hosts.tmp
rm /mnt/etc/hosts
mv /mnt/etc/hosts.tmp /mnt/etc/hosts


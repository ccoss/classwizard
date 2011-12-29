#!/bin/bash

hd=

if [ $1 != "" ]; then hd=$1; fi
echo "hd = $hd"
hdPath=/dev/$hd
echo "chroot /mnt grub-install --force $hdPath"
echo "chroot /mnt update-grub"
chroot /mnt grub-install --force $hdPath
chroot /mnt update-grub

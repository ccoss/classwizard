#!/bin/bash

hd=

if [ $1 != "" ]; then hd=$1; fi
echo "hd = $hd"
hdPath=/dev/$hd
echo "chroot /mnt grub-install --force $hdPath"
echo "chroot /mnt update-grub"
mount --bind /dev /mnt/$hd/dev
mount --bind /sys /mnt/$hd/sys
mount --bind /proc /mnt/$hd/proc
chroot /mnt/$hd update-grub

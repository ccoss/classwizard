#!/bin/sh
#sed -n 's/###\ END\ \/etc\/grub.d\/10_linux\ ###/menuentry\ 'revert'\ --class\ 共创gaea\ --class\ gnu-linux\ --class\ gnu\ --class os\ {\nrecordfail\ninsmod\ ext2\nset\ root='(hd0,1)'\nlinux\ \ \ /live/vmlinuz1\ boot=live\ config\ i915.modeset=0\ xforcevesa\ radeon.meset\ noprompt\ vga=788\ ip=frommedia\ nosplash\ BOOT_IMAGE=/live/vmlinuz1\ninitrd\ \ /live/initrd1.img\n}\n/p' grub.cfg
hd=
part=
rootpart=

if [ $1 != "" ]; then hd=$1;fi
part=$2
rootpart=$3
echo "***********************************"
num=${part#sd[a-z]*}
echo "***********************************"
sed 's/###\ END\ \/etc\/grub.d\/10_linux\ ###/menuentry\ '\'revert\'' \ --class\ 共创gaea\ --class\ gnu-linux\ --class\ gnu\ --class os\ \{\nrecordfail\ninsmod\ ext2\nset root='\($hd,$num\)'\nlinux\ \ \ \/live\/vmlinuz1\ boot=live\ config\ i915.modeset=0\ xforcevesa\ radeon.meset\ noprompt\ nofstab\ vga=788\ ip=frommedia\ nosplash\ BOOT_IMAGE=\/live\/vmlinuz1\ninitrd\ \/live\/initrd1.img\n\}\n###\ END\ \/etc\/grub.d\/10_linux\ ###/p' /mnt/$rootpart/boot/grub/grub.cfg > /mnt/$rootpart/boot/grub/grub.cfg.tmp

rm /mnt/$rootpart/boot/grub/grub.cfg
mv /mnt/$rootpart/boot/grub/grub.cfg.tmp /mnt/$rootpart/boot/grub/grub.cfg

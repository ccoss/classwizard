#!/bin/sh
#sed -n 's/###\ END\ \/etc\/grub.d\/10_linux\ ###/menuentry\ 'revert'\ --class\ 共创gaea\ --class\ gnu-linux\ --class\ gnu\ --class os\ {\nrecordfail\ninsmod\ ext2\nset\ root='(hd0,1)'\nlinux\ \ \ /live/vmlinuz1\ boot=live\ config\ i915.modeset=0\ xforcevesa\ radeon.meset\ noprompt\ vga=788\ ip=frommedia\ nosplash\ BOOT_IMAGE=/live/vmlinuz1\ninitrd\ \ /live/initrd1.img\n}\n/p' grub.cfg
hd=
part=

if [ $1 != "" ]; then hd=$1;fi
if [ $2 != "" ]; then part=$2;fi
sed 's/###\ END\ \/etc\/grub.d\/10_linux\ ###/menuentry\ '\'revert\'' \ --class\ 共创gaea\ --class\ gnu-linux\ --class\ gnu\ --class os\ \{\nrecordfail\ninsmod\ ext2\nset root='\($hd,$part\)'\nlinux\ \ \ \/live\/vmlinuz1\ boot=live\ config\ i915.modeset=0\ xforcevesa\ radeon.meset\ noprompt\ \ nofstab vga=788\ ip=frommedia\ nosplash\ BOOT_IMAGE=\/live\/vmlinuz1\ninitrd\ \/live\/initrd1.img\n\}/p' /mnt/boot/grub/grub.cfg > /mnt/boot/grub/grub.cfg.tmp

rm /mnt/boot/grub/grub.cfg
mv /mnt/boot/grub/grub.cfg.tmp /mnt/boot/grub/grub.cfg

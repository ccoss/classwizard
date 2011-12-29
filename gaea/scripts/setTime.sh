#!/bin/bash

#*******************argument***************
# $1 时区名称
#******************************************

timezoneLocal=$1
path=/usr/share/zoneinfo/Asia/Shanghai/Shanghai
setZonePath=$path

#if [ timezoneLocal == "China(Shanghai)" ]; then
chroot /mnt ln -s $path /etc/localetime
chroot /mnt ntpdate 210.72.145.44
chroot /mnt hwclock -w
#else
#	echo "no $timezoneLocal zone"
#fi
date

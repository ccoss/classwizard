#!/bin/bash
partition_form=null
partition_no=null
partition_swap=null
args1=null
args2=null
temp=null

if [ "$1" != "" ];then partition_form=$1;fi
if [ "$2" != "" ];then partition_no=$2;fi
if [ "$3" != "" ];then partition_swap=$3;fi
#TEST--echo "$partition_form"

#args1=${partition_no:3:4}
args1=`echo $partition_no | sed 's/.*\(.$\)/\1/'`
#TEST--echo $args1

temp=$(awk '/'$args1'/' ./dic)
#args2=${temp:2:3}
args2=`echo $temp | sed 's/.*\(.$\)/\1/'`
#TEST--echo $args2

cat > boot.cfg << "EOF"
timeout 30
default 0
showmenu 1
title CoCreate Linux System
EOF
echo "	kernel /dev/fs/ext2@wd0$args2/vmlinux" >> ./boot.cfg
echo "	args console=tty quiet root=/dev/$partition_no resume=/dev/$partition_swap splash init=/sbin/finit-mod" >>./boot.cfg
mv boot.cfg /mnt/boot

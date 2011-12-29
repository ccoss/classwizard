#!/bin/bash

LANG=C
[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
###检测磁盘
	#探测设备、分区大小	
get_part_block_size() {
        s=0 
        d=$1
        if [ -b "$1" ]; then
                d=${d#/dev/}
                dev=${d%[1-9]*}
	        [ ${dev} = ${d} ] && s=$(cat /sys/block/${dev}/size) || s=$(cat /sys/block/${dev}/${d}/size)
        fi    
        echo $s
}
	#以MB为单位显示设备、分区大小
get_hd_mb_size() {
        s=$(get_part_block_size $1)
        s=$(($s * 512 / 1024 / 1024))
        echo $s
}

###格式化磁盘
	#强制卸载分区
force_umount_part() {
	        umount -l $1 2>/dev/null
}
force_umount_parts() {
        for d in $1[1-9]*; do
                [ "$d" = "$1[1-9]*" ] && return
                umount -l $d 2>/dev/null
	        done
}
	# 低格
lformat_part() {
        force_umount_part $1
        ${LFORMAT_TOOL} ${LFORMAT_ARGS} $1
}
	# 格式化
format_part() {
        force_umount_part $1
        ${FORMAT_TOOL} ${FORMAT_ARGS} $1
}

###修复磁盘
fsck_part() {
        force_umount_part $1
        ${FSCK_TOOL} ${FSCK_ARGS} $1
}

###磁盘分区
	# 创建分区
	# $1: /dev/sdx, $2: primary/logical/extended, $3: fstype, $4: begin, $5: end (MB)
create_part() {
	        ${PART_TOOL} -s $1 mkpart $2 $3 $4 $5
}


partprobe_dev() {

	partprobe $1

}     




###删除分区
# $1 : /dev/sdx
# $2 : [1-9]*
delete_part() {
        force_umount_part "$1$2"
        ${PART_TOOL} -s $1 rm $2
}
### 删除设备所有分区
delete_all_parts() {
        for d in $(for f in $1[1-9]*;do echo ${f#$1};done | sort -r -n); do
                [ "$d" = "[1-9]*" ] && break
                delete_part $1 ${d}
                sleep 0.5
	        done
	        partprobe_dev $1
	        sleep 1
}
### 挂载分区
mount_part(){
	 [ -n "$1" ] && ${FSCK_TOOL} -p $1
	 mount -t ${FSTYPE} -o rw $1 $2 2>/dev/null
}

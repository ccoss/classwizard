#!/bin/bash
   
    [ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts
      
     . ${SCRIPTDIR}/gaea.conf
     . ${SCRIPTDIR}/autopart.sh

hd=
fstype=
mnt=
crtype=
size=

if [ "$1" != "" ];then hd=$1;fi
if [ "$2" != "" ];then fstype=$2;fi
if [ "$3" != "" ];then mnt=$3;fi
if [ "$4" != "" ];then crtype=$4;fi
if [ "$5" != "" ];then size=$5;fi

swap=linux-swap

if [ "${fstype}" = "${swap}" ];then
	basename $hd | grep "[0-9]" > /dev/null
	if [ $? = 0 ];then 
        	B1=`echo $hd | sed -e "s/[0-9]/ &/"`
        	B2=`echo $B1 | awk '{print $1}'`
	        B3=`echo $B1 | awk '{print $2}'`
		parted -s $B2 mkfs $B3 ${fstype}
	else 
		echo "Device error!!!"
	fi
else
	force_umount_part ${hd}
	#echo "mkfs.${fstype} ${hd}"
	mkfs.${fstype} ${hd}
fi

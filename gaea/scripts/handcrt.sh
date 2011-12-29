#!/bin/bash

   [ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

   . ${SCRIPTDIR}/gaea.conf
   . ${SCRIPTDIR}/autopart.sh

hd=
crtype=
hd_head=
hd_tail=

if [ "$1" != "" ];then hd=$1;fi
if [ "$2" != "" ];then crtype=$2;fi
if [ "$3" != "" ];then hd_head=$3;fi
if [ "$4" != "" ];then hd_tail=$4;fi


###检测整块硬盘大小
#total=$(get_hd_mb_size ${hd} )
#echo $TOTAL
#echo  "disk total size: ${total} MB"

###设定分区大小
#echo "create_part ${hd} ${crtype}  ${hd_head} ${hd_tail}"
create_part ${hd} ${crtype}  ${hd_head} ${hd_tail}


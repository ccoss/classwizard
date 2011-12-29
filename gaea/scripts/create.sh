#!/bin/bash

[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
. ${SCRIPTDIR}/autopart.sh

###检测整块硬盘大小
total=$(get_hd_mb_size ${HD})
echo $TOTAL
echo  "disk total size: ${total} MB"
###设定分区大小
       head=1
       echo ${head}
       HD_1_tail=10240
       echo "${HD_1_tail} MB"
       HD_2_tail=$(( ${HD_1_tail} + (${total} - 10240) / 2))
       echo "${HD_2_tail} MB"
       HD_3_tail=$total
       echo "${HD_3_tail} MB"
sleep 1
###创建分区
        #创建第一分区
       create_part ${HD} primary ${FSTYPE}  ${head}      ${HD_1_tail}
      #  echo "create_part  ${head}      ${HD_1_tail}"
        #创建第二分区
       create_part ${HD} primary ${FSTYPE}  ${HD_1_tail} ${HD_2_tail}
     #   echo "create_part  ${HD_1_tail} ${HD_2_tail}"
        #创建第三分区
       create_part ${HD} primary ${FSTYPE}  ${HD_2_tail} ${HD_3_tail}
     #   echo "create_part  ${HD_2_tail} ${HD_3_tail}" 



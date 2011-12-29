#!/bin/bash

[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
. ${SCRIPTDIR}/autopart.sh

###格式化分区
        #格式化第一分区
       format_part ${HD_1}
#        echo "format_part ${HD_1}"
        #格式化第二分区
       format_part ${HD_2}
#        echo "format_part ${HD_2}"
        #格式化第三分区
       format_part ${HD_3} 
#        echo "format_part ${HD_3}"

#echo "end create tsos partition"


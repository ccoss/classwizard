#!/bin/bash

[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
. ${SCRIPTDIR}/autopart.sh

hd=

if [ "$1" != "" ];then
	hd=$1
else
	hd=$HD
fi

#echo "begin create tsos partition"

#echo "delete_all_parts $hd"
delete_all_parts $hd
#echo "over"


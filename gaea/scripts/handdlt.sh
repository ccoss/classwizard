#!/bin/bash

[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
. ${SCRIPTDIR}/autopart.sh

hd=

if [ "$1" != "" ];then 
	hd=$1
else 
	echo "Need a deviceName!"
fi

#echo "delete_all_parts $hd"
basename $hd | grep "[0-9]" > /dev/null
if [ $? = 0 ];then 
	eval `echo $hd | sed -e "s/^/delete_part /; s/[0-9]/ &/"`
else
	delete_all_parts $hd
fi
#echo "over"


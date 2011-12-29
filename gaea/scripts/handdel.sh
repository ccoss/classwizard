#!/bin/bash

[ -z "${SCRIPTDIR}" ] && export SCRIPTDIR=/etc/gaea/scripts

. ${SCRIPTDIR}/gaea.conf
. ${SCRIPTDIR}/autopart.sh

hd=

if [ "$1" != "" ];then 
	hd=$1
else 
	echo "Need a deviceName!"
	exit
fi

echo "delete_part $hd"

basename $hd | grep "[0-9]" > /dev/null
if [ $? = 0 ];then 
	B1=`echo $hd | sed -e "s/[0-9]/ &/"`
	B2=`echo $B1 | awk '{print $1}'`
	B3=`echo $B1 | awk '{print $2}'`
	echo $B2
	echo $B3
	parted -s  $B2 rm $B3	
	
else 
	echo "error"
fi	
echo "over"


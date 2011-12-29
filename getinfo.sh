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

name=`basename ${hd}`

parted -s ${hd} print > ${SCRIPTDIR}/${name}.txt

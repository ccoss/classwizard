#!/bin/bash

dev=
file=

if [ $1 != "" ]; then dev=$1; fi;
echo "dev=$dev"
file=uuid
echo "file=$file"
if [ -e /etc/gaea/scripts/uuid ]
then
	echo "rm uuid"
	rm /etc/gaea/scripts/uuid
fi
/sbin/blkid $dev > /etc/gaea/scripts/uuid

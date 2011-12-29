#!/bin/bash

host=

if [ host != "" ]; then host=$1; fi

echo "sed '/kubuntu/$host/' /mnt/etc/hosts > /mnt/etc/hosts.tmp"
sed 's/kubuntu/'$host'/' /mnt/etc/hosts > /mnt/etc/hosts.tmp
rm /mnt/etc/hosts
mv /mnt/etc/hosts.tmp /mnt/etc/hosts

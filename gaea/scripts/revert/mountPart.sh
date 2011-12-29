#!/bin/bash
mountDev=$1
mountRootDev=$2

echo "$1"
mkdir /mnt/$mountDev
mount /dev/$mountDev /mnt/$mountDev
mkdir /mnt/$mountRootDev
mount /dev/$mountRootDev /mnt/$mountRootDev

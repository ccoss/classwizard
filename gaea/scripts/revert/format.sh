#!/bin/bash

dev=
rootPath=
if [ -n $1 ]; then dev=$1;fi
if [ -n $2 ]; then rootPath=$2;fi
#if [ -n $2 ]; then rootPath=$2;fi
#if [ $1 != "" ]; then srcPath=$1;fi
#if [ $2 != "" ]; then rootPath=$2;fi

echo "mkfs.ext3 $dev"
umount $dev
mkfs.ext3 $dev
mount $dev $rootPath

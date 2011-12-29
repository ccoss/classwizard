#!/bin/sh
backupPart=$1
hostName=$2
cd $backupPart
tar zcf $hostName *
#tar zcf $hostName $backupPart

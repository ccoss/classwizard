#!/bin/bash

user=
passwd=

if [ $1 != "" ]; then user=$1; fi
if [ $2 != "" ]; then passwd=$2; fi

sed '/\[X-:0-Core\]/a\AutoLoginPass='$passwd'' /mnt/etc/kde4/kdm/kdmrc >kdmrc.1
sed '/\[X-:0-Core\]/a\AutoLoginUser='$user'' ./kdmrc.1 >kdmrc.2
sed '/\[X-:0-Core\]/a\AutoLoginEnable=true\' ./kdmrc.2 >kdmrc.3

rm /mnt/etc/kde4/kdm/kdmrc
mv kdmrc.3 /mnt/etc/kde4/kdm/kdmrc
rm kdmrc.*

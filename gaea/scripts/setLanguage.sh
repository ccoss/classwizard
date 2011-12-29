#!/bin/bash
#***********************argument**********************
#$1 选择语言如果是中文则输出Chinese
#*****************************************************

language=

if [ "$1" != "" ];then language=$1; fi

chroot /mnt  echo "set language"
if [ "${language}" == "China" ];then
	echo "LANG=zh_CN.UTF-8
LANGUAGE=zh_CN:zh">locale.tmp
echo "chinese"
else
	echo "LANG=en_US.UTF-8
LANGUAGE=en_US:en">locale.tmp
echo "english"
fi
mv locale.tmp /mnt
chroot /mnt  mv locale.tmp /etc/default/locale

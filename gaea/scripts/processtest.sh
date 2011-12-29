#!/bin/sh

process=$(pidof classwizard)
echo $process

if [ "$process" != "" ]
then
	echo "classwizard is up!"
else
	sudo /home/source/classwizard
	echo "classwizard is not up!"
fi

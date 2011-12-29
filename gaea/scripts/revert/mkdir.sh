#!/bin/sh
if [ -d $1 ]; then
	echo "$1 is exist!"
else
	mkdir $1
fi

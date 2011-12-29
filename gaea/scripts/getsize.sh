#!/bin/bash

path=

if [ "$1" != "" ]; then path=$1; fi;

/usr/bin/du -sh $path > /etc/gaea/scripts/tarSize

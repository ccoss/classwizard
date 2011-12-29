#!/bin/bash

src=
dst=

if [ "$1" != "" ]; then src=$1; fi
if [ "$2" != "" ]; then dst=$2; fi
src1=$1/*
/bin/cp -rf $src1 $dst

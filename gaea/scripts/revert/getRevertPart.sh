#!/bin/sh
df -h > /tmp/revert.df
sed -n '/live\/image/p' /tmp/revert.df > /tmp/revert.cow

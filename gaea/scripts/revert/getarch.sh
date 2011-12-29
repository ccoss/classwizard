#!/bin/sh
if [ -f /etc/gaea/scripts/revert/architecture ]; then
	rm /etc/gaea/scripts/revert/architecture
fi
cat /proc/cpuinfo | grep model | grep Loongson > /etc/gaea/scripts/revert/architecture

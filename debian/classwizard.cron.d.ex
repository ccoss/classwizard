#
# Regular cron jobs for the classwizard package
#
0 4	* * *	root	[ -x /usr/bin/classwizard_maintenance ] && /usr/bin/classwizard_maintenance

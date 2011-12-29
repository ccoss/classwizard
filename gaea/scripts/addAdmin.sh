#!/usr/bin/expect
#************arguments**************#
#argv 0 username
#argv 1 passward
#************arguments**************#

set user [lindex $argv 0]
set passward [lindex $argv 1]

spawn chroot /mnt/
expect "#"
send "adduser $user\n"
expect "UNIX"
send "$passward\n"
expect "UNIX"
send "$passward\n"
expect "Name"
send "\n"
expect "Room"
send "\n"
expect "Work"
send "\n"
expect "Home"
send "\n"
expect "Other"
send "\n"
expect "Y"
send "Y\n"
expect "#"
send "exit\n"
interact

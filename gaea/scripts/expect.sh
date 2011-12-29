#!/usr/bin/expect -f

set passward [lindex $argv 0]
spawn su
expect "Passward:"
send "$passward\r"
expect "#"
#send "ifconfig\r"
#expect "eth0"
interact

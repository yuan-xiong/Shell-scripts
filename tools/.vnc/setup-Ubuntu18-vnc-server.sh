#!/bin/bash

# https://www.cnblogs.com/ningmengcaokanyu/p/10410611.html


# install vnc4server
	sudo apt install -y xfce4 xfce4-goodies vnc4server
# passwd for vnc4server
	vnc4passwd
# start vnc4server
	#vnc4server :1
	vncserver :4 -geometry 1800x960 -alwaysshared -depth 24
# stop  vnc4server
	vnc4server -kill :1

# config for vnc4server
	vim ~/.vnc/xstartup

	#!/bin/bash
	unset SESSION_MANAGER
	unset DBUS_SESSION_BUS_ADDRESS
	startxfce4 &


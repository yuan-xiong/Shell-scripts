#!/usr/bin/bash

# ok
function setup-vnc-server-way2 {

	echo "https://www.cnblogs.com/luhouxiang/p/4829443.html"

	sudo systemctl daemon-reload

	vncserver -kill :1
	vncserver :1

	# 开机自启动
	sudo systemctl enable vncserver@:1.service
}


# failed
function setup-vnc-server-way1 {
	echo "https://www.linuxidc.com/Linux/2017-09/147050.htm"
	yum install -y tigervnc tigervnc-server tigervnc-server-module

	sudo cp /lib/systemd/system/vncserver@.service /lib/systemd/system/vncserver@:1.service

	sudo vim /lib/systemd/system/vncserver@:1.service
		# [Unit]
		# Description=Remote desktop service (VNC)
		# After=syslog.target network.target
		# 
		# [Service]
		# Type=forking
		# 
		# # Clean any existing files in /tmp/.X11-unix environment
		# ExecStartPre=/bin/sh -c '/usr/bin/vncserver -kill %i > /dev/null 2>&1 || :'
		# ExecStart=/usr/sbin/runuser -l root -c "/usr/bin/vncserver %i"
		# PIDFile=/home/root/.vnc/%H%i.pid
		# ExecStop=/bin/sh -c '/usr/bin/vncserver -kill %i > /dev/null 2>&1 || :'
		# 
		# [Install]
		# WantedBy=multi-user.target

	
	vncpasswd
	#123456

	sudo systemctl daemon-reload
	sudo systemctl stop vncserver@:1.service	# or vncserver -kill :1
	sudo systemctl start vncserver@:1.service	# or vncserver :1
	# sudo rm -rf /tmp/.X11-unix/*
	# 开机自启动
	sudo systemctl enable vncserver@:1.service
}

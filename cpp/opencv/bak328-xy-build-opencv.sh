#!/usr/bin/bash

function myecho {
	echo
	echo "[+] $1"
}

# start from here

	myecho "start to build OpenCV 3.3.1"

	# https://www.vultr.com/docs/how-to-install-opencv-on-centos-7
	yum install kernel-devel gcc gtk2-devel numpy pkconfig
	
	wget https://github.com/opencv/opencv/archive/3.3.1.zip
	mv 3.3.1.zip opencv-3.3.1.zip
	tar xzvf opencv-3.3.1.zip
	cd opencv-3.3.1
	
	mkdir build
	cd build
	cmake -D CMAKE_BUILD_TYPE=DEBUG -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_LAPACK=OFF ..
	make
	make install                  
	
	export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
	
	# test
	# git clone https://github.com/opencv/opencv_extra.git
	# export OPENCV_TEST_DATA_PATH=/root/opencv_extra/testdata


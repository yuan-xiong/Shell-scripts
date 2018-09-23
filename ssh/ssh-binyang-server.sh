#!/bin/bash

target=$1
#echo 'target: '$target




function who {
	case $USER in
		root)
			echo 'You are root'
			;;
		xy2)
			echo 'You are xy2'
			;;
		*)
			echo 'You are guest'
	esac
}

function login {
	case $target in
		xy4)
			echo 'Login xy4...'
			# xy4
			#ssh xy4@xy4-OptiPlex-9020
			ssh xy4@10.239.76.28
			;;
		go)
			echo 'Login go...'
			# go
			ssh binyang@aospdev-go
			#ssh binyang@10.239.93.169
			#ssh binyang@10.239.93.64
			;;
		hop)
			echo 'Login hop...'
			# hop
			ssh binyang@aospdev-hop
			#ssh binyang@10.239.93.163
			;;
		*)
			echo 'Login target UNKNOWN, failed'
	esac
}

echo
who
date

echo
login

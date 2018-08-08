#!/bin/bash

file=$1
time=$2
param=$3


date
echo "compile file: "${file}
echo

target=mytest
rm ${target}


# 1. compile
gcc -pg ${file} -o $target -lpthread
#g++ ${file} -o $target


# 2. run
#sudo mount --make-private /proc
#sudo mount --make-private /dev/sda1

#./umount-isos.sh
echo "run param: "${param}
echo
time sudo ./${target} ${time} ${param}

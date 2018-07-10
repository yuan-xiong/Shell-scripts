#!/bin/bash

file=$1
param=$2

date
echo "compile file: "${file}
echo

target=mount
rm ${target}


# 1. compile
gcc ${file} -o $target
#g++ ${file} -o $target


# 2. run
#sudo mount --make-private /proc
#sudo mount --make-private /dev/sda1

#./umount-isos.sh
echo "run param: "${param}
echo
time sudo ./${target} ${param}

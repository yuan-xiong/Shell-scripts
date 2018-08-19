#!/bin/sh
number=$1

file=${number}_containers.txt
echo "CPU Usage" > $file
sar -u 1 5 >> $file

echo "" >> $file

echo "Memory Usage" >> $file
sar -r 1 5 >> $file

echo "" >> $file

echo "Context Switch" >> $file
sar -w 1 5 >> $file

echo "" >> $file
echo "Run Queue and Load Average"
sar -q 1 5 >> $file



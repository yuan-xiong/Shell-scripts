#########################################################################
# File Name: parse.sh
# Author: ma6174
# mail: ma6174@163.com
# Created Time: 2018年07月09日 星期一 10时36分34秒
#########################################################################
#!/bin/bash

file=$1
echo 'parse file: '${file}
echo

# 5 000
#grep -nir 'time' ${file} | awk '$7>700{print $6" "$7}'

# 10 000
grep -nir 'time' ${file} | awk '$7>2370{print $7" "$8}'


# 5000 mount_umount
grep -nir "mount time" 5000_mount_umout.log | awk '$5>100{print $3" "$5}'
grep -nir "umount time" 5000_mount_umout.log | awk '$5>90{print $3" "$5}'


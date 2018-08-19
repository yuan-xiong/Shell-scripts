#!/bin/bash

mystart=$1
myend=$2
myinterval=$3
mytolerant=$4

number=0


#echo "Sleep for $myintervals for Instances $i"
echo "Create and start Android Instances from $mystart to $myend"


for (( i=$mystart; i< $myend; ++i )) ; do
	echo -e '\n-----------------------------------------------------'
	#date
	echo 'instance: '$i

	needstat=$((number%10))
	#echo '-->needstat: '$needstat

	if [[ $needstat = 0 ]];then
		echo '[sleeping...] 10s here'
		sleep 10
		#./mysar.sh-20180819-yuan $i
	fi

	echo
	date
	echo "Create Instances $i"
		./container_client create "v$i"
	
	echo "Start  Instances $i"
		./container_client start "v$i"
	
	#while [ $number -le $i ];do
	while [ $((number+mytolerant)) -le $i ];do
		#sleep 2
		sleep $myinterval

		#number=$(($number+1))
		number=`ps -A | grep tmgp | wc -l`
	done
	date

	#sleep another 2s for safty
	sleep 2
done

echo
echo "Finish to create and start Android Instances"


 




 

#!/bin/bash


echo "number of arguments is $#"
echo "Start create and start Android Instances $1"

for (( i=0; i< $1; ++i )) ; do
echo
date
    echo "Create Instances $i"
        ./container_client create "v$i"
    echo "Sleep for $2s for Instances $i"
       sleep $2

    #time=120
    #time=$[$[$[$i/10]*10]+10]
    #echo "Sleep for $time s for Instances $i"
       #sleep $time
    echo "Start Instances $i"
        ./container_client start "v$i"
done

echo "Finish to create and start Android Instances"


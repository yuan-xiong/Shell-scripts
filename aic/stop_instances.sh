#!/bin/bash

echo "number of arguments is $#"
echo "Start stop Android Instances $1"

for (( i=0 ; i< $1; ++i )) ; do
    echo "Sleep for $2s for Instances $i"
       sleep $2
    echo "Stop Instances $i"
        ./container_client stop "v$i"
done

echo "Finish to stop $1 Android Instances"


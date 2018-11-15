#!/bin/bash

target=uevent_dumper
myfile=myfile-aic-manager.txt


function myecho {

    echo
    echo -e " ---> ${1}"

}

function check-permission {

    case $USER in
        root)
            echo 'You are root, continue...'
            ;;
        *)
            echo "You are $USER, please sudo -s"
#            exit
    esac
}

    check-permission

    myecho 'check'
    ls -l /     | grep ${myfile}

    myecho 'clear'
    rm /${myfile}
    rm /dev/.coldboot_done
    rm ${target}

    myecho 'check'
    ls -l /     | grep ${myfile}

    myecho 'compile'
    gcc -W -o ${target} main.c uevent_dump.c 
    ls -l | grep -v "sh"

    myecho 'run'
    ./${target}
    
    myecho 'check'
    ls -l /     | grep ${myfile}

#!/bin/bash

source=$1
target=mknod
mydev=hidraw8
myfile=myfile.txt


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
            exit
    esac
}

    check-permission

    myecho 'check'
    ls -l /dev/ | grep ${mydev}
    ls -l /     | grep ${myfile}

    myecho 'clear'
    sudo rm /dev/${mydev}
    sudo rm /${myfile}
    sudo rm -fr uevent_dump/
    sudo rm /dev/.coldboot_done
    rm ${target}

    myecho 'check'
    ls -l /dev/ | grep ${mydev}
    ls -l /     | grep ${myfile}

    myecho 'compile'
    gcc -W -o ${target} ${source}
    ls -l | grep -v "sh"

    myecho 'run'
    sudo ./${target}
    
    myecho 'check'
    ls -l /dev/ | grep ${mydev}
    ls -l /     | grep ${myfile}

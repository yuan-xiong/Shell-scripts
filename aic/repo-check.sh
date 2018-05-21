#!/bin/bash
echo -e "\033[33m*****************prompt*******************\033[0m"
echo -e "\033[32musage: ./repo_check.sh\033[0m"
echo -e "\033[32mafter run the script, a user input with 'repo init' full command is required\033[0m"
echo -e "\n"
echo -e "\033[32mgreen color:  \033[0mchecking item"
echo "white color:  right results"
echo -e "\033[31mred   color:  \033[0mwrong results"
echo -e "\033[33m*****************prompt*******************\033[0m"
echo -e "\n"

current_dir=`pwd`
cd $current_dir
echo "please input the 'repo init -u xxx' command you are using:"
echo -e "\n"
read repo_cmd
echo -e "\n"
mkdir test_repo
cd test_repo
function code_init(){
    $repo_cmd >log.txt 2>&1
    repo sync a/aosp/platform/build >log1.txt 2>&1
}
function check_repo_version(){
repo_version=`repo --version`
arr=($repo_version)
echo ${arr[1]}|grep -q "from ssh://android.intel.com/admin/repo.git"
if [ $? -eq 0 ];then
    echo "you got the right repo!"
else
    echo -e "\033[31mplease refer to wiki 'https://wiki.ith.intel.com/display/CACTUS/2.1.1.2+-+Set+Local+Environment#id-2.1.1.2-SetLocalEnvironment-Installrepo'\033[0m"
fi
}

function check_manifest_branch_access(){

cat log.txt|grep -q "^error: revision .* not found$"
if [ $? -eq 0 ];then
    error=`cat log.txt|grep  "^error: revision .* not found$"`
    echo -e "\033[31myou may meet the issue: '$error',please contact cactus team: 'cactus-configmgt <cactus-configmgt@intel.com>' for this branch's access\033[0m"
else
    echo "issue not found!"
fi
}

function check_code_branch_access(){
cat log1.txt|grep -q "Missing access rights for a/aosp/platform/bionic"
if [ $? -eq 0 ];then
    error=`cat log1.txt|grep  "Missing access rights for a/aosp/platform/bionic"`
    echo -e "\033[31myou may meet the issue: '$error',please apply this branch's access, you can contact cactus team: 'cactus-configmgt <cactus-configmgt@intel.com>' for help\033[0m"
else
    echo "issue not found!"
fi
}

function check_mirror_access(){
cat log1.txt|grep -q "error: coercing to Unicode: need string or buffer, NoneType found"
fetch_url=`cat .repo/manifest.xml|grep "android-mirror"`
mirror_server=`echo $fetch_url|awk -F'"' '{print $2}'`
if [ $? -eq 0 ];then
    error=`cat log1.txt|grep  "error: coercing to Unicode: need string or buffer, NoneType found"`
    echo -e "\033[31myou may meet the issue: '$error',please check if your ~/.gitconfig file has the '$mirror_server' config\033[0m"
    return 10
else
    echo "issue not found!"
    return 2
fi
}


function check_repoconfig(){

cat log.txt|grep -q "gpg: Can't check signature: public key not found"
if [ $? -eq 0 ];then
    echo -e "\033[31myou may meet the issue: 'gpg: Can't check signature: public key not found',pleast run this command '"rm ~/.repoconfig"' to have a try\033[0m"
else
    echo "issue not found!"
fi
}

function check_gitconfig(){
cat ~/.gitconfig|grep -q 'review "https://android.intel.com"' && cat ~/.gitconfig|grep -q "username = .*"
if [ $? -eq 1 ];then
    echo -e "\033[31mplease check your '~/.gitconfig' if '[review "https://android.intel.com"]' and 'username' are included,you can refer to 'https://wiki.ith.intel.com/display/CACTUS/2.1.1.2+-+Set+Local+Environment#id-2.1.1.2-SetLocalEnvironment-Installrepo'\033[0m"
else
    if [ $res -eq 10 ];then
       echo -e "\033[31mplease add '$mirror_server' config in your ~/.gitconfig\033[0m"
    else
       echo "you got the right config in '~/.gitconfig'"
    fi
fi

}
function check_sshconfig(){
result=`cat ~/.ssh/config`
echo $result|grep -q "host android.intel.com port 29418 user .*"
if [ $? -eq 1 ];then
    echo -e "\033[31mplease check your '~/.ssh/config' if 'host android.intel.com port 29418 user .' are included,you can refer to 'https://wiki.ith.intel.com/display/CACTUS/2.1.1.2+-+Set+Local+Environment#id-2.1.1.2-SetLocalEnvironment-Installrepo'\033[0m"
 
else
    echo "you got the right config in '~/.ssh/config'"
   
fi
}
code_init
echo -e "\033[32m1. checking your repo version\033[0m"
IFS_old=$IFS
IFS=$'\n'
check_repo_version
IFS=$IFS_old
echo -e "\n"
echo -e "\033[32m2. checking your manifest branch access\033[0m"
check_manifest_branch_access
echo -e "\n"
echo -e "\033[32m3. checking your code branch access\033[0m"
check_code_branch_access
echo -e "\n"
echo -e "\033[32m4. checking your mirror server config\033[0m"
check_mirror_access
res=`echo $?`
echo -e "\n"
echo -e "\033[32m5. checking if you meet the problem with 'gpg: Can't check signature: public key not found'\033[0m"
check_repoconfig
echo -e "\n"
echo -e "\033[32m6. checking your '~/.gitconfig'\033[0m"
check_gitconfig
echo -e "\n"
echo -e "\033[32m7. checking your '~/.ssh/config'\033[0m"
check_sshconfig
echo -e "\n"
rm -rf ../test_repo

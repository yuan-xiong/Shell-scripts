

target=test

echo
echo -e "[reference]\thttps://blog.csdn.net/gujing001/article/details/6882363"


echo
echo -e "[compile]"
rm $target
g++ main.cpp mybacktrace.cpp -rdynamic -ldl -o $target

echo
echo -e "[go]"
./$target

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 

/** add for getprogname
* https://stackoverflow.com/questions/12439358/setprogname-in-linux
* sudo apt-get install libbsd-dev
* g++ main.cpp -lbsd
**/

#include <bsd/stdlib.h>	

#include <error.h>
#include <errno.h>


int main() {

	printf("----Main----\n");	
	if (fork() == 0) {
		setprogname("xy child");
		printf("Parent, pid: %d. Child, pid: %d, name :%s\n", getppid(), getpid(), getprogname());
	} else {
		setprogname("xy parent");
		printf("Parent, pid: %d, name: %s\n", getpid(), getprogname());
	}
	return 0;
}

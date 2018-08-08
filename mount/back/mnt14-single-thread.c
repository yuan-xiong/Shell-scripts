#define _GNU_SOURCE
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
 
#define STACK_SIZE (1024 * 1024)
 
static char child_stack[STACK_SIZE];
char* const child_args[] = {
  "/bin/bash",
  NULL
};

int default_cpu = 0;

long long get_time() {
	struct timeb t;
	ftime(&t);
	return 1000*t.time + t.millitm;
}

// set the CPU Affinity
void set_cpu_affinity()
{
	cpu_set_t cpu_mask;

	CPU_ZERO(&cpu_mask);
	// use CPU #0
	CPU_SET(default_cpu, &cpu_mask);

	// "0" stands for the process itself
	if ( sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask) == -1 )
	{
		printf("Warning: Cannot set CPU affinity, continuing...");
	}
}

// get the CPU affinity
void get_cpu_affinity()
{
	cpu_set_t cpu_get;

	CPU_ZERO(&cpu_get);
	// "0" stands for the process itself
	if ( sched_getaffinity(0, sizeof(cpu_get), &cpu_get) == -1 )
	{
		printf("Warning: Cannot get CPU affinity, continuing...");
	}

	// check the CPU affinity
	if ( CPU_ISSET(default_cpu, &cpu_get) )
	{
		printf("----> [%5d] CPU Affinity: %d\n", getpid(), default_cpu);
		//printf("----> [%5d] CPU Affinity: Process is running on processor: %d\n", getpid(), default_cpu);
	}
}

int child_main(void* arg)
{
	int idx = *((int *)arg);
	//printf("go child_main child %d\n", idx);
	char c;
	char dst[1024];
	
	long long start = get_time();
	printf("----> in child %d\n", idx);
	//printf("----> [%5d] in child %d\n", getpid(), idx);
	
	//get_cpu_affinity();

	int k;
	if ( (k = unshare(CLONE_NEWNS)) == -1 ) {
	  	printf("child %d unshare failed return %d\n", idx, k);
	}

	long long endus = get_time();
	printf("child %d unshare time: %lld ms\n", idx, endus-start);
	fflush(stdout);

	sprintf(dst, "/mnt/proc%d", idx);
	//printf("mkdir %s\n", dst);
	
	int err = 0;
	if ((err=mkdir(dst, 0755)) != 0) {
		perror("mkdir error");
		return -1;
	}

	int i;
	//if ( (i = mount("/dev/sda1", dst, "ext4", 0, 0)) != 0 ) {
	if ( (i = mount("proc", dst, "proc", 0, 0)) != 0 ) {
		printf("child %d mount failed return %d\n", idx, i);
	}

	long long endm = get_time();
	//printf("child %d mount time: %lld ms\n", idx, endm-start);
	printf("child %d mount time: %lld ms\n", idx, endm-endus);
	fflush(stdout);

//	while (1) {
//		//sleep(1);
//		//printf("child %d: i am alive!\n", idx);
//		//fflush(stdout);
//	}
	
	long long endum = get_time();
	//printf("child %d umount time: %lld ms\n", idx, endum-endm);
	printf("----> [%5d] child %d out, time: %lld ms\n", getpid(), idx, endum-start);
	return 1;
}
 
int umount_one(void* arg)
{
	int idx = *((int *)arg);
	//printf("go umount_one child %d\n", idx);
	
	char c;
	char dst[1024];
	sprintf(dst, "/mnt/proc%d", idx);
	//printf("start to umount %s\n", dst);
  
	long long start = get_time();

	int j;
	if ( (j = umount(dst)) != 0 ) {
		printf("child %d umount failed return %d\n", idx, j);
		return -1;
	}

	int err = 0;
	if ((err=rmdir(dst)) != 0) {
		perror("rmdir error");
		return -1;
	}

	long long end = get_time();
	printf("----> child %d umount %lld ms\n", idx, end-start);

	return 0;
}

void handle_child_pro(int child_idx, int not_umount)
{
		if (not_umount) {
			child_main((void*) (&child_idx));
		} else {
			umount_one((void*) (&child_idx));
		}
		return;
}


int main(int argc, char* argv[])
{
	long long start = get_time();
	
	int time;
	int child_idx;

	int times = atoi(argv[1]);
	
	for(time=0; time<times;++time) {

	  	child_idx = time;
		handle_child_pro(child_idx, (argv[2] == NULL));
	}
		
	long long end = get_time();
	printf("----> [%5d] parent time: %lld ms\n", getpid(), end-start);	
		
	return 0;
}

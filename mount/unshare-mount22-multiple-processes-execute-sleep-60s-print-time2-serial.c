#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)
 
static char child_stack[STACK_SIZE];
char* const child_args[] = {
  "/bin/bash",
  NULL
};

int default_cpu = 0;
int fds[2];

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
	char c;
	int idx = *((int *)arg);
	
	long long start = get_time();
	printf("child %d unshare at: %lld\n", idx, start);
	printf("%d\t\t", idx);
	//printf("----> in child %d\n", idx);


	// 1. unshare
	int k;
	if ( (k = unshare(CLONE_NEWNS)) == -1 ) {
	  	printf("child %d unshare failed return %d\n", idx, k);
	}

	long long endus = get_time();
	printf("%lld\t\t", endus-start);
	//printf("child %d unshare time: %lld ms\n", idx, endus-start);
	fflush(stdout);


	// 2. mount
	char dst[1024];
	sprintf(dst, "/mnt/proc%d", idx);
	
	// mkdir for mount destination
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
	printf("%lld\n", endm-endus);
	//printf("child %d mount time: %lld ms\n", idx, endm-endus);
	printf("child %d mount at: %lld\n", idx, endm);
	fflush(stdout);


	// 3. send message to parent finished unshare & mount
	close(fds[0]);
	char child_msg[1024];
	sprintf(child_msg, "child %d finished", idx);
	//printf("send message: %s\n", child_msg);
	write(fds[1], child_msg, strlen(child_msg) + 1);
	close(fds[1]);

	while (1) {
		sleep(60);
	}
	
	long long endum = get_time();
	//printf("----> [%5d] child %d out, time: %lld ms\n", getpid(), idx, endum-start);
	return 1;
}
 
int umount_one(void* arg)
{
	char c;
	char dst[1024];
	int idx = *((int *)arg);

	long long start = get_time();

	// umount
	sprintf(dst, "/mnt/proc%d", idx);

	int j;
	if ( (j = umount(dst)) != 0 ) {
		printf("child %d umount failed return %d\n", idx, j);
	}

	long long end = get_time();
	printf("----> child %d umount %lld ms\n", idx, end-start);
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

	int time;
	int child_idx;
	pid_t fpid;
	
	long long start = get_time();
	signal(SIGCHLD, SIG_IGN);

	// times: fork children number
	int times = atoi(argv[1]);
	printf("child\t\tunshare\t\tmount\t\ttime(ms)\n");
	

	// fork children
	for(time=0; time<times;++time) {

		// create pipe
		if (pipe(fds) < 0)
		{
			printf("create pipe failed");
			return -1;
		}

		fpid = fork();
	  	if (fpid <= 0) {		// in child

	  	    child_idx = time;
			handle_child_pro(child_idx, (argv[2] == NULL));
	  	    break;
	  	} else {				// in parent

			// wait for child
			close(fds[1]);
			char buf[1024];
			read(fds[0], buf, sizeof(buf));
			//printf("Get msg %d: %s\n", time, buf);
			close(fds[0]);
	  	}
	}
	

	long long end = get_time();
	printf("----> [%5d] parent time: %lld ms\n", getpid(), end-start);	
	
	printf("wait for all children...");
	wait(NULL);
	return 0;
}

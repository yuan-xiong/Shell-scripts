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
 
#define STACK_SIZE (1024 * 1024)
 
static char child_stack[STACK_SIZE];
char* const child_args[] = {
  "/bin/bash",
  NULL
};

// recursive times in child_main
int num=1;
int times=1500;
//static volatile int cnt=0;

long long get_time() {
	struct timeb t;
	ftime(&t);
	return 1000*t.time + t.millitm;
}

int child_main(void* arg)
{
  int idx = *((int *)arg);
  char c;
  char dst[1024];
  
  long long start = get_time();
  printf("----> [%5d] in child %d\n", getpid(), idx);

  int k;
  if ( (k = unshare(CLONE_NEWNS)) == -1 ) {
		printf("child %d unshare failed return %d\n", idx, k);
  }
  long long endus = get_time();
  printf("child %d unshare time: %lld ms\n", idx, endus-start);


  int n;
  for (n=0; n<num; ++n) {
	//sprintf(dst, "/mnt/iso%d", n);
	sprintf(dst, "/mnt/iso%d", idx);
	//printf("mkdir %s\n", dst);
	
	int err = 0;
	if ((err=mkdir(dst, 0755)) != 0) {
		perror("mkdir error");
		return -1;
	}

    int i;
	if ( (i = mount("/dev/sda1", dst, "ext4", 0, 0)) != 0 ) {
		printf("child %d mount failed return %d\n", idx, i);
	}
  }

  long long endm = get_time();
  printf("child %d mount time: %lld ms\n", idx, endm-endus);

  int j;
  if ( (j = umount(dst)) != 0 ) {
		printf("child %d umount failed return %d\n", idx, j);
  }
  long long endum = get_time();
  printf("child %d umount time: %lld ms\n", idx, endum-endm);
  

  printf("----> [%5d] child %d out, time: %lld ms\n", getpid(), idx, endum-start);
  return 1;
}
 
int main()
{

	signal(SIGCHLD, SIG_IGN);
	long long start = get_time();
	
	int time;
	pid_t fpid;
	for(time=0; time<times;++time) {

	  fpid = fork();

	  if (fpid < 0) {
		printf("error in fork!");
	  } else if (fpid == 0) {
		//cnt++;
		//printf("child %d\n", cnt);
		int child_idx = time;
		child_main((void*) (&child_idx));
		//printf("child %d\n", child_idx);
		return 0;
	  } else {
		//printf("----> [%5d] in parent\n", getpid());	
		//waitpid(fpid, NULL, 0);
	  }
	}

	printf("----> last fpid %d\n", fpid);	
	//sleep(4);
	wait(NULL);

	long long end = get_time();
	printf("----> [%5d] parent out, time: %lld ms\n", getpid(), end-start);	
	return 0;
}

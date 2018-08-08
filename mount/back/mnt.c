#define _GNU_SOURCE
#include <sys/mount.h>
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
int checkpoint[2];


int child_main(void* arg)
{
  char c;

  close(checkpoint[1]);
  read(checkpoint[0], &c, 1);

  printf("----> [%5d] in child\n", getpid());
  sethostname("In Namespace", 12);

  int i = mount("/dev/sda1", "/mnt/iso", "ext4", 0, 0);
  //mount("/dev/sda1", "/mnt/iso", "ext4", NULL, NULL);

  //mount("/mnt/runtime/default", "/storage", NULL, MS_BIND | MS_REC | MS_SLAVE, NULL);

  
  execv(child_args[0], child_args);
  printf("Ooops\n");
  return 1;
}
 
int main()
{
  pipe(checkpoint);

  printf("----> [%5d] in parent\n", getpid());
  int child_pid = clone(child_main, child_stack+STACK_SIZE, CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);

  close(checkpoint[1]);

  waitpid(child_pid, NULL, 0);
  return 0;
}

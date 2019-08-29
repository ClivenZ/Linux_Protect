#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
  pid_t pid;

  printf("Before:pid is %d\n",getpid());
  if((pid = fork()) == -1)
    perror("fork()");
  printf("After:pid is %d, fork return %d\n",getpid(),pid);
  
  return 0;
}

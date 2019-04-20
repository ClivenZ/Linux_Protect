#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(){
  char* fifo = "./test.fifo";
  umask(0);
  int ret = mkfifo(fifo,0664);
  if(ret < 0 && error != EEXIST){
    perror("mkfifo error");
    return -1;
  }
  int fd = open(fifo,O_RDONLY);

  

  return 0;
}

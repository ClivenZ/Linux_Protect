#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

int main(){
  close(1);
  int fd = open("myflie",O_WRONLY | O_CREAT,00644);
  if(fd < 0){
    perror("open");
    return 1;
  }
  printf("fd: %d\n",fd);
  fflush(stdout);
  close(fd);
  return 0;
}

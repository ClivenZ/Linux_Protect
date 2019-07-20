#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<error.h>
int main(){
  pid_t rem = fork();
  //fork函数，通过复制调用进程（父进程）创建子进程
  //创建成功：父进程返回子进程的PID，子进程返回0；
  //创建失败：返回<0的结果；
  //printf("PID=%d PPID=%d rem=%d\n",getpid(),getppid(),rem);
  if(rem > 0 ){
    printf("F:%d\n",getpid());
    while(1){
      sleep(1);
    }
  }else if(rem == 0){
    printf("C:%d\n",getpid());
    int count = 5;
    while(count > 0){
      sleep(1);
      --count;
    }
  }else{
    perror("fork");
  }
  sleep(1);
  return 0;
}

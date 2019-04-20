#include<stdio.h>
#include<stdlib.h>
#include<error.h>
#include<unistd.h>
#include<sys/wait.h>
//使用匿名管道实现管道符作用
//连接两个命令，将前面命令的输出结果，交给后面的命令处理
int main(){
  int pipefd[2];

  if(pipe(pipefd) < 0){
    perror("pipe error");
    return -1;
  }
  //ls | grep make
  int pid1 = fork();
  if(pid1 == 0){
    //执行 ls - 将目录下的文件信息打印到标准输出
    close(pipefd[0]);
    dup2(pipefd[1],1);
    execlp("ls","ls",NULL);
  }
  int pid2 =fork();
  if(pid2 == 0){
    //执行grep make 程序
    //循环从标准输入读取数据进行字符串匹配
    close(pipefd[1]);
    dup2(pipefd[0],0);
    execlp("grep","grep","make",NULL);
  }
  close(pipefd[0]);
  close(pipefd[1]);
  wait(NULL);
  wait(NULL);
  return 0;
}

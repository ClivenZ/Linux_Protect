/*自主minishell 实现
 *  1、获取标准输入
 *  2、解析输入得到[命令名]+[运行参数]
 *  3、创建子进程
 *    子进程中进行程序替换
 *  4、进程等待
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<strings.h>

int main(){
  while(1){
    char buf[1024] = {0};
    printf("[user@localhost]$ ");
    fflush(stdout);
    //正则表达式
    //%[^\n] 从缓冲区取数据的时候，遇到\n为止
    //进过这一步之后，缓冲区中留下了\n，但是取不出来
    //导致scnaf非阻塞，但又取不出来\n字符
    //%*c 从缓冲区当中去取出一个字符（丢弃）
    if( scanf("%[^\n]%*c",buf) != 1){
      getchar();
      continue;
    }
   // printf("buf:[%s]\n",buf);
    int argc = 0;
    char* argv[32];
    char* ptr = buf;
    while(*ptr != '\0'){
     //将指针走到非空白字符处
      if(!isspace(*ptr)){
        argv[argc] = ptr;
        printf("argv[%d]=[%s]\n",argc,argv[argc]);
        argc++;
        //将ls走完，认为1s是一个完整的字符串
        //不能仅仅将l字符串作为第0个参数
        while(!isspace(*ptr) && *ptr != '\0'){
          ptr++;
        }
        *ptr = '\0';
      }
      ptr++;
    }
    //
    argv[argc] = NULL;
    //实现shell 内建命令
    if(strcmp(argv[0],"cd") == 0){
     //改变当前工作路径
     //int chdir(const char* path)
      chdir(argv[1]);
      continue;
    }
    int pid = fork();
    if(pid < 0){
      perror("fork error");
      return -1;
    }else if(pid == 0){
      execvp(argv[0],argv);
      exit(-1);
    }
    wait(NULL);
  }
  return 0;
}

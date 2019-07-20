#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>


void *test(void* arg){
  int i;
  for(;;){
    printf("This is the NO.1 prthead.\n");
    sleep(1);
  }
}

int main(){
  pthread_t tid;
  int ret;
  ret = pthread_create(&tid,NULL,test,NULL);
  if(ret == 0){
    printf("Create thread;\n");
    sleep(1);
  }
  int i,j;
  for(;;){
  j = pthread_self();
  printf("The pthread ID:%d\n",j);
  sleep(1);
  }
}

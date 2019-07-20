#include<pthread.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


void* rout(void* arg){
  int i;
  for(;;){
    printf("pthread1\n");
    sleep(1);
  }
}

int main(){
  pthread_t tid;
  int ret;
  if((ret = pthread_create(&tid,NULL,rout,NULL)) != 0) {
    fprintf(stderr,"pthread_create:%s\n",strerror(ret));
    exit(EXIT_FAILURE);
  }
  int i;
  for(;;){
    printf("Main pthread\n");
    sleep(1);
  }
}

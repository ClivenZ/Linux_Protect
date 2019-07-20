#include<pthread.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


void *rout(void* arg){
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
//#include<stdio.h>
//#include<unistd.h>
//#include<stdlib.h>
//#include<pthread.h>
//#include<string.h>
//
//void *test(void *arg){
//  int i = 1;
//  while(i == 1){
//
//  printf("Pthraed1\n");
//  sleep(1);
//  }
//}
//
//int main(void){
//  pthread_t tid;
//  int ret;
//  ret = pthread_create(&tid,NULL,test,NULL);
//  if(ret == 0){
//    printf("Create a new pthread");
//    sleep(1);
//  }else{
//    printf("Create error:%s\n",ret);
//    sleep(1);
//  }
//}
//
//

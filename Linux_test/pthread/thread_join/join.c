#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


void* thread1(void* arg){
  printf("thread1 returning...\n");
  int* p = (int*)malloc(sizeof(int));
  *p = 1;
  return (void*)p;
}
void* thread2(void* arg){
  printf("thread2 exiting...\n");
  int* p = (int*)malloc(sizeof(int));
  *p = 2;
  pthread_exit((void*)p);
}
void* thread3(void* arg){
  while(1){
    printf("thread3 is running...\n");
    sleep(1);
  }
}

int main(){
  pthread_t tid;
  void* ret;


  //thread1
  pthread_create(&tid,NULL,thread1,NULL);
  pthread_join(tid,&ret);
  printf("thread return,thread ID %x,return code:%d\n",tid,*(int*)ret);
  free(ret);
  //thread2
  pthread_create(&tid,NULL,thread2,NULL);
  pthread_join(tid,&ret);
  printf("thread return,thread ID %x,return code:%d\n",tid,*(int*)ret);
  //thread3
  pthread_create(&tid,NULL,thread3,NULL);
  sleep(3);
  pthread_cancel(tid);
  pthread_join(tid,&ret);
  if(ret == PTHREAD_CANCELED){
    printf("thread return,thread ID %x,return code:PTHREAD_CANCELED\n",tid);
  }else{
    printf("thread return,thread ID %x,return code:NULL\n",tid);
  }
}

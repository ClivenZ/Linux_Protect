#define RED "\033[31m"
#define WHITE "\033[37m"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main(){
  char arr[102] = {0};
  int i = 0;
  memset(arr,0,sizeof(arr));
  const char* lable = "|/-\\";
  for(i = 0;i <= 100;i++){
    arr[i] = '#';
    printf(RED);
    printf("[%s]%s[%d][%c]\r",arr,"waiting...",i,lable[i%4]);
    printf(WHITE);
    fflush(stdout);
    usleep(100000);
  }
  printf("\n");
  return 0;
}

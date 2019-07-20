#include<stdio.h>
#include<unistd.h>

int main(){
  printf("----head----\n");
  int num = fork();
  printf("%d\n",num);
  printf("----tail----\n");
  printf("%d\n",num);
  return 0;
}

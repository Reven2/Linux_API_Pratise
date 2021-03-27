#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//truncate(const char *path,off_t length)  拓展文件长度

int main(int argc,char*argv[]){

  int ret =  truncate("open.c",100);

  if(ret ==0 ){
    printf("file truncate success!\n");}

return 0;
}

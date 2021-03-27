#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
void sys_err(const char *str){  perror(str); exit(1);}

int main(int argc,char *argv[]){ 
    
    char *p = NULL ;
    int fd;

    fd = open("test_map",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd==-1) sys_err("open err\n");
/*
 *  extend file size
    lseek(fd,10,SEEK_END);
    write(fd,"\0",1);
  */  
    ftruncate(fd,10);       //需要写权限才可以拓展文件大小
    
    int len = lseek(fd,0,SEEK_END);  //get file size

    p =mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p == MAP_FAILED) sys_err("mmap err\n");
    
    //使用 p pointer 对文件读写
    strcpy(p,"hello mmap\n");
    printf("----%s",p);
    int ret = munmap(p,len); if(ret == -1) sys_err("munmap err\n");


    return 0;}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/mman.h>

void sys_err(const char *str){  perror(str); exit(1);}

struct student{
    int id ;
    char name[256];
    int age;
};

int main(int argc,char *argv[]){
    
    struct student stu = {1,"reven",22};
    struct student *p;
    int fd ;
    fd =  open("test_map",O_RDWR|O_CREAT|O_TRUNC,0664);
    if(fd==-1) sys_err("open err\n");

    ftruncate(fd,sizeof(stu));   //extend  file size
    
    p = mmap(NULL,sizeof(stu),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(p == MAP_FAILED) sys_err("map err\n");

    close(fd);

    while(1){
        memcpy(p,&stu,sizeof(stu));  //循环写每一次都会覆盖上次写的
        stu.id++;
        sleep(1);
        
    }

    munmap(p,sizeof(stu));

    
    return 0;}

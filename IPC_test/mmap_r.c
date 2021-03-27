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
   struct student stu; 
    struct student *p;
    int fd ;
    fd =  open("test_map",O_RDONLY);
    if(fd==-1) sys_err("open err\n");

    
    p = mmap(NULL,sizeof(stu),PROT_READ,MAP_SHARED,fd,0);
    if(p == MAP_FAILED) sys_err("map err\n");

    close(fd);

    while(1){
        printf("id = %d, name=%s,age=%d\n",p->id ,p->name,p->age);
        sleep(1);
        
    }

    munmap(p,sizeof(stu));

    
    return 0;}

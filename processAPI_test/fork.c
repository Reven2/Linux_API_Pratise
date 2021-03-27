#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

int main(int argc,char*argv[]){

    printf("before fork-1---\n");
    printf("before fork-2---\n");
    printf("before fork-3---\n");
    printf("before fork-4---\n");

    pid_t pid = fork();
    if(pid == 0){
    printf("there is child ! child ID is %d  my parent ID is %d \n",getpid(),getppid());
    }else if(pid > 0){
        printf("there is parent，parent ID is %d !\n",getpid());
    }else{
        perror("something error!\n");
    }

    printf("end of process!\n");
return 0;
}

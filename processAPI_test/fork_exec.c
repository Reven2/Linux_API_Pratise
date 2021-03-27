#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

int main(int argc,char*argv[]){


    pid_t pid = fork();
    if(pid == 0){
        //exelcp("a,out","argv[1]","argv[2]...,NULL")
        execlp("ls","ls","-l","-d","-h",NULL);      //argc[0]是可执行程序本身
    
    }else if(pid > 0){
        sleep(1);
        printf("I am parent\n");
    
    }else{
        perror("fork error!\n");
    
    }



    return 0;
}

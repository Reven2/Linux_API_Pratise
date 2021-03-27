#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
void sys_err(const char * str){

    perror(str);
    exit(1);
}




int main(int argc,char *argv[]){ 
    
    int fd[2];
    int ret ;
    pid_t pid ;
    int i ;

    ret = pipe(fd);
    if(ret == -1) sys_err("pipe error!\n");
   
    for( i =0;i<2;i++){
        pid = fork();
        if(pid == -1)
            sys_err("fork error!\n");   
        else if(pid == 0) break;
    }
        if(i==2){  //父进程回收两个子进程
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
        }
        
        
        else if(i==0){  //哥哥进程
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO); //wc-l原来是从STDIN_FILENO读的，现在要把从pipe读
        execlp("wc","wc","-l",NULL);
        sys_err("execlp error!\n");
        }
        

        else if(i==1){ //弟弟进程
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO); //屏幕输出复制到fd[1]写端
        execlp("ls","ls",NULL); //ls 原来是写到STDOUT_FILENO，我们需要写到pipe
        sys_err("execlp error!\n");
            
        }


    return 0;}

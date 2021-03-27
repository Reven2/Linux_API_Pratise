#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc ,char *argv[]){
    
    pid_t pid = fork();
    int status ;    

    if(pid == 0){
        printf("----child, my ID = %d,I going to sleep 5S\n",getpid());
        sleep(5);
        printf("------child is dead ------\n");
        return 73;
    }else if(pid>0){
        //pid_t wait(int *wstatus); status是一个传出参数，返回回收子进程的pid
       // int wwpid = wait(NULL);传参数NULL 不关心子进程如何死亡
        int wpid = wait(&status); //此函数会一直阻塞，知道子进程死亡了，执行回收，然后父进程结束
        if(wpid ==-1 ){
            perror("wait error\n");
            exit(1);
        }
        //子进程正常终止
        if(WIFEXITED(status)){
            printf("child exit with %d\n",WEXITSTATUS(status));
        }
        //子进程被信号终止
        if(WIFSIGNALED(status)){
            
            printf("child kill with %d\n",WTERMSIG(status));
        }



        printf("parent wait finish ,recycling pid  %d\n",wpid);
    
    }

return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc ,char*argv[]){

    int i;
    pid_t pid, wpid;

    for(int i=0;i<5;i++){
        pid =fork();
        if(pid == 0){
            break;
        }
    
        if(i==5){           //父进程
        //     while((wpid = waitpid(-1,NULL,0))){  //阻塞回收子进程
          //      printf("wait child %d\n",wpid);
            
           // }
           while((wpid = waitpid(-1,NULL,WNOHAND))!=-1){    //非阻塞回收子进程 -1参数表示等待任意子进程
                if(wpid >0){                                //WNOHAND表示非阻塞，NULL表示不关心如何死亡
                    printf("wait child %d\n",wpid);
                }else if(wpid == 0){  //返回值为0参数三WNOHAND表示进程正常运行
                    sleep(1);
                    continue;
                }
           }
        }


    }



return 0;
}

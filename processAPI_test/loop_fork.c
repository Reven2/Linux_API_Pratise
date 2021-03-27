#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

int main(int argc ,char* argv[]){


    pid_t pid ;
    int i ;

    for(i=0;i<5;i++){
        pid = fork();
        if(pid == 0){
            printf("this is child %d , ID is %d \n",i,getpid());
            break;
        }
        else if( pid == -1) {
            perror("fork error!\n");
        }
    }

        if (i == 5){
            sleep(1);
            printf("I am parent ,ID is %d \n",getpid());
        }

return 0;
}

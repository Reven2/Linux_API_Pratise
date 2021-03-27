#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void sys_err(char * str){

    perror(str);
    exit(1);
}


int main(int argc,char*argv[]){

    
    pid_t pid ;
    int ret ;
    int fd[2];

    char *str = "hello pipe!\n";
    char buf[4096];

    ret = pipe(fd);     //创建并打开管道，无写端返回0，管道已满阻塞等待被读
    if (ret == -1) sys_err("pipe error\n");

    pid = fork();
    if(pid > 0){
        close(fd[0]);   //父进程用于写入，关闭读端
        write(fd[1],str,strlen(str)); //str是要写入数据的地址，返回写入多少bytes
        sleep(1);   //父进程后结束
        close(fd[1]);
    }
    else if(pid == 0){
        close(fd[1]);
        ret = read(fd[0],buf,sizeof(buf)); //返回实际读到bytes，数据缓存在buf
        write(STDOUT_FILENO,buf,ret);
        close(fd[0]);
    }


return 0;
}

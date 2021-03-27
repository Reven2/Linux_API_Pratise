#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>


//如果一个一个字节读 read() write()无缓冲IO 的速度不如 C语言库中的 fget() 和 fput()
//因为其实 fget()是一次4096个字节拷贝

int main(int argc ,char *argv[]){

    int fd1 = open(argv[1],O_RDONLY);  //read from fd
    if(fd1==-1){
        perror("open error\0");
        exit(1);
    }
    
    int fd2 = open(argv[2],O_CREAT|O_RDWR|O_TRUNC,0664);
    if(fd2==-1){
        perror("open error\0");
        exit(1);
    }

    char buf[1024];            
    int numRead = 0;
    int numWrite = 0;
  //最多一次只能读1024个字节所以循环读返回的n是读了多少bytes
    while((numRead = read(fd1,buf,1024))!= 0){
        if(numRead<0){
            perror("read error\0");
            exit(1);
        }
        numWrite = write(fd2,buf,numRead);
        if(numWrite<0){
            perror("write error\0");
            exit(1);
        }
        numWrite = write(fd2,buf,numRead);
    }
    
    printf("writen in %d bytes\n",numWrite);
    close(fd1);
    close(fd2);
    return 0;

}

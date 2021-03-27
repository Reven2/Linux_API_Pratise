#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>


//int stat(const char *pathname, struct stat *statbuf); statbuf 是传出参数执行文件属性结构体

int main(int argc,char*argv[]){

    struct stat sbuf;

    int ret = stat(argv[1],&sbuf);
    if(ret ==- 1){
        perror("gain status of file is failure!");
        exit(1);
    }
    
printf("file size is %ld\n",sbuf.st_size);
printf("file inode is %ld\n",sbuf.st_ino);
printf("file uid is %d\n",sbuf.st_uid);
    
    if(S_ISDIR(sbuf.st_mode)){
        
        printf("this is a directory\n");
    }
    
        printf("this is not a directory\n");

return 0;
}

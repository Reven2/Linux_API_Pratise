#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>


int main(int argc ,char*argv[]){

    link(argv[1],argv[2]); //创建硬链接

    unlink(argv[1]);    //删除文件


    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc ,char * argv[]){

    int n = 0;
    int s_offset;
    int fd = open(argv[1],O_RDWR);
    if(fd==-1){
        perror("open error\n");
        exit(1);
    }
    s_offset = lseek(fd,24,SEEK_SET);  //从文件开头偏移offset个字节数

    //soffset = lseek(fd,0,SEEK_END) 这样可以得到文件大小，
    //soffset = lseek(fd,100,SEEK_END)如果引起IO操作，
    //这样就可以将文件大小拓展100bytes，比如写点东西进去
    //write(fd,"something write in",100) //记得第二个参数是缓冲区内存地址，第三个参数是数据大小
    printf("now the byte index is in %d",s_offset);

    close(fd);


    return 0;
}

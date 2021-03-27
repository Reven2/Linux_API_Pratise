#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
int main(int argc ,char *argv[]){
    
    int fd = open(argv[1],O_RDWR);

    int newfd = dup(fd);

    write("newfd = %d\n",newfd);

return 0;
}

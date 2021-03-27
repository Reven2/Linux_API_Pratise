#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>

//实现ls

int main(int argc,char*argv[]){
    
    DIR *dp;
    struct dirent *sdp;

    dp = opendir(argv[1]);
    if(dp==NULL){
        perror("open directory error!\n");
        exit(1);
    }

    while((sdp = readdir(dp))!=NULL){    //读目录  ，sdp return struct dirent,d_name[256]
                                         //这就是为什么文件名最长只有255bytes
        
        printf("%s\t",sdp->d_name);
    }
    
    printf("\n");

closedir(dp);
return 0;
}

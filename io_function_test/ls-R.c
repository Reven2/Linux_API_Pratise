#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>


void isFile(char *name);


void read_dir(char *dir){
    
    char path[257];         //文件名称最长为256字节，但sprintf要求多大一个 不然会报警告
    struct dirent *sdp;
    DIR *dp =opendir(dir); //打开目录
    if(dp ==NULL){
        perror("opendir error");
        return;
    }
    //读取目录项
    while((sdp = readdir(dp))!= NULL){  //读目录
        if(strcmp(sdp->d_name,".") == 0  || strcmp(sdp->d_name,"..") ==0){
            continue;
        } 
        // 目录/目录项 以这样的形式拼接成新的目录
        sprintf(path,"%s/%s",dir,sdp->d_name);  //拿到当前读到的d_name拼接当前目录
        isFile(path);           //传入路径判断是否为文件
    
    }

    closedir(dp);
    return;
}




void isFile(char *name){
        int ret =0;

        struct stat sb;
        ret = stat(name,&sb);   //sb是一个传出参数 传出一个stat struct 拿来判断文件类型
        if(ret == -1){
            perror("stat error\n");
                return;
        }

        if(S_ISDIR(sb.st_mode)){  //如果是目录则进入读目录函数
           read_dir(name); 
        }

        printf("%s\t%ld\n",name,sb.st_size); //如果是文件打印出大小和文件名

        return;
}





int main(int argc ,char *argv[]){

    if(argc == 1){
        isFile("."); //默认当前路径
    }else{          
        isFile(argv[1]);  //可能给的是一个普通文件
    }

return 0;
}

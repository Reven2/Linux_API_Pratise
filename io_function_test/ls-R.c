#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>


void isFile(char *name);


//void read_dir(char *dir,void (*fcn)(char *path))

void read_dir(char *dir){
    
    char path[257];         //文件名称最长为256字节，但sprintf要求多大一个 不然会报警告
    struct dirent *sdp;
    DIR *dp =opendir(dir); //打开目录
    if(dp ==NULL){
        perror("opendir error");
        return;
    }
    //读取目录项 he
    while((sdp = readdir(dp))!= NULL){  //读目录

		//很多条件判断 例如文件名/目录名最长 256 Byte,这里就不写了
		//以下判断防止无线递归 因为每个目录下都有. ..目录
        if(strcmp(sdp->d_name,".") == 0  || strcmp(sdp->d_name,"..") ==0){
            continue;
        } 
		
        //int sprintf(char *buffer, const char *format, ...)
		//Writes the results to a character string buffer
        sprintf(path,"%s/%s",dir,sdp->d_name);  //拿到当前读到的d_name拼接当前目录
        isFile(path);           //传入路径判断是否为文件
    	//(*fcn)(path);		//回调函数写法
    }

    closedir(dp);
    return;
}


void isFile(char *name){
        int ret =0;

        struct stat sb;

        //int stat(const char *pathname, struct stat *statbuf);Q
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

	//argc 表示接受的命令参数个数,==1只有一个执行参数./ls-R 
	//没有无后续文件名/目录名参数，故默认当前路径
    //argv 表示传入的命令内容 如./ls-R , dirName , fileName
	if(argc == 1){
        isFile("."); //默认当前路径
    }else{          
        isFile(argv[1]);  //可能给的是一个普通文件
		//while(--argc > 0) //可以一次查询多个目录
		//isFile(*++argv);
    }

return 0;
}

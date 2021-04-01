#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SERVER_PORT 9527

void sys_err(const char* str){
		perror(str);
			exit(1);
}
int main(int argc,char *argv[]){ 
	
	char buf[BUFSIZ];
	int count = 5;
	int cfd ;	//此socket就是用来通信的，不想服务器端要accept返回新创建一个拿来通信
	struct sockaddr_in server_addr; //服务器地址接哦古
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr.s_addr);
	
	cfd = socket(AF_INET,SOCK_STREAM,0);
	if(cfd == -1) sys_err("socket err\n");
	
	int ret = connect(cfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret == -1) sys_err("connect err\n");
	
	while(--count){   
		write(cfd,"hello",5);    	//把数据写到 cfd

		//从cfd读数据到缓冲区然后写到标准输出 方便观察
		read(cfd,buf,sizeof(buf));		
		write(STDOUT_FILENO,buf,ret);
	
	}

	close(cfd);
	
	return 0;}


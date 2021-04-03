#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<strings.h>
#include<poll.h>
#include<pthread.h>

#define SERV_POTR 9999

void recycl_child(int sig){
	while(waitpid(0,NULL,WNOHANG)>0)
		return ;
}





//为了突出逻辑，系统调用函数很多都没检查返回值！
int main(int argc ,char **argv){
	int lfd,cfd,ret,i;
	pid_t pid;
	char buf[BUFSIZ];
	socklen_t client_addr_len;

	struct sockaddr_in serv_addr,client_addr;
	bzero(&serv_addr,sizeof(serv_addr));		//make adress be 0
	
	serv_addr.sin_port = htons(SERV_POTR);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	lfd = socket(AF_INET,SOCK_STREAM,0);

	bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	listen(lfd,128);

	client_addr_len = sizeof(client_addr);

	while(1){
		if(cfd = accept(lfd,(struct sockaddr*)&client_addr,&client_addr_len));
			pid = fork();
			if(pid<0) perror("fork err\n");
			else if(pid==0){
				close(lfd);   //只需要父进程的lfd当作监听队列
				break;
			}else{			//这是父进程 要回收子进程
				
				struct sigaction act;
				act.sa_handler = recycl_child;
				sigemptyset(&act.sa_mask);
				act.sa_flags = 0;

				sigaction(SIGCHLD,&act,NULL);  //register sigaction

				close(cfd);
				continue;
			}
	}

	if(pid==0){
		while(1){		 	//注意次循环是为了一个客户端可以写多次信息
		ret = read(cfd,buf,sizeof(buf));
		for(i=0;i<ret;i++)
			buf[i] = toupper(buf[i]);

		write(cfd,buf,ret);
		write(STDOUT_FILENO,buf,ret);
		}	
	}


	return 0;
}

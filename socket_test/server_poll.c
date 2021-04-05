#include"wrap.h"
#include<poll.h>
#define SERVER_PORT 9999
#define OPEN_MAX 1024

int main(int argc,char *argv[]){ 

	char str[80];
	int lfd,cfd,sockfd;
	int i,j,maxi,nready;
	ssize_t n;
	char buf[BUFSIZ];     //BUFSIZ 	4096 bytes


	struct pollfd client[OPEN_MAX];


	struct sockaddr_in server_addr,client_addr;	//定义结构体
	//初始化
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY 表示取的一个可用的主机地址
	server_addr.sin_port = htons(SERVER_PORT);	//注意port是 short,否则客户端将连不上！！
	
	lfd = Socket(AF_INET,SOCK_STREAM,0);

	//设置端口可重用, 还是会等待2MSL 
	int opt =1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt,sizeof(opt));

	Bind(lfd,(struct sockaddr *)&server_addr,sizeof(server_addr)); //绑定addr，socket，addrlen

	Listen(lfd,128);	//socket, backlog(max connections) 
	
	socklen_t client_addr_len;			//客户端地址长度
	client_addr_len = sizeof(client_addr);
	

	client[0].fd = lfd;	//监听的第一个
	client[0].events = POLLIN;//lfd监听普通读事件

	//初始化
	for(i=1;i<OPEN_MAX;i++){
		client[i].fd = -1;
	}
	
	maxi = 0;	//有效元素最大数组下标

	while(1){
		nready = poll(client,maxi+1,-1);//阻塞监听链接请求
		
		if(client[0].revents & POLLIN){
			cfd = Accept(lfd,(struct sockaddr*)&client_addr,&client_addr_len);
			printf("received from %s at PORT %d\n",
	inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
													
	ntohs(client_addr.sin_port));

			//寻找空闲位置添加文件描述符
			for(i=1;i<OPEN_MAX;i++)
				if(client[i].fd<0){
					client[i].fd = cfd;
					break;
				}
			if(i==OPEN_MAX) perr_exit("too many cfd\n");
			//设置刚刚添加进来的cfd监控事件类型
			client[i].events = POLLIN;
			//更新数组下标
			if(i>maxi) maxi = i;
			//没有更多就绪事件，继续回到poll阻塞等待
			if(--nready<=0) continue; 
		}
	//到这里说明lfd没满足条件，以下应该是处理就绪cfd的行为
	for(i=1;i<=maxi;i++){
		if((sockfd = client[i].fd)<0) continue;

		if(client[i].revents & POLLIN){

			if((n=Read(sockfd,buf,sizeof(buf)))<0){
				if(errno == ECONNRESET){
					printf("client[%d] abort connection\n",i);
					Close(sockfd);
					client[i].fd = -1;
				}else perr_exit("read error\n");
			}
		
		//客户端关闭链接情况
		else if(n==0){
			printf("client[%d] closed connection\n",i);
			Close(sockfd);
			client[i].fd=-1;
		}else{
			for(j=0;j<n;j++)
				buf[j]=toupper(buf[j]);

			Write(sockfd,buf,n);
			Write(STDOUT_FILENO,buf,n);
		}
		if(--nready<=0) break;
		}
	}
}

	return 0;}

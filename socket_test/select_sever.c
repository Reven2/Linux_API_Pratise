#include"wrap.h"
#include<sys/select.h>
#include<sys/time.h>
#define SERVER_PORT 7777


int main(int argc,char *argv[]){ 

	int lfd , cfd;
	int ret,i,n,j;
	char buf[BUFSIZ];     //BUFSIZ 	4096 bytes

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

	//status ：LISTEN
	Listen(lfd,128);	//socket, backlog(max connections) 
	
	socklen_t client_addr_len;			//客户端地址长度
	client_addr_len = sizeof(client_addr);
	
	fd_set allset;			//事件是否就绪 文件描述符集合
	
	FD_ZERO(&allset);
	FD_SET(lfd,&allset);	//把lfd加入集合

	fd_set rset;			//临时变量，因为是用做传入传出参数

	int maxfd;		//集合中最大文件描述符
	maxfd = lfd;

	while(1){
		rset = allset;	//备份
		//int select(int nfds, fd_set *readfds, fd_set *writefds,
		//                  fd_set *exceptfds, struct timeval *timeout); fd_set 是传入传出参数
		ret = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(ret<0) perr_exit("select error\n");
		//判断是否在就绪事件集合
		if(FD_ISSET(lfd,&rset)){
		//建立连接（SYN_RECV），注意不会阻塞，因为是由就绪事件我们才执行accept()，并不是阻塞等待事件到来
		cfd = Accept(lfd,(struct sockaddr*)&client_addr,&client_addr_len);
		//把新连接的cfd加入集合
		FD_SET(cfd,&allset);
			if(cfd>maxfd) maxfd = cfd;
			//以下说明只有lfd从select返回，无后续执行
			if(ret == 1) continue;
		}

		//循环寻找满足事件的那个fd
		for(i=lfd+1;i<=maxfd;i++){
			//处理事件
			if(FD_ISSET(i,&rset)){
				n = Read(i,buf,sizeof(buf));

				//检测到客户端关闭链接，移出集合
				if(n==0){ 
				Close(i);
				FD_CLR(i,&allset);
				}
				else if(n == -1) perr_exit("read error\n");
		
			//业务逻辑
			for(j=0;j<n;j++)
				buf[j]=toupper(buf[j]);
			
			write(i,buf,n);
			write(STDOUT_FILENO,buf,n);
			
		}
	}
}
	Close(lfd);
	
	return 0;}

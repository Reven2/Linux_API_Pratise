#include"wrap.h"
#include<sys/select.h>

#define SERVER_PORT 8888


int main(int argc,char *argv[]){ 

	int lfd,cfd,maxfd;
	int j,i,maxi,n;
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

	Listen(lfd,128);	//socket, backlog(max connections) 


	//自定义数组，防止遍历全部文件描述符（1024个）
	int nready,client[FD_SETSIZE];
	maxi = -1;  //数组下标
	//初始化数组
	for(i=0;i<FD_SETSIZE;i++){
		client[i] = -1;
	}
	//开始时lfd就是最大文件描述符
	maxfd = lfd;

	fd_set allset,rset;
	FD_ZERO(&allset);		//清零
	//内核其实已经记录了所有的就绪事件
	//是我们额外增加一个数组复制了一份就绪事件集合来提高效率
	FD_SET(lfd,&allset);	//添加
	
	socklen_t client_len = sizeof(client_addr);

	while(1){
		rset = allset;

		nready = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nready<0) perr_exit("select error\n");

		if(FD_ISSET(lfd,&rset)){
			cfd = Accept(lfd,(struct sockaddr*)&client_addr,&client_len);
		//寻找数组中没用使用的位置拿来保存新连接的cfd
			for(i=0;i<FD_SETSIZE;i++)
				if(client[i]<0){
					client[i] = cfd;
					break;
				}
			if(i==FD_SETSIZE) {
				perr_exit("overflow\n");
				exit(1);
			}

		FD_SET(cfd,&allset); //添加进集合
		if(cfd>maxfd) maxfd=cfd;
		if(i>maxi) maxi = i; 
		if(--nready==0) continue; //只有lfd
	}

		int sockfd ;  //临时fd
		//检测client[] 哪个就绪
		for(i=0;i<=maxi;i++){
			if((sockfd = client[i])<0) continue;
			if(FD_ISSET(sockfd,&rset)){
				if((n=Read(sockfd,buf,sizeof(buf)))==0){
					Close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i]=-1;
				}else if(n>0){
					for(j=0;j<n;j++)
						buf[j]=toupper(buf[j]);
					Write(sockfd,buf,n);
					Write(STDOUT_FILENO,buf,n);
				}
				if(--nready==0) break;
			}
		}
	}

	Close(lfd);

	return 0;}

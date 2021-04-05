#include"wrap.h"
#include<sys/epoll.h>
#define SERVER_PORT 9999
#define OPEN_MAX 1024

int main(int argc,char *argv[]){ 

	int lfd,cfd,sockfd;
	int i,j,nready;
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
	
	socklen_t client_addr_len;			//客户端地址长度
	client_addr_len = sizeof(client_addr);
	
	int n,efd,res;

	//tep:epoll_ctl参数  ep[]:epollwait传出参数
	struct epoll_event tep,ep[OPEN_MAX];

	//创建epoll兴趣表模型  efd指向红黑树root
	efd = epoll_create(OPEN_MAX);
	if(efd<0) perr_exit("epoll create error\n");

	//设置结构体属性
	tep.events = EPOLLIN;
	tep.data.fd = lfd;

	//将lfd及对应的结构体添加到树上，efd可以找到树
	res = epoll_ctl(efd,EPOLL_CTL_ADD,lfd,&tep);
	if(res == -1) perr_exit("add lfd and tep error\n");
	
	while(1){
		//阻塞监听事件，ep为 epoll_event类型数组
		//OPEN_MAX是数组最大容量，-1代表永久阻塞
		//成功返回ep数组中的元素个数，失败返回-1
		nready = epoll_wait(efd,ep,OPEN_MAX,-1);
		if(nready==-1) perr_exit("epoll_wait error\n");

		//循环判断满足事件是哪种事件
		for(i=0;i<nready;i++){
			//不是读事件，继续循环
			if(!(ep[i].events & EPOLLIN)) continue;
			//是监听事件,处理接受链接
			if(ep[i].data.fd == lfd){
			 	cfd = Accept(lfd,(struct sockaddr*)&client_addr,&client_addr_len);
			//把cfd添加到树上（efd）
				tep.events = EPOLLIN;
				tep.data.fd = cfd;
				res = epoll_ctl(efd,EPOLL_CTL_ADD,cfd,&tep);
				if(res == -1) perr_exit("add error\n");
			}
			//不是lfd
			else{
				sockfd = ep[i].data.fd;
				n = Read(sockfd,buf,sizeof(buf));
				//读到0字节，说明客户端关闭了链接,删除此节点
				if(n==0){
					res = epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);
					if(res == -1) perr_exit("delete error\n");
					Close(sockfd);//关闭连接
				}
			
				else if(n<0){
					res = epoll_ctl(efd,EPOLL_CTL_DEL,sockfd,NULL);
					Close(sockfd);}		

				else{
					for(j=0;j<n;j++)
						buf[j] = toupper(buf[j]);
				
					Write(sockfd,buf,n);
					Write(STDOUT_FILENO,buf,n);
				}
			}
		}
	}
	
	Close(lfd);

	return 0;}

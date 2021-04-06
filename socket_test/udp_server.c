#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<ctype.h>

#define SERV_PORT 9000		//端口用来在网络中唯标识一个进程


//不检查返回值了，突出逻辑观察与TCP通信的不同之处
int main(void){

	struct sockaddr_in server_addr,client_addr;
	socklen_t client_addr_len;

	int sockfd;
	char buf[BUFSIZ];
	char str[INET_ADDRSTRLEN];   //INET_ADDRSTRLEN 16
	int i,n;
	//int socket(int domain, int type, int protocol);
	//注意使用协议不同，第三个参数标识默认协议
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   //系统自动分配一个可用服务器地址

	bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));

	printf("Accepting connections....\n");

	client_addr_len = sizeof(client_addr_len);

	while(1){
		//ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
		//					struct sockaddr *src_addr, socklen_t *addrlen);
		//此函数做了相当于 accept()的事情，后面两个参数是传出和传入传出参数
		n = recvfrom(sockfd,buf,BUFSIZ,0,(struct sockaddr*)&client_addr,&client_addr_len);
		if(n==-1) perror("recvfrom error\n");

		printf("recived from %s at port %d\n",
				inet_ntop(AF_INET,&client_addr,str,sizeof(str)),
				ntohs(client_addr.sin_port));
			
		for(i=0;i<n;i++)
			buf[i]=toupper(buf[i]);

		write(STDOUT_FILENO,buf,n); //写到屏幕上观察一下
//ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
//                      const struct sockaddr *dest_addr, socklen_t addrlen);
//小写转大写完成 直接发回去给客户端 ,TCP下:write(cfd,buf,n);后两个参数传入传出
		n = sendto(sockfd,buf,n,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
		if(n==-1) perror("sendto error\n");
	}

close(sockfd);
}

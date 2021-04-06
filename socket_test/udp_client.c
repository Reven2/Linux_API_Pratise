#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<ctype.h>


//netconnect 默认是TCP不能用来测试 必须要写客户端
//udp不用建立链接，默认可用并发链接多个客户端
//
#define SERV_PORT 9000

int main(int argc,char **argv){

	struct sockaddr_in sever_addr;
	int sockfd,n;
	char buf[BUFSIZ];

	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	bzero(&sever_addr,sizeof(sever_addr));
	sever_addr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&sever_addr.sin_addr);
	sever_addr.sin_port = htons(SERV_PORT);


//	bind(sockfd,(struct sockaddr*)&sever_addr,sizeof(sever_addr)); 

	while(fgets(buf,BUFSIZ,stdin)!=NULL){
		
		//类似connect的作用,注意一定是buf内数据实际大小，后面两个参数是传出参数
		n = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&sever_addr,sizeof(sever_addr));
		if(n==-1) perror("sendto error\n");

		//这里是接收数据，实际上用不到server端的地址结构，可以传入0,因为服务器地址固定，早就在上面绑定好了
		//但是如果是在服务器端，此函数是为了获得客户端的地址
		n = recvfrom(sockfd,buf,BUFSIZ,0,NULL,0);
		if(n==-1) perror("recvfrom error\n");

		write(STDOUT_FILENO,buf,n);
	
	}

	close(sockfd);

return 0;
}

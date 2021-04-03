#include"wrap.h"

#define SERVER_PORT 9527


int main(int argc,char *argv[]){ 

	int lfd = 0 , cfd;
	int ret,i ;
	char buf[BUFSIZ],client_IP[1024];     //BUFSIZ 	4096 bytes

	struct sockaddr_in server_addr,client_addr;	//定义结构体
	//初始化
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY 表示取的一个可用的主机地址
	server_addr.sin_port = htons(SERVER_PORT);	//注意port是 short,否则客户端将连不上！！
	
	lfd = Socket(AF_INET,SOCK_STREAM,0);

	Bind(lfd,(struct sockaddr *)&server_addr,sizeof(server_addr)); //绑定addr，socket，addrlen

	Listen(lfd,128);	//socket, backlog(max connections) 
	
	socklen_t client_addr_len;			//客户端地址长度
	client_addr_len = sizeof(client_addr);
	
	//注意cfd是server的缓存socket 不是client的
	cfd = Accept(lfd,(struct sockaddr *)&client_addr,&client_addr_len);//第二个是传出参数

	printf("client ip is %s   port is %d\n",
	inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_IP,sizeof(client_IP)),
	ntohs(client_addr.sin_port));


	while(1){
		ret = read(cfd,buf,sizeof(buf));	//server 端从 client（cfd） 端读数据到buf，ret 实际读到字节数
		write(STDOUT_FILENO,buf,ret);		//写到屏幕上方便观察
		for( i=0;i<ret;i++) buf[i] = toupper(buf[i]);

		write(cfd,buf,ret);				//把buf数据写回去到cfd（客户端）
	}
	close(lfd);
	close(cfd);

	return 0;}

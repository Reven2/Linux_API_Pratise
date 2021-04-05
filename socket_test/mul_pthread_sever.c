#include"wrap.h"
#include<pthread.h>
#define MAXLINE 8192
#define SERV_PORT 8000


struct s_info{
	struct sockaddr_in client_addr;	//用结构体 把地质结构和cfd捆绑
	int connfd;
};

//thread working function
void *do_work(void *arg){
	int n,i;
	struct s_info *ts = (struct s_info*)arg;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];   //define INET_ADDRSTRLEN 16 系统文件自带的一个macro 这里用来装地址字符串

	while(1){
		n = Read(ts->connfd,buf,MAXLINE);		//读客户端
		if(n==0){
			printf("the client %d closed....\n",ts->connfd);
			break;					//跳出循环 关闭cfd
		}
		printf("received from %s at port %d\n",			
				inet_ntop(AF_INET,&(*ts).client_addr.sin_addr,str,sizeof(str)),
				ntohs((*ts).client_addr.sin_port));	//打印客户端信息（IP/PORT）

		for(i=0;i<n;i++) buf[i]=toupper(buf[i]);
		Write(STDOUT_FILENO,buf,n);	//写到屏幕方便观察
		Write(ts->connfd,buf,n);		//write back 到 客户端
	}
	Close(ts->connfd);
	return (void *)0;			//equal : pthread_exit(0);

}


int main(int argc,char**argv){
	
	struct sockaddr_in serv_addr,client_addr;
	socklen_t client_len;
	int listenfd,connfd;
	pthread_t tid;

	struct s_info ts[256];			//结构体数组
	int i =0;
	
	listenfd = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //指定系统的IP
	serv_addr.sin_port = htons(SERV_PORT);

	Bind(listenfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	Listen(listenfd,128);		//通识课链接服务器上线
	printf("Accepting client connect....\n");

	while(1){
		client_len = sizeof(client_addr);
		connfd = Accept(listenfd,(struct sockaddr *)&client_addr,&client_len);

		ts[i].client_addr = client_addr;
		ts[i].connfd = connfd;
	
		pthread_create(&tid,NULL,do_work,(void*)&ts[i]);
		//pass by value 强制转换会丢失，因为结构体不止一个指针大小！
		//这里是read only ，pass by pointer 也可以
		pthread_detach(tid);   //分离,防止僵尸线程
		i++;
	}

return 0;
}

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAXSIZE 2000

//发送应答报文 和 404 错误页面
void send_error(int cfd){

	char buf[4096] = {0};
	//把报文拼接到一个buf内 一起发过去
	sprintf(buf,"%s %d %s\r\n","HTTP/1.1",404,"Not Found");
	sprintf(buf+strlen(buf),"Content-Type:text/html;charset=iso-8859-1\r\n");
	sprintf(buf+strlen(buf),"Content-Length:%d\r\n",-1);
	sprintf(buf+strlen(buf),"Connection:close\r\n");
	send(cfd,buf,strlen(buf),0);
	send(cfd,"\r\n",2,0);

	memset(buf,0,sizeof(buf));

	sprintf(buf, "<html><head><title>%d %s</title></head>\n", 404, "Not Found");
	sprintf(buf+strlen(buf), "<body bgcolor=\"#cc99cc\"><h2 align=\"center\">%d %s</h4>\n", 404, "Not Found");
	sprintf(buf+strlen(buf), "%s\n", "no this file");
	sprintf(buf+strlen(buf), "<hr>\n</body>\n</html>\n");
	send(cfd, buf, strlen(buf), 0);

	return ;
}


//发送服务器本地文件给浏览器
void send_file(int cfd,const char *file){
	int fd = open(file,O_RDONLY);
	int ret ;

	if(fd == -1) {
		send_error(cfd);
		perror("open error\n");
		exit(1);
	}
	int n = 0;
	char buf[1024];
	while((n = read(fd,buf,sizeof(buf)))>0){

		ret = send(cfd,buf,n,0);
		if(ret == -1){
			printf("errno = %d\n",errno);
			//要判断错误号，下面两种错判号 可能会打断发送文件的行为
			if(errno == EAGAIN) continue;
			else if (errno == EINTR) continue;
			else {
				perror("send error\n");
				exit(1);
			}
		}	
	}
	close(fd);
}

//通过文件名获取文件的类型
const char *get_file_type(const char *file_name){
	char *dot;
	//The strrchr() function returns a pointer to the last occurrence of the character c in the string s.
	dot = strrchr(file_name,'.');
	if (dot == NULL)
		    return "text/plain; charset=utf-8";
	    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
			return "text/html; charset=utf-8";
		if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
			return "image/jpeg";
		if (strcmp(dot, ".gif") == 0)
			return "image/gif";
		if (strcmp(dot, ".png") == 0)
			return "image/png";
		if (strcmp(dot, ".css") == 0)
			return "text/css";
		if (strcmp(dot, ".au") == 0)
			return "audio/basic";
		if (strcmp( dot, ".wav" ) == 0)
			return "audio/wav";
		if (strcmp(dot, ".avi") == 0)
			return "video/x-msvideo";
		if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
			return "video/quicktime";
		if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
			return "video/mpeg";
		if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
			return "model/vrml";
		if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
			return "audio/midi";
		if (strcmp(dot, ".mp3") == 0)
			return "audio/mpeg";
		if (strcmp(dot, ".ogg") == 0)
			return "application/ogg";
		if (strcmp(dot, ".pac") == 0)
			return "application/x-ns-proxy-autoconfig";

		return "text/plain; charset=utf-8";

}



//回发回答报文,客户端cfd，错误号，错误描述，回发文件类型，文件长度（要么不求长度，那么一定要对）
void send_respond(int cfd,int no,char *discription,const char *type,int len){
	
	//Http1.1 200 OK
	//Content-Type：text/plain; charset=iso-8859-1 
	//Content-Length: 32  （ 要么不写 或者 传-1， 要写务必精确 ！ ）
	
	// int sprintf(char *str, const char *format, ...);
	char buf[1024] = {0};
	//把报文拼接到一个buf内 一起发过去
	sprintf(buf,"HTTP/1.1 %d %s\r\n",no,discription);
	sprintf(buf+strlen(buf),"%s\r\n",type);
	sprintf(buf+strlen(buf),"Content-Length:%d\r\n",len);
	send(cfd,buf,strlen(buf),0);
	send(cfd,"\r\n",2,0); 				//注意http最后一行的 \r\n 结束标记
}


//处理http请求的文件，判断文件是否存在，回发
void http_request(int cfd ,const char*file){
	struct stat sbuf;
	//int stat(const char *pathname, struct stat *statbuf);获取文件状态
	int ret = stat(file,&sbuf);
	if(ret != 0) {
		send_error(cfd);
		perror("stat error\n");
		exit(1);
	}

	//如果是一个普通文件
	if(S_ISREG(sbuf.st_mode)){
	//要先发http协议应答
	//注意传入的文件类型一定要和请求的文件类型一样,这里是jpg图片类型
	send_respond(cfd,200,"OK",get_file_type(file),sbuf.st_size);
	//发送完应答协议后发送 请求的文件
	send_file(cfd,file);
	}
}



//读一行以/r/n结尾的数据
int get_line(int cfd, char *buf, int size)
{
	    int i = 0;
		char c = '\0';
		int n;
		while ((i < size - 1) && (c != '\n')) {    
			n = recv(cfd, &c, 1, 0);
			if (n > 0) {        
				if (c == '\r') {            
					n = recv(cfd, &c, 1, MSG_PEEK);//MSG_PEEK拷贝读一行，缓冲区的数据还在
					if ((n > 0) && (c == '\n')) {               
						recv(cfd, &c, 1, 0);
					 }
					else    c = '\n';
				  }

					buf[i] = c;
					i++;
				} 
				else    c = '\n';
				}
				buf[i] = '\0';
				return i; //读到数据的长度
}

//断开连接
void disconnect(int cfd,int epfd){
	int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,cfd,NULL);
	if(ret == -1){
		printf("epoll_ctl_del error\n");
		exit(1);
	}
	close(cfd);
}


void do_read(int cfd,int epfd){
	//读取一行http协议，拆分，获取 请求方法 文件名 协议号
	char line[1024] = {0};
	int len = get_line(cfd,line,sizeof(line));
	//len = 0 说明客户端断开连接了
	if(len == 0 ){
		disconnect(cfd,epfd);
		printf("client was disconnected");
	}
	else{
		char method[16],path[256],protocol[16];
		//int sscanf(const char *str, const char *format, ...) 从字符串读取格式化输入
		//下面用正则表达式来分割读到的第一行字符串 提取需要的 请求方法 文件路径 协议版本
		sscanf(line,"%[^ ] %[^ ] %[^ ]",method,path,protocol);
		printf("method : %s		path : %s	protocol : %s",method,path,protocol);	
		
		//循环读取后面的每一行http报文
		while(1){
			char buf[1024] = {0};		//重新定一个读缓冲区
			len = get_line(cfd,buf,sizeof(buf));
			if(buf[0] == '\n') break;	//http请求协议报文 最后一行只有一个\n,读到最后就说明读完了整个报文 退出
			else if(len==-1) {			//read error
				printf("read line error\n");
				break;
			}
		}
		//比较获取的信息是否正确，如果正确则开始处理请求
		//The strcasecmp() function compares the two strings s1 and s2, ignoring the case of the characters.
		//int strncasecmp(const char *s1, const char *s2, size_t n);
		if(strncasecmp(method,"GET",3)==0){
			char *file = path+1;	//取出客户要访问的文件名
			
			//获取文件属性
			struct stat st;
			int ret = stat(file,&st);
			if(ret == -1){
				send_error(cfd);
				return;
			}

			http_request(cfd,file);	//处理请求http请求文件
		}
	}
}


void do_accept(int lfd,int epfd){

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr_len);
	int cfd;

	cfd = accept(lfd,(struct sockaddr*)&client_addr,&(client_addr_len));
	if(cfd == -1){
		printf("accept error\n");
		exit(1);
	}
	//打印一下链接客户端信息
	char client_ip[64] = {0};
	printf("New client ip:%s,port:%d,cfd=%d\n",
			inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,client_ip,sizeof(client_ip)),
			ntohs(client_addr.sin_port),cfd);
	//设置cfd非阻塞
	int flag = fcntl(cfd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd,F_SETFL,flag);

	//ET+非阻塞模式，新节点挂到树上监听
	struct epoll_event ev;
	ev.data.fd = cfd;
	ev.events = EPOLLIN|EPOLLET;
	int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);	
	if(ret == -1){
		printf("epoll_ctl error\n");
		exit(1);
	}
}


int init_listen_fd(int port,int epfd){
	
	//创建监听套接字lfd
	int lfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_addr;

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr =  htonl(INADDR_ANY);

	//端口复用
	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	//绑定地址结构
	int ret = bind(lfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(ret == -1){
		printf("bind error\n");
		exit(1);
	}

	ret = listen(lfd,128);
	if(ret == -1){
		printf("listen error\n");
		exit(1);
	}

	struct epoll_event ev;	//关联lfd的结构体，用于epoll_ctl参数
	ev.events = EPOLLIN;
	ev.data.fd = lfd;

	//添加上树
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
	if(ret == -1){
		printf("epoll_ctl error\n");
		exit(1);
	}

	return lfd;
}




void epoll_run(int port){
	
	//创建内核事件表，监听红黑树根
	int epfd = epoll_create(MAXSIZE);
	if(epfd == -1){
		perror("epoll_create error\n");
		exit(1);			//异常退出程序	
	}

	int lfd = init_listen_fd(port,epfd);

	//用于epoll_wait 传出参数，装载所有的就绪事件
	struct epoll_event all_events[MAXSIZE];

	while(1){
		int ret = epoll_wait(epfd,all_events,MAXSIZE,-1);//-1表示阻塞等待就绪事件集返回
		if(ret == -1){
		perror("epoll_wait error\n");
		exit(1);
		}

		//循环处理就绪事件
		for(int i=0;i<ret;i++){
			struct epoll_event *pev = &all_events[i];
			//只处理读事件，其他类型事件 先忽略
			if(!(pev->events & EPOLLIN)) continue;

			//处理连接请求
			if(pev->data.fd == lfd)
				do_accept(lfd,epfd);
			else 
				do_read(pev->data.fd,epfd);
		}
	}
}



int main(int argc,char**argv){
	
	//命令行参数获取 用户输入 端口 和 severt提供的目录
	if(argc<3){
		printf("usage:./server port path\n");
	}
	
	int port = atoi(argv[1]); 
	
	//改变进程工作目录,直接就可以请求文件名打开，方便测试用，如果没有这个，只输入文件名是请求不到的
	int ret = chdir(argv[2]);
	if(ret!=0){
		perror("chdir error\n");
		exit(1);			//异常退出程序
	}

	epoll_run(port);

	return 0;
}


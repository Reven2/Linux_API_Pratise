#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>


// 读缓冲区回调
void read_cb(struct bufferevent *bev, void *arg){
		char buf[1024] = {0};  
		//由于没有文件描述符故不能使用read(),只能使用包含着文件描述符的bev的bufferevent_read()
		bufferevent_read(bev, buf, sizeof(buf));
		
		printf("client say: %s\n", buf);

      	char *p = "我是服务器, 已经成功收到你发送的数据!";
        
		// 发数据给客户端
    	bufferevent_write(bev, p, strlen(p)+1);
        sleep(1);
            }



// 写缓冲区回调
void write_cb(struct bufferevent *bev, void *arg)
 {
  	printf("我是服务器, 成功写数据给客户端,写缓冲区回调函数被回调...\n"); 
          }




void event_cb(struct bufferevent *bev, short events, void *arg)
{
	    if (events & BEV_EVENT_EOF) 
			printf("connection closed\n");  
							   
		else if(events & BEV_EVENT_ERROR)   
			 printf("some other error\n");
								    
			 
		bufferevent_free(bev);    
		printf("buffevent 资源已经被释放...\n"); 
}



//监听回调,看笔记上有这个函数的参数的解释，且这个函数是满足条件时系统自动帮我们调用
void cb_listener(struct evconnlistener* listener,
				 evutil_socket_t fd,
				 struct sockaddr* addr,
				 int len,void* ptr)
{
	printf("connect new client\n");
	struct event_base* base = (struct event_base*)ptr;

	//添加新事件
	struct bufferevent* bev;
	bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);

	//给bufferevent缓冲区设置回调,读回调，写回调，事件回调
	bufferevent_setcb(bev,read_cb,write_cb,event_cb,NULL);

	//启用bufferevent的 读缓冲， 因为默认是disable 要设置一下可用
	bufferevent_enable(bev,EV_READ);
}



int main(int argc,const char** argv){
	//init server
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9876);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//创建框架base
	struct event_base* base;
	base = event_base_new();

	//创建套接字，绑定，接收链接请求
	struct evconnlistener* listener;		//监听器
	evconnlistener_new_bind(base,cb_listener,base,
							LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,
							36,(struct sockaddr*)&server_addr,sizeof(server_addr));

	//启动循环监听
	event_base_dispatch(base);

	evconnlistener_free(listener);
	event_base_free(base);

return 0;
}


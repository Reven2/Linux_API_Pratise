#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <arpa/inet.h>

void read_cb(struct bufferevent *bev, void *arg)
{
	    char buf[1024] = {0}; 
		bufferevent_read(bev, buf, sizeof(buf));

		printf("fwq say:%s\n", buf);

		bufferevent_write(bev, buf, strlen(buf)+1);
		sleep(1);
}

void write_cb(struct bufferevent *bev, void *arg)
{
	    printf("----------我是客户端的写回调函数,没卵用\n"); 
}

void event_cb(struct bufferevent *bev, short events, void *arg)
{
	    if (events & BEV_EVENT_EOF)
			printf("connection closed\n");  
							   
		else if(events & BEV_EVENT_ERROR)   
			 printf("some other error\n");
								    
		else if(events & BEV_EVENT_CONNECTED)
		{
			printf("已经连接服务器...\\(^o^)/...\n");
			 return;
											    }
				    
		bufferevent_free(bev);
}


void read_terminal(evutil_socket_t fd, short what, void *arg)
{

	    char buf[1024] = {0};
		int len = read(fd, buf, sizeof(buf));

		struct bufferevent* bev = (struct bufferevent*)arg;

		bufferevent_write(bev, buf, len+1);
}

int main(int argc, const char* argv[])
{
	     struct event_base* base = NULL;
		 base = event_base_new();

	   	 int fd = socket(AF_INET, SOCK_STREAM, 0);

				 
		 struct bufferevent* bev = NULL;
		 bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);


		 struct sockaddr_in serv;
		 memset(&serv, 0, sizeof(serv));
		 serv.sin_family = AF_INET;
		 serv.sin_port = htons(9876);
		 inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr.s_addr);



 		bufferevent_socket_connect(bev, (struct sockaddr*)&serv, sizeof(serv));
 
 		bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);


  		struct event* ev = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, read_terminal, bev);

	 	event_add(ev, NULL);

	 	event_base_dispatch(base);

	 	event_free(ev);
																	    
	 	event_base_free(base);

	  	return 0;
}


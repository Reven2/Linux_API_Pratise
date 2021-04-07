#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<event2/event.h>

//对操作处理回调函数
void read_cb(evutil_socket_t fd,short what,void *arg){
	//读管道
	char buf[1024] = {0};
	int len = read(fd,buf,sizeof(buf));
	printf("read event :%s \n",what & EV_READ ? "yes":"no");//后面的判断式是用来判断事件类型是不是读事件
	printf("data length = %d,buf = %s\n",len,buf);
	
	sleep(1);
	return;
}

int main(int argc , const char **argv){

	//存在myfifo就先删掉 下面重新创建
	unlink("myfifo");
	//创建有名管道
	mkfifo("myfifo",0664);

	//open file
	int fd = open("myfifo",O_RDONLY|O_NONBLOCK);
	if(fd==-1) perror("open error\n");

	//创建event_base（事件处理器，相当于一个Reactor实例）
	struct event_base* base = event_base_new();
	
	//创建事件,当内核检测到就绪事件通过fd通知应用程序，fd与base（通用事件处理器）相关联
	//注意这里也给回调函数传入了所需要的参数，what 参数是对应事件类型
	//struct event *event_new(struct event_base *base，evutil_socket_t fd，short what，event_callback_fn cb;  void *arg);
	struct event* ev = NULL;
	ev = event_new(base,fd,EV_READ|EV_PERSIST,read_cb,NULL);

	//添加事件，把事件加入到，注意到base已经在event_new传入了,传入返回的ev！
	//int event_add(struct event *ev, const struct timeval *tv);
	event_add(ev,NULL);

	//事件循环，监听
	event_base_dispatch(base);


	//释放资源
	event_free(ev);
	event_base_free(base);
	close(fd);

return 0;
}

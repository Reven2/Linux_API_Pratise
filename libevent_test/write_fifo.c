#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>
#include<event2/event.h>

//对操作处理回调函数
void write_cb(evutil_socket_t fd,short what,void *arg){

	char buf[1024] = {0};
	static int num = 0;

	//例：将”test 1 2”写入数组s中: sprintf(s,"%s%d%c","test",1,'2');
	sprintf(buf,"hello,world-%d\n",num++);
	write(fd,buf,strlen(buf)+1);
	
	sleep(1);
}

int main(int argc , const char **argv){


	//open file 打开读端
	int fd = open("myfifo",O_WRONLY|O_NONBLOCK);
	if(fd==-1) perror("open error\n");

	//创建event_base（相当于一个Reactor实例）,写管道
	struct event_base* base = NULL;
	base = event_base_new();			//初始化base
	
	//创建事件,当内核检测到就绪事件通过fd通知应用程序，fd与base（通用事件处理器）相关联
	//注意这里也给回调函数传入了所需要的参数
	struct event* ev = NULL;
	ev = event_new(base,fd,EV_WRITE|EV_PERSIST,write_cb,NULL);

	//添加事件，把事件加入到，注意到base已经在event_new传入了,传入返回的ev！
	event_add(ev,NULL);

	//事件循环，监听
	event_base_dispatch(base);


	//释放资源
	event_free(ev);
	event_base_free(base);
	close(fd);

return 0;
}

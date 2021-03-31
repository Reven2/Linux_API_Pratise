#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

void err_thread(int ret,char*str){	
	
	if(ret!=0)
	{
		fprintf(stderr,"%s:%s\n",str,strerror(ret));
		pthread_exit(NULL);
		}
}

struct msg{
	int num;
	struct msg*next;
};

struct msg *head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	//定义和初始化必须要同时进行
//pthread_mutex_init(&mutex,NULL);

pthread_cond_t has_data = PTHREAD_COND_INITIALIZER ; //dynamic : pthread_cond_init(has_data,NULL)



void *producer(void *arg){
	while(1){
	struct msg *mp = malloc(sizeof(struct msg));
	mp->num = rand()%1000 + 1 ; //模拟生产，这是一个数字
	printf("-------produce a number %d\n",mp->num);
	
	pthread_mutex_lock(&mutex);
	mp->next = head;		//头插法，写共享内存
	head = mp;
	pthread_mutex_unlock(&mutex);
	
	pthread_cond_signal(&has_data);	//唤醒阻塞在条件变量上的线程
	sleep(rand()%3);
	}
	return NULL;
}


void *consumer(void *arg){
	while(1){
	struct msg *mp;
	pthread_mutex_lock(&mutex);  

	while(head == NULL){	//多个消费者应该while判断,当producer 同时唤醒阻塞在条件变量上的多个consumer , 可能产品不够分，所以每次消费一个之前先判断有没有产品，如果有跳过下面这行代码去消费产品，如果没有了就当前线程阻塞
		pthread_cond_wait(&has_data,&mutex); //block ，wait condition and unlock, until sigal came unlock
	}
	
	mp = head;		//删除头部节点
	head = mp->next ;

	pthread_mutex_unlock(&mutex);	
	printf("--------comsume a number %d, ID %lu\n",mp->num,pthread_self());
	free(mp);
	sleep(rand()%3);
	}
	return NULL;
}


int main(int argc,char *argv[]){ 
	
	int ret;
	pthread_t pid,cid;
	
	srand(time(NULL));
	ret = pthread_create(&pid,NULL,producer,NULL);
		if(ret!=0) err_thread(ret,"producer err\n");
	
	ret = pthread_create(&cid,NULL,consumer,NULL);
		if(ret!=0) err_thread(ret,"consumer err\n");

	ret = pthread_create(&cid,NULL,consumer,NULL);
		if(ret!=0) err_thread(ret,"consumer err\n");
	
	pthread_join(pid,NULL);
	pthread_join(cid,NULL);
	pthread_join(cid,NULL);
	
	return 0;
	}

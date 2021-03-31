#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#define NUM 5

int queue[NUM];
sem_t blank_number,producer_number;

void *producer(void* arg){
	int i = 0;

	while(1){
	sem_wait(&blank_number);		//空格数--
	queue[i] = rand()%1000+1;		//产生产品
	printf("-----produce---%d\n",queue[i]);
	sem_post(&producer_number);		//产品数 ++

	i = (i+1)%NUM; 			//实现环形队列
	sleep(rand()%1);
	}
	return NULL;
}


void *consumer(void* arg){
	
	int i = 0;

	while(1){
		sem_wait(&producer_number);  //类似加锁然后产品--，产品为0则阻塞
		printf("Consumer----%d\n",queue[i]);
		queue[i] = 0;				//空格数据为0表示空格
		sem_post(&blank_number);  //类似解锁，但是产品空格--

		i=(i+1)%NUM;
		sleep(rand()%3);
	}
	return NULL;
}







int main(int argc, char**argv){

	pthread_t pid ,cid;

	sem_init(&blank_number,0,NUM);
	sem_init(&producer_number,0,0);

	pthread_create(&pid,NULL,producer,NULL);	 
	pthread_create(&cid,NULL,consumer,NULL);

	pthread_join(pid,NULL);
	pthread_join(cid,NULL);

	sem_destroy(&blank_number);
	sem_destroy(&producer_number);

	return 0;
}

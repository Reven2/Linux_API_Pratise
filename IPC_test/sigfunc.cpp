#include<iostream>
#include<signal.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
using namespace std;

void print_set(sigset_t *set)
{
	for(int i=1;i<32;i++)
	{
		if(sigismember(set,i))
			putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}


int main(int argc,char* argv[])
{
	
	sigset_t set,oldset,pedset;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);

	int	ret = sigprocmask(SIG_BLOCK,&set,&oldset);
	if(ret==-1)
		printf("error ocurred\n");

	//ctrl+c will doesnt work and set it 1 in pedset 
	while(1){
		ret = sigpending(&pedset);
		if(ret==-1)
		printf("error ocurred\n");
		sleep(1);
		print_set(&pedset);
	}
	
	return 0;
}

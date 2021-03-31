#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void){

	pid_t pid;

	if((pid = fork()<0))
	{
		perror("fork error\n");
	 	exit(1);
	}else if(pid == 0 ){
		printf("child process PID is %d\n",getpid());
		printf("group ID of child is %d\n",getpgid(0));
		printf("session ID of child is %d\n",getsid(0));
		
		sleep(10);
		setsid();	//子进程不是组长进程，故成为新的会话首进程和组长进程
		
		printf("changed:\n");
		
		printf("child process PID is %d\n",getpid());
		printf("group ID of child is %d\n",getpgid(0));
		printf("session ID of child is %d\n",getsid(0));
		
		sleep(20);
		exit(0);
	}

}

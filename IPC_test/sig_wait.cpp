#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdio.h>
#include<pthread.h>



using namespace std;


//when child status changed kernel will invoke this func
void catch_child(int signo)
{

	//为什么wait没有回收全部子进程二产生zoom？
	//cuz signal 是不排队的!! 还有需要 while wait！
	// one wait only recycle one zoom 
	pid_t wpid;
	while((wpid = wait(NULL))!= -1)//wait() is blockin	
	//waitpid(-1,&status,0)
	//if(WIFEXITED(status))
	{
		printf("catch child %d\n",wpid);
	}
	return;
}


int main(int argc,char* argv[])
{
	pid_t pid;
	int i;
	for(i=0;i<5;i++)
		if((pid = fork())==0)
			break;

	if(i==5)	//when i equal 5 is parent 
	{

		//如果函数还没注册 子进程就先行死亡该如何解决？
		//内核为每个进程维护信号掩码，并将阻塞其对进程传递
		
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set,SIGCHLD);
		sigprocmask(SIG_BLOCK,&set,NULL);

		struct sigaction act;
		
		act.sa_handler = catch_child; //callback func
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGCHLD,&act,NULL);//when SIGCHLD callback 
	
		//解除阻塞
		sigprocmask(SIG_UNBLOCK,&set,NULL);

		printf("I am parent pid = %d\n",getpid());
	
		while(1);  //keep parent runing before all child dead
	}
	
	else
	{
		printf("I am child pid = %d\n",getpid());
	}

		return 0;
}

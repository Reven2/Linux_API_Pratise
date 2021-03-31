#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>

void sys_err(const char *str){	perror(str); exit(1);}


//when several child dead , signal come in the same time ,can only deal with a signal 
//so it probably cuz zoombiz child 
void catch_child(int signo){ //有child终止，发送SIGCHLD，该函数被回调
	pid_t wpid ;
	int status ; 
//	while((wpid = wait(NULL))!=-1)
	while((wpid = waitpid(-1,&status,0))!=-1){
		if(WIFEXITED(status))
		printf("--------catch child id %d,return = %d\n",wpid,WEXITSTATUS(status));
	}
	return ; 
}


int main(int argc,char *argv[]){
	pid_t pid;
	//block,make the signal is undecided,in case child was dead in front of parent registering
	//cause canot recycle the child
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGCHLD);
	sigprocmask(SIG_BLOCK,&set,NULL);


	int i ;
	for(i=0;i<5;i++){
		if((pid = fork())==0) break;   //child break out from this loop
	}	
	
	if(5==i){		//parent 	
		struct sigaction act;  //here register ,cuz it all about parent which recycle child
		act.sa_handler = catch_child ; //call back function
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;         // usually initialize like this
		
		sigaction(SIGCHLD,&act,NULL); //register signal
	//release block,如果不解除阻塞，sigaction没有机会执行
		sigprocmask(SIG_UNBLOCK,&set,NULL);

		printf("I am parent,pid = %d\n",getpid());
		while(1); //make parent alive
	}	
	else{			//child
		printf("I am child pid = %d\n",getpid());
		return i;
	}
	
	
	return 0;}







#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>


void sig_catch(int signo){	//callback function
	if(signo == SIGINT){
	printf("catch you !\n");}
	return ;
}






void sys_err(const char *str){	perror(str); exit(1);}


int main(int argc,char *argv[]){ 
	
	struct sigaction act, oldact;
	int ret;

	act.sa_handler = sig_catch;  //set callback function name
	sigemptyset(&(act.sa_mask)); 	//set nasj when sig_catch working
	act.sa_flags = 0 ;		//default
	
	ret = sigaction(SIGINT,&act,&oldact); //rigsiter a signal
	if(ret == -1) sys_err("sigaction err\n");

	
	while(1);
	
	return 0;}

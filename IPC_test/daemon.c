#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
void sys_err(const char *str){	perror(str); exit(1);}
int main(int argc,char *argv[]){ 
	
	pid_t pid ;

	pid = fork();
	if(pid>0) exit(0); //parent terminated

	pid = setsid(); //create session
	if(pid == -1) sys_err("setsid err\n");

	int ret = chdir("/");

	umask(0022);		//改变文件访问权限
	
	int fd;
	close(STDIN_FILENO);  //关闭文件描述符0
	open("/dev/null",O_RDWR);

	dup2(fd,STDOUT_FILENO); //重定向
	dup2(fd,STDERR_FILENO) ;
	
	while(1);    //模拟业务逻辑
	return 0;}

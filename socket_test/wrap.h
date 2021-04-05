#ifndef __WRAP_H_  
#define __WRAP_H_  
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<strings.h>
#include<poll.h>
#include<pthread.h>


void perr_exit(const char *s);  
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);  
int Bind(int fd, const struct sockaddr *sa, socklen_t salen);  
int Connect(int fd, const struct sockaddr *sa, socklen_t salen);  
int Listen(int fd, int backlog);  
int Socket(int family, int type, int protocol);  
ssize_t Read(int fd, void *ptr, size_t nbytes);  
ssize_t Write(int fd, const void *ptr, size_t nbytes);  
int Close(int fd);  
 
#endif

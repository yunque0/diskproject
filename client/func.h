#ifndef __FUNC_H__
#define __FUNC_H__
#include <sys/epoll.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#define args_check(a,b)  { if(a!=b) {printf("arg error\n");return -1;}}
#define error_check(arg,num,info) {if(arg==num){perror(info);return -1;}}
#define _XOPEN_SOURCE
//int tcp_server(int*,char *,int ,int);
//int send_s(int,char *,int);
//int tran_file(int);
typedef struct node
{
	int data;
	struct node *pnext;
}node,*pnode;
typedef struct 
{
	int len;
	char context[1000];
}msgtrain;
int send_file(int,char*);
int recv_file(int,char*);
int send_s(int ,char *,int);
int recv_s(int ,char *,int);
char *crypt(const char *key,const char *salt);
#endif

#ifndef __FUNC_H__
#define __FUNC_H__
#include <mysql/mysql.h>
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
//int tcp_server(int*,char *,int ,int);
//int send_s(int,char *,int);
//int tran_file(int);
typedef struct node
{
	int data;
	int type;
	char filename[100];
	struct node *pnext;
	int filesize;
}node,*pnode;
typedef struct 
{
	int fd;
	int signin;//是否登录成功,-1代表登录成功，其他代表失败
	int node;
	char username[100];
	char path[100];//登录用户的当前路径
}userdata,puserdata;
typedef struct 
{
	int len;
	char context[1000];
}msgtrain;
#endif

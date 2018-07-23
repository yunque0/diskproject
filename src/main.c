#include "thread.h"
#include "socket.h"
#include "epoll.h"
#include "mainfunc.h"
#include "verify.h"
int fds[2];
typedef struct 
{
	int msgtype;
	int msglen;
	char msg[1024];
}train;//消息结构体

void sigfunc(int signum)
{
	char a='a';
	write(fds[1],&a,1);
}
int main()
{
	char seraddr[]="192.168.5.111";
	int port=2001;
	int threadnum=5;
	int sfd,efd;
	fac f;
	factory_init(&f,threadnum,100,threadfunc);
	socket_init(&sfd,seraddr,port);
	//注册信号函数
	pipe(fds);
	signal(SIGINT,sigfunc);
	//创建epoll
	struct epoll_event evs[100];
	int readynum,listennum=2,newfdnum=0;
	userdata  user[50];//表示正在监听的user
	bzero(user,50*sizeof(userdata));
	efd=epoll_create(1);
	epoll_add(efd,sfd);
	epoll_add(efd,fds[0]);
	//开始循环epoll监听
	int i,j,newfd,datalen;
	char buf[50];
	train msg,recvmsg;
	//char password[50];
	bzero(&msg,sizeof(train));
	while(1)
	{
		readynum=epoll_wait(efd,evs,listennum,-1);
		for(i=0;i<readynum;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				printf("client connected\n");
				newfd=accept(sfd,NULL,NULL);
				epoll_add(efd,newfd);
				user[newfdnum].fd=newfd;
				newfdnum++;
				listennum++;
			}
			if(fds[0]==evs[i].data.fd)
			{
				printf("有序退出\n");
				pthread_mutex_lock(&(f.mutex));
				queue_insert(&(f.fd_queue),-1,1,"a",0);
				pthread_mutex_unlock(&(f.mutex));
				pthread_cond_broadcast(&(f.cond));
				for(i=0;i<threadnum;i++)
				{
					pthread_join(f.pth_id[i],NULL);
				}
				return 0;
			}
			for(j=0;j<newfdnum;j++)
			{
				if(user[j].fd==evs[i].data.fd)
				{
					bzero(buf,sizeof(buf));
					bzero(&recvmsg,sizeof(recvmsg));
					datalen=recv(user[j].fd,&recvmsg.msgtype,4,0);
					if(datalen==0)//对方断开连接
					{
						printf("client disconnected\n");
						epoll_del(efd,user[j].fd);
						bzero(user+j,sizeof(userdata));
						listennum--;
						newfdnum--;//待完善，这里得遍历数组
					}
					else
					{
					//	printf("buf=%s\n",buf);
	//					printf("msgtype=%d\n",recvmsg.msgtype);
						if(-1==user[j].signin)//已经成功登录
						{
							recv(user[j].fd,&recvmsg.msglen,4,0);
							recv(user[j].fd,buf,recvmsg.msglen,0);
	//						printf("command=%s\n",buf);
							mainfunc(buf,user[j].fd,user+j,&f);
						}
						else
						{
					//		printf("verify... \n");`
							recv(user[j].fd,&recvmsg.msglen,4,0);
							recv(user[j].fd,buf,recvmsg.msglen,0);
	//						printf("buf=%s\n",buf);
							if(1==recvmsg.msgtype)//发过来的是用户名信息
							{
								bzero(user[j].username,sizeof(user[j].username));
								strcpy(user[j].username,buf);
	//							printf("username=%s\n",user[j].username);
								send_salt(user[j].fd,user[j].username);//发送盐值给对方
								continue;
							}
							else if(2==recvmsg.msgtype)//发过来的是加密过的盐值
							{
								if(0==verify(user[j].username,buf))//验证用户名密码成功
								{
									
									//printf("username=%s\n",user[j].username);
									user[j].signin=-1;
									user[j].node=1;
									msg.msgtype=2;
									send(user[j].fd,&msg,4,0);
	//								printf("signin success\n");
								}					
								else
								{
									msg.msgtype=1;
									send(user[j].fd,&msg,4,0);
	//								printf("signin failed\n");
								}
							}
						}
					}
					//printf("receive a msg\n");
				}
			}
		}
	}
}














#include "thread.h"
void *threadfunc(void *p)
{
	int newfd;
	pfac pf=(pfac)p;
	char filename[100]={0};
	char temp[100]={0};
	int type;
	int filesize;
	while(1)
	{
		bzero(filename,sizeof(filename));
		pthread_mutex_lock(&(pf->mutex));
		if(0==pf->fd_queue.size)
		{
			pthread_cond_wait(&(pf->cond),&(pf->mutex));
		}
		newfd=queue_get(&(pf->fd_queue),&type,filename,&filesize);
		pthread_mutex_unlock(&(pf->mutex));
		if(-1==newfd)
		{
			printf("exit\n");
			pthread_exit(NULL);
		}
		if(type==1)//gets
		{
			//printf("filename=%s\n",filename);
			strcat(temp,"../file/");
			strcat(temp,filename);
			send_file(newfd,temp,filesize);
			printf("getsfile\n");
		}
		else
		{
			recv_file(newfd,filename);
		}
		printf("newfd=%d\n",newfd);
	}
}


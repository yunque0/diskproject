#include "factory.h"
int factory_init(pfac pf,int threadnum,int capacity,thread_func func)
{
	int i;
	pf->threadnum=threadnum;
	pf->fd_queue.phead=NULL;
	pf->fd_queue.ptail=NULL;
	pf->fd_queue.size=0;
	pf->fd_queue.capacity=capacity;
	pf->func=func;
	pthread_mutex_init(&(pf->mutex),NULL);
	pthread_cond_init(&(pf->cond),NULL);
	pf->pth_id=(pthread_t *)calloc(threadnum,sizeof(pthread_t));
	for(i=0;i<threadnum;i++)
	{
		pthread_create(pf->pth_id+i,NULL,func,pf);
	}
	return 0;
}

#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "queue.h"

typedef void *(*thread_func)(void *p);
typedef struct 
{
	pthread_t *pth_id;
	int threadnum;
	queue fd_queue;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	thread_func func;
}fac,*pfac;

int factory_init(pfac,int,int,thread_func );

#endif

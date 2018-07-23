#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "func.h"

typedef struct 
{
	pnode phead,ptail;
	int size;
	int capacity;
	//int filesize;
}queue,*pqueue;

int queue_insert(pqueue,int,int,char *,int);
int queue_get(pqueue pque,int *,char *,int *);

#endif

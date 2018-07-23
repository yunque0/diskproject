
#include "queue.h"

//队列结构体

int queue_insert(pqueue pque,int newfd,int type,char *filename,int filesize)//type=1代表puts,typed=0代表gets
{
	pnode pnew=(pnode)calloc(1,sizeof(node));
	pnew->data=newfd;
	pnew->type=type;
	pnew->filesize=filesize;
	strcpy(pnew->filename,filename);
	if(pque->phead==NULL)
	{
		pque->phead=pnew;
		pque->ptail=pnew;
		return 0;
	}
	else
	{
		if(-1==newfd)//头插法
		{
			pnew->pnext=pque->phead;
			pque->phead=pnew;
		}
		else//尾插法
		{
			pque->ptail->pnext=pnew;
			pque->ptail=pnew;
		}
	}
	return 0;
}
int queue_get(pqueue pque,int *ptype,char *filename,int *filesize)
{
	if(NULL==pque->phead)//代表队列为空，一般不可能
	{
		return 0;
	}
	else
	{
		if(-1==pque->phead->data)
		{
			return -1;
		}
		else
		{
			int newfd=pque->phead->data;
			*ptype=pque->phead->type;
			*filesize=pque->phead->filesize;
			strcpy(filename,pque->phead->filename);
			pnode ptemp=pque->phead;
			pque->phead=ptemp->pnext;
			free(ptemp);
			return newfd;
		}
	}
}









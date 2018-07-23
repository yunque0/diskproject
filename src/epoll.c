#include "epoll.h"

int epoll_add(int efd,int fd)
{
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	epoll_ctl(efd,EPOLL_CTL_ADD,fd,&event);
	return 0;
}

int epoll_del(int efd,int fd)
{
	struct epoll_event event;
	event.events=EPOLLIN;
	event.data.fd=fd;
	epoll_ctl(efd,EPOLL_CTL_DEL,fd,&event);
	return 0;
}

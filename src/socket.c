#include "socket.h"

int socket_init(int *sfd,char *seraddr,int port)
{
	int ret;
	*sfd=socket(AF_INET,SOCK_STREAM,0);
	int reuse=1;
	ret=setsockopt(*sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
	error_check(-1,ret,"setsockopt");
	struct sockaddr_in saddr;
	saddr.sin_port=htons(port);
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=inet_addr(seraddr);
	ret=bind(*sfd,(struct sockaddr*)&saddr,sizeof(struct sockaddr));
	error_check(-1,ret,"bind");
	ret=listen(*sfd,50);
	error_check(-1,ret,"listen");
	return 0;
}

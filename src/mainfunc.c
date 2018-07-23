#include "mainfunc.h"

int mainfunc(char *command,int fd,userdata *puser,pfac pf)
{
	int putsfd,getsfd,ret,filesize;
	char result[100];
	MYSQL *conn;
	conn=mysql_init(NULL);
	sql_init(conn);
	char temp[50];
	char openfile[100];
	//int node=4;
	strcpy(temp,command);
	char *token;//获取第一个字符串
	char *filename;//存放puts命令里的文件名
	int putsflag=0,getsflag=0;
	token = strtok(temp," ");
	char md5[100]={0};
	if(0==strcmp(token,"cd"))
	{
		bzero(result,sizeof(result));
		token=strtok(NULL," ");
		changedir(conn,puser->username,&(puser->node),token,result);
		printf("%s\n",result);
		send(fd,result,strlen(result),0);
	}
	else if(0==strcmp(token,"ls"))
	{
		//printf("ls\n");
		bzero(result,sizeof(result));
		listfile(conn,puser->username,puser->node,result);
		send(fd,result,strlen(result),0);
	}
	else if(0==strcmp(token,"gets"))
	{
		printf("command:gets\n");
		filename=strtok(NULL," ");
		bzero(md5,sizeof(md5));
		ret=sql_getmd5(conn,puser->username,puser->node,filename,md5);
		printf("getsret=%d\n",ret);
		printf("md5=%s\n",md5);
		if(-1==ret)
		{
			send(fd,&getsflag,4,0);
			return 0;
		}
		bzero(openfile,sizeof(openfile));	
		strcat(openfile,"../file/");
		strcat(openfile,md5);
		printf("getsfile=%s\n",openfile);
		getsfd=open(openfile,O_RDONLY);
		if(getsfd!=-1)//存在传送文件
		{
			token=strtok(NULL," ");
			filesize=atoi(token);
			getsflag=1;
			send(fd,&getsflag,4,0);
			pthread_mutex_lock(&(pf->mutex));
			queue_insert(&(pf->fd_queue),fd,1,md5,filesize);
			pthread_mutex_unlock(&(pf->mutex));
			pthread_cond_signal(&(pf->cond));
			return 0;
		}
		else
		{
			send(fd,&getsflag,4,0);
		}
	}
	else if(0==strcmp(token,"pwd"))
	{
		//printf("command:pwd\n");
		getpwd(conn,puser->username,puser->node,result);
		printf("%s\n",result);
		send(fd,result,strlen(result),0);
	}
	else if(0==strcmp(token,"mkdir"))
	{
		printf("commond:mkdir\n");
		token=strtok(NULL," ");
		if(sql_makedir(conn,puser->username,puser->node,token))
		{
			send(fd,"ok",2,0);
		}
		else
		{
			send(fd,"mkdir fail",10,0);
		}
	}
	else if(0==strcmp(token,"remove"))
	{
		printf("commond:remove\n");
		token=strtok(NULL," ");
		if(sql_remove(conn,"cr",puser->node,token))
		{
			send(fd,"ok",2,0);
		}
		else
		{
			send(fd,"remove fail",11,0);
		}
	}
	else if(0==strcmp(token,"puts"))
	{
		bzero(openfile,sizeof(openfile));
		printf("command:puts\n");
		filename=strtok(NULL," ");
		token=strtok(NULL," ");
		printf("md5sum=%s\n",token);
		strcat(openfile,"../file/");
		strcat(openfile,token);
		printf("%s\n",openfile);
		putsfd=open(openfile,O_RDONLY);
		if(putsfd==-1)//文件不存在传送文件
		{
			putsflag=1;
			send(fd,&putsflag,4,0);
			pthread_mutex_lock(&(pf->mutex));
			queue_insert(&(pf->fd_queue),fd,0,openfile,0);
			pthread_mutex_unlock(&(pf->mutex));
			pthread_cond_signal(&(pf->cond));
			sql_addfile(conn,puser->username,puser->node,filename,token,0);
			return 0;
		}
		else
		{
			printf("秒传\n");
		}
		close(putsfd);
		send(fd,&putsflag,4,0);
		sql_addfile(conn,puser->username,puser->node,filename,token,0);
	}
	else
	{
		send(fd,"error",5,0);
		return -1;
	}
	sql_addlog(conn,puser->username,command);
	return 0;
}

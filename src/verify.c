#include "verify.h"
#include "sql.h"
//检查密码是否正确
int verify(char *username,char *password)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	conn=mysql_init(NULL);
	sql_init(conn);
	char query[100];
	sprintf(query,"select * from user where username='%s' and password='%s'",username,password);
	t=mysql_query(conn,query);
	if(t)
	{
		printf("error query:%s\n",mysql_error(conn));
		return -1;
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			row=mysql_fetch_row(res);
			if(row==NULL)
			{
				mysql_free_result(res);
				mysql_close(conn);
				return -1;
			}
		}
	}
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}


int getusername(char *buf,char *username)
{
	bzero(username,sizeof(username));
	int i=0,j=0;
	while(buf[i]==' ')
	{
		i++;
	}
	for(;buf[i]!=' ' ;i++)
	{
		username[j++]=buf[i];
	}
	return 0;

}

int send_salt(int fd,char *username)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	 conn=mysql_init(NULL);
	sql_init(conn);
	char query[100];
	char salt[100]={0};
	sprintf(query,"select salt from user where username='%s'",username);
	t=mysql_query(conn,query);
	if(t)
	{
		printf("error query:%s\n",mysql_error(conn));
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			row=mysql_fetch_row(res);
			if(row==NULL)
			{
				send(fd,"aaa",3,0);
				mysql_free_result(res);
				mysql_close(conn);
				return -1;
			}
			strcpy(salt,row[0]);
			send(fd,salt,strlen(salt),0);
			mysql_free_result(res);
			mysql_close(conn);
		}
	}
	return 0;
}


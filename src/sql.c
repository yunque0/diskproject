#include "sql.h" 
int sql_init(MYSQL *conn)
{
	char *server="localhost";
	char *user="root";
	char *password="crmysql";
	char *database="disksys";
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
	return 0;
}
int sql_addlog(MYSQL *conn,char *username,char *command)
{
	int t;
	char query[100];
	sprintf(query,"insert into log(username,command) values('%s','%s')",username,command);
	t=mysql_query(conn,query);
	printf("%s\n",query);
	if(t)
	{
		printf("error query:%s\n",mysql_error(conn));
	return -1;
	}
	return 0;
}
int sql_getmd5(MYSQL *conn,char *username,int node,char *filename,char *md5)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	char query[100];
	sprintf(query,"select md5sum from filesystem where username='%s' and prenode='%d' and filetype='f'",username,node);
	t=mysql_query(conn,query);
	printf("%s\n",query);
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
				printf("row is null\n");
				mysql_free_result(res);
				return -1;
			}
			printf("strcpy\n");
			printf("%s\n",row[0]);
			strcpy(md5,row[0]);
			printf("strcpy end\n");
			//mysql_free_result(res);
		}
		mysql_free_result(res);
	}
	return 0;
}

int sql_addfile(MYSQL *conn,char *username,int node,char *filename,char *md5,int filesize)
{	
	int t;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int maxnode=getmaxnode(conn,username);	
	char query[300]={0};
	sprintf(query,"select * from filesystem where username='%s' and filename='%s' and prenode=%d",username,filename,node);
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))!=NULL)//文件已经存在
			{
				mysql_free_result(res);
				return 0;
			}
			else
			{
				mysql_free_result(res);
				sprintf(query,"insert into filesystem(prenode,node,username,filetype,filename,md5sum,filesize) values(%d,%d,'%s','f','%s','%s',%d)",node,maxnode+1,username,filename,md5,filesize);
				t=mysql_query(conn,query);
				if(t)
				{
					printf("Error making query:%s\n",mysql_error(conn));
				}
				else
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int getmaxnode(MYSQL *conn,char *username)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	int maxnode;
	char query[100];
	sprintf(query,"select max(node) from filesystem where username='%s'",username);
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
			maxnode=atoi(row[0]);
			mysql_free_result(res);
			return maxnode;
		}
		mysql_free_result(res);
	}
	return 0;
}

int listfile(MYSQL *conn,char *username,int node,char *result)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	char query[100];
	sprintf(query,"select filetype,filename,filesize from filesystem where username='%s' and prenode=%d",username,node);
	t=mysql_query(conn,query);
	//printf("listfile:query:%s\n",query);
	if(t)
	{
		printf("error query:%s\n",mysql_error(conn));
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{
				for(t=0;t<mysql_num_fields(res);t++)
				{
					//printf("%s  ",row[t]);
					strcat(result,row[t]);
					strcat(result," ");
				}
				//printf("\n");
				strcat(result,"\n");
			}
			result[strlen(result)-1]='\0';
		}
		mysql_free_result(res);
	}
	return 0;
}

int getpwd(MYSQL *conn,char *username,int node,char *pwd)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	char query[100];
	char path[100]="";
	char temp[100]={0};
	//printf("getpwd:node=%d\n",node);
	while(node!=0)
	{
		sprintf(query,"select prenode,filename from filesystem where username='%s' and node=%d",username,node);
		t=mysql_query(conn,query);
		//	printf("getpwd:query:%s\n",query);
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
				strcat(temp,row[1]);
				strcat(temp,"/");
				strcat(temp,path);
				strcpy(path,temp);
				bzero(temp,sizeof(temp));
				node=atoi(row[0]);
			}
			else
			{
				break;
			}
			mysql_free_result(res);
		}
	}
	//printf("cd:%s\n",path);
	strcpy(pwd,path);
	return 0;
}

int changedir(MYSQL *conn,char *username,int *pnode,char *dir,char *result)//dir是要cd的路径
{
	int node=*pnode;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int t;
	int temp;
	char query[100];
	if(strcmp(dir,"..")==0)
	{
	//	printf("..\n");
		sprintf(query,"select prenode from filesystem where username='%s' and node=%d",username,node);
		t=mysql_query(conn,query);
		//printf("cd:query:%s\n",query);
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
				if(row!=NULL&&(strcmp(row[0],"0")!=0))
				{
				printf("row[0]=%s\n",row[0]);
				temp=atoi(row[0]);
				mysql_free_result(res);
				getpwd(conn,username,temp,result);
	//			printf("changedir,result=%s\n",result);
				*pnode=temp;
				}
				else
				{
					strcpy(result,"it been root directory");
				}
			}
			else
			{
				//strcpy(result,"it has been root directory\n");
			}
		}
	}
	else
	{
		sprintf(query,"select node from filesystem where username='%s' and prenode=%d and filename='%s' and filetype='d'",username,node,dir);
		t=mysql_query(conn,query);
		if(t)
		{
			printf("cd error query:%s\n",mysql_error(conn));
		}
		else
		{
			res=mysql_use_result(conn);
			if(res)
			{
				if((row=mysql_fetch_row(res))!=NULL)
				{
				temp=atoi(row[0]);
				mysql_free_result(res);
				getpwd(conn,username,temp,result);
				*pnode=temp;
				}
				else
				{
					strcpy(result,"cd:no such directory\n");
				}
			}
			else
			{
				strcpy(result,"there are no such dir\n");
			}
		}
	}
	return 0;
}

int sql_makedir(MYSQL *conn,char *username,int node,char *dirname)
{
	int t;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int maxnode=getmaxnode(conn,"cr");	
	char query[300]={0};
	sprintf(query,"select * from filesystem where username='%s' and filename='%s'",username,dirname);
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))!=NULL)//目录已经存在
			{
				mysql_free_result(res);
				return 0;
			}
			else
			{
				mysql_free_result(res);
				sprintf(query,"insert into filesystem(prenode,node,username,filetype,filename,filesize) values(%d,%d,'%s','d','%s',0)",node,maxnode+1,username,dirname);
				t=mysql_query(conn,query);
				if(t)
				{
					printf("Error making query:%s\n",mysql_error(conn));
				}
				else
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

int sql_remove(MYSQL *conn,char *username,int node,char *filename)
{
	int t;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//int maxnode=getmaxnode(conn,"cr");	
	char query[300]={0};
	sprintf(query,"select * from filesystem where username='%s' and filename='%s'",username,filename);
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}
	else
	{
		res=mysql_use_result(conn);
		if(res)
		{
			if((row=mysql_fetch_row(res))==NULL)//文件不存在
			{
				mysql_free_result(res);
				return 0;
			}
			else
			{
				mysql_free_result(res);
				sprintf(query,"delete from  filesystem where username='%s' and prenode=%d and filename='%s'",username,node,filename);
				t=mysql_query(conn,query);
				if(t)
				{
					printf("Error making query:%s\n",mysql_error(conn));
				}
				else
				{
					return 1;
				}
			}
		}
	}
		return 0;
}










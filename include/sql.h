#ifndef __SQL_H__
#define __SQL_H__

#include "factory.h"

int sql_init(MYSQL *conn);
int getmaxnode(MYSQL *conn,char *username);
int listfile(MYSQL *,char *,int,char *);
int getpwd(MYSQL *conn,char *,int,char *);
int changedir(MYSQL *,char *,int *,char *,char *);//dir是要cd的路径
int sql_makedir(MYSQL *,char *,int,char *);
int sql_getmd5(MYSQL *,char *,int,char *,char *);
int sql_remove(MYSQL *,char *,int,char *);
int  sql_addfile(MYSQL *,char *,int,char *,char *,int);
int sql_addlog(MYSQL *,char *,char *);
#endif



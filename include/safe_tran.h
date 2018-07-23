#ifndef __SAFE_TRAN_H__
#define __SAFE_TRAN_H__
#include "factory.h"

int send_s(int,char *,int);
int recv_s(int,char *,int);
int recv_file(int,char *);
int send_file(int ,char *,int);
#endif

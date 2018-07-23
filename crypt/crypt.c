#include <unistd.h>
#include <stdio.h>  
#include <stdlib.h>  
#define _XOPEN_SOURCE  
char *crypt(const char *key, const char *salt); 

int main(void)  
{  
	char *passwd;  
	char key[] = "123";  
	passwd = crypt(key, "abcdef");  
	printf("password: %s\n", passwd);  
	return 0;  
}  	

#include <stdio.h> 
#include <fcntl.h>   
#include<limits.h>
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<errno.h>
#include<sys/stat.h> 
#include<sys/types.h> 
#include<sys/wait.h>
#include<dirent.h>
#include<stdbool.h>

char host[1030], user[1030], cwd[2050], myhome[2050];
typedef struct bgTask{
	char name[200];
	pid_t pid;
}bgTask;
bgTask bgP[200];
int run;
int fgPid;
int bgCnt;
char fgName[260];
#include "prompt.h"
#include "headers.h"

extern int errno;

void welc(){
printf("\033[1;96m\t       __   ___      ____ _    __  _____  _____ __  __\n");
  printf("\t      / /  /   |    / __ \\ |  / / /_  _/ / ___// / / /\n");
  printf("\t __  / /  / /| |   / /_/ / | / /   / /   \\__ \\/ /_/ / \n");
  printf("\t/ /_/ /_ / ___ |_ / _, _/| |/ /_ _/ / _ ___/ / __  /_ \n");
 printf("\t\\____/(_)_/  |_(_)_/ |_(_)___/(_)___/(_)____/_/ /_/(_)\n");
  printf("                                                      \n\033[0m");
}

void prompt() {
//	welc();
	char *promptLine,*dir;

	dir=(char *)malloc(2000);
	if(getcwd(cwd,2000)==NULL){
		perror("Get current working directory ERROR");
		exitCode = -1;
		exit(1);
	}
	strcpy(dir,cwd);
//	printf("%d\n", exitCode);
	if(exitCode < 0){
		printf(":'(");
	}
	else if(exitCode>0)
		printf(":')");

	if(strcmp(myhome,dir)==0){
		printf("\033[1;96m<%s@%s:~>\033[0m",user,host);
	}
	else if(strlen(myhome)<strlen(dir) && strstr(dir,myhome)!=NULL){
		dir+=strlen(myhome);
		printf("\033[1;96m<%s@%s:~%s>\033[0m",user,host,dir);
		dir-=strlen(myhome);
	}
	else{	
		printf("\033[1;96m<%s@%s:%s>\033[0m",user,host,dir);	
	}
	free(dir);
//	printf("%s@%s:~",user,host);
	fflush(stdout);
}

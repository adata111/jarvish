#include "headers.h"
#include "command.h"

void cd(char *newDir){
	if(strcmp(newDir,"-")==0){
		char shortP[2000];
		if(strstr(prevwd,myhome)==NULL)
			strcpy(shortP,prevwd);
		else{
			strcpy(shortP,"~");
			strcat(shortP,(prevwd+strlen(myhome)));
		}
		printf("%s\n",shortP);
		
		if(chdir(prevwd)==0){
			strcpy(prevwd,cwd);
			exitCode = 5;
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stderr);
			perror(shortP);
			exitCode = -1;
		}
	}
	else if(strcmp(newDir,"~")==0){
		if(chdir(myhome)==0){
			strcpy(prevwd,cwd);
			exitCode = 5;
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stdout);
			perror(newDir);
			exitCode = -1;
		}
	}
	else if(newDir[0]=='~'){
		char fullP[3000];
		strcpy(fullP,myhome);
		strcat(fullP,(newDir+1));
		if(chdir(fullP)==0){
			strcpy(prevwd,cwd);
			exitCode = 5;
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stderr);
			perror(fullP);
			exitCode = -1;
		}
	}
	else{
		if(chdir(newDir)==0){
			strcpy(prevwd,cwd);
			exitCode = 5;
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stderr);
			perror(newDir);
			exitCode = -1;
		}
	}
	if(getcwd(cwd,2000)==NULL){
		perror("getcwd");
		exit(1);
	}
	return;
}
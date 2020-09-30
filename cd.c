#include "headers.h"
#include "command.h"

void cd(char *newDir){
	char *token;//=strtok(newDir,"\n");
//	strcpy(token,newDir);
//	printf("%s\n",newDir);
	if(strcmp(newDir,"~")==0){
		if(chdir(myhome)==0){
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stdout);
			perror(newDir);
		}
	}
	else if(newDir[0]=='~'){
		char fullP[3000];
	//	printf("hi\n");
		strcpy(fullP,myhome);
		strcat(fullP,(newDir+1));
		if(chdir(fullP)==0){
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stdout);
			perror(fullP);
		}
	}
	else{
		if(chdir(newDir)==0){
		}
		else{
			fprintf(stderr,"Jarvish: cd: ");
			fflush(stdout);
			perror(newDir);
		}
	}
}
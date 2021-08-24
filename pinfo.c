#include "headers.h"

void exeStat(pid_t pid){
	char *exeFile = malloc(50 * sizeof(char));
	sprintf(exeFile,"/proc/%d/exe",pid);
	char exeP[3000];
	int ret = readlink(exeFile, exeP,2000);
	if(ret<0){
		printf("Executable Path not found\n");
		return;
	}
	exeP[ret]='\0';
	char *convertP = strstr(exeP, myhome);
	if(convertP==NULL)
		printf("Executable Path -- %s\n", exeP);
	else{
		convertP+=strlen(myhome);
		printf("Executable Path -- ~%s\n", convertP);
	}
	free(exeFile);
}

void readStat(pid_t pid){
	char *statFile;
	statFile = malloc(50 * sizeof(char));
	sprintf(statFile,"/proc/%d/status",pid);
	FILE *stream = fopen(statFile, "r");
	if(stream == NULL){
		fprintf(stderr,"jarvish: pinfo: No process with ID %d exists\n",pid);
		exitCode = -1;
		free(statFile);
		return;
	}
	else{
		char *data = NULL;
		long len=0;
		long r = getline(&data,&len,stream);
		long lineNo=0;
		printf("pid -- %d\n",pid);
		while(r>0){
			lineNo++;
			if(lineNo==3){
				char *token = strtok(data," \t\n");
				token = strtok(NULL," \n");
				printf("Process Status -- %s\n",token);
			}
			else if(lineNo==18){
				char *token = strtok(data," \t\n");
				token = strtok(NULL," \n");
				printf("memory -- %s\n",token);
				break;
			}
			r = getline(&data,&len,stream);
		}
		free(data);
	}

	free(statFile);
	fclose(stream);
	exeStat(pid);
}

void pinfo(char *tokens[200], int tokenCnt){
	pid_t pid;
	if(tokenCnt == 1){
		pid = getpid();
	}
	else if(tokenCnt==2){
		pid = atoi(tokens[1]);
	}
	else{
		write(2,"jarvish: pinfo: Too many arguments\n",50);
		exitCode = -1;
		return;
	}

	readStat(pid);

}
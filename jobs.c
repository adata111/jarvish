#include "headers.h"

void getStat(pid_t pid){
	char *statFile;
	statFile = malloc(50 * sizeof(char));
	sprintf(statFile,"/proc/%d/status",pid);
	FILE *stream = fopen(statFile, "r");
	if(stream == NULL){
		fprintf(stderr,"No process with ID %d exists",pid);
		exitCode = -1;
		return;
	}
	else{
		char *data = NULL;
		long len=0;
		long r = getline(&data,&len,stream);
		long lineNo=0;
		while(r>0){
			lineNo++;
			if(lineNo==3){
				char *token = strtok(data," \t\n");
				token = strtok(NULL," \n");
				if(!strcmp(token,"R") || !strcmp(token,"S") || !strcmp(token,"D")){
					printf("Running");
				}
				else{
					printf("Stopped");
				}
				break;
			}
			r = getline(&data,&len,stream);
		}
		free(data);
	}
	free(statFile);
	fclose(stream);
}

void jobs(){
	for(int i=0;i<bgCnt;i++){
		printf("[%d] ", (i+1));
		getStat(bgP[i].pid);
		printf(" %s [%d]\n", bgP[i].name, bgP[i].pid);
	}
}
#include "headers.h"
#include "history.h"

void saveHist(){
	char *histP = malloc(500 * sizeof(char));
	sprintf(histP,"%s/history.txt",myhome);
	FILE *stream = fopen(histP,"w");
		int i;
	if(stream == NULL){
		perror("Jarvish: history");
		exitCode = -1;
		return;
	}
	else{
		fprintf(stream, "%d\n", histNo);
		for(i=0;i<histNo;i++){
			fprintf(stream, "%s", histArr[i]);
		}
		fclose(stream);
	}
	free(histP);
	return;
	
}

void addToHistArr(char *comm){
	int i=0;
	char commCopy[2000];
	strcpy(commCopy, comm);
	char *com = strtok(comm, " \r\t\n");
	if(com == NULL) return;
	if(histNo>0){
		if(strcmp(commCopy,histArr[histNo-1])==0){ 
			return;
		}
	}
	if(histNo ==20){
		for(i=0;i<19;i++){
			strcpy(histArr[i],histArr[i+1]);
		}
		strcpy(histArr[i],commCopy);
		histNo = 20;
	}
	else{
		strcpy(histArr[histNo],commCopy);
		histNo++;
	}
}

void history(char *com, char *tokens[200], int tokenCnt){
	if(tokenCnt>2){
		fprintf(stderr,"Jarvish: history: Too many arguments");
		exitCode = -1;
		return;
	}
	int readNo;
	if((tokenCnt==2 && atoi(tokens[1])>=20)) readNo = 20;
	else if(tokenCnt == 1) readNo = 10;
	else readNo = atoi(tokens[1]);
	if(readNo>(histNo+1)) readNo=histNo+1;
	for(int i=(histNo-readNo);i<histNo;i++){
		printf("%s",histArr[i]);
	}
	
}

void loadHist(){
	char *histP = malloc(500 * sizeof(char));
	
	sprintf(histP,"%s/history.txt",myhome);
	FILE *stream = fopen(histP,"r");
	if(stream == NULL){
		if(errno==ENOENT){
			histNo=0;
			return;
		}
		exitCode = -1;
		perror("Jarvish: history");
		return;
	}
	else{
		char *data = NULL;
		long len=0;
		long r = getline(&data,&len,stream);
		
		if(r>0){
			char *token = strtok(data," \t\n");
			int lines = atoi(token), lineNo=0;
			histNo = lines;
			while(r>0){
				lineNo++;
				if(lineNo>1)
					strcpy(histArr[lineNo-2], data);
				r = getline(&data,&len,stream);
			}
			fclose(stream);
		}
		free(data);
	}
	free(histP);
	return;
}
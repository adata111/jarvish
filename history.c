#include "headers.h"
#include "history.h"

void saveHist(){
	char *histP = malloc(500 * sizeof(char));
	sprintf(histP,"%s/history.txt",myhome);
	FILE *stream = fopen(histP,"w");
		int i;
	if(stream == NULL){
		perror("Jarvish: history");
		return;
	}
	else{
		/*char *data = NULL;
		long len=0;
		int i, lines;
		long r = getline(&data,&len,stream);
		printf("%s\n",data);
		char *token = strtok(data," \t\n");
		lines = atoi(token);
		if(histNo >=20){
			fseek(stream,0,SEEK_SET);
			*/
			fprintf(stream, "%d\n", histNo);
			for(i=0;i<histNo;i++){
			//	printf("ii%s", histArr[i]);
				fprintf(stream, "%s", histArr[i]);
			}
		/*}
		else{
			printf("hello\n");
			char *buff[20];int lineNo=0, startFrom;
			for(i=0;i<20;i++){
				buff[i] = (char *)malloc(200 * sizeof(char));
			}
			if(histNo+lines>=20) startFrom = 2+lines+histNo-20;
			else startFrom=2;
			while(r>0){
				lineNo++;
				if(lineNo >= startFrom)
					strcpy(buff[lineNo-startFrom], data);
				r = getline(&data,&len,stream);
			}
			printf("h\n");
		//	fseek(stream,0,SEEK_SET);
			FILE *ptr = fopen(histP,"w");
			fprintf(ptr, "%d\n", (lines+histNo >=20)?20:(lines+histNo));
			for(i=0;i<=(lineNo-startFrom);i++){
				fprintf(ptr, "%s", buff[i]);
			}
			printf("hi\n");
			for(i=0;i<histNo;i++){
				printf("%s",histArr[i]);
				fprintf(ptr, "%s", histArr[i]);
			}
			fclose(ptr);
			printf("hil\n");
			for(i=0;i<20;i++){
				free(buff[i]);
			}
		}*/
	//	printf("\nhell\n");
		fclose(stream);
	//	printf("hello\n");
	}
	free(histP);
	/*for(i=0;i<histNo;i++){
	//	printf("ii%s", histArr[i]);
		free(histArr[i]);
	}*/
}

void addToHistArr(char *comm){
	int i=0;
	char commCopy[2000];
	strcpy(commCopy, comm);
	char *com = strtok(comm, " \r\t\n");
//	printf("%s\n", com);
	if(com == NULL) return;
//	printf("hi\n");
	if(histNo>0){
		if(strcmp(commCopy,histArr[histNo-1])==0){ 
//			printf("hello\n");
			return;
		}
	}
	if(histNo ==20){
//		printf("histNo=25");
		for(i=0;i<19;i++){
			strcpy(histArr[i],histArr[i+1]);
		}
		strcpy(histArr[i],commCopy);
		histNo = 20;
//		printf("ok\n");
	}
	else{
//		printf("histNo<25\n");
		strcpy(histArr[histNo],commCopy);
		histNo++;
	}
	//printf("%d\n",histNo);
}

void history(char *com, char *tokens[200], int tokenCnt){
	if(tokenCnt>2){
		fprintf(stderr,"Jarvish: history: Too many arguments");
		return;
	}
	int readNo;
//	printf("%s",tokens[1]);
	if((tokenCnt==2 && atoi(tokens[1])>=20)) readNo = 20;
	else if(tokenCnt == 1) readNo = 10;
	else readNo = atoi(tokens[1]);
	if(readNo>(histNo+1)) readNo=histNo+1;
//	readNo--;
//	printf("%d %d\n",histNo,readNo);
	for(int i=(histNo-readNo);i<histNo;i++){
		printf("%s",histArr[i]);
	}
	/*char *histP = malloc(500 * sizeof(char));
	sprintf(histP,"%s/history.txt",myhome);
	FILE *stream = fopen(histP,"r");
	if(stream == NULL){
		perror("Jarvish: history");
		return;
	}
	else{
		char *data = NULL;
		long len=0;
		long r = getline(&data,&len,stream);
		long lineNo=0; int lines=0,startFrom=2;
		while(r>0){
			lineNo++;
			if(lineNo == 1){
				char *token = strtok(data," \t\n");
				lines = atoi(token);
				if((lines+1) <= readNo){
					startFrom = 2;
				}
				else 
					startFrom = lines-readNo+2;
			}
			else if(lineNo>=startFrom){
				printf("%s", data);
			}
			r = getline(&data,&len,stream);
		}
		printf("%s\n", com);
	}
	fclose(stream);	
	*/
}

void loadHist(){
	char *histP = malloc(500 * sizeof(char));
	
	sprintf(histP,"%s/history.txt",myhome);
	FILE *stream = fopen(histP,"r");
	if(stream == NULL){
		if(errno==ENOENT){
		//printf("hi\n");
			histNo=0;
			return;
		}
		perror("Jarvish: history");
		return;
	}
	else{
		char *data = NULL;
		long len=0;
	//	printf("hi\n");
		long r = getline(&data,&len,stream);
	//	printf("hello\n");
		
		if(r>0){
			char *token = strtok(data," \t\n");
			int lines = atoi(token), lineNo=0;
			histNo = lines;
			while(r>0){
				lineNo++;
				if(lineNo>1)
					strcpy(histArr[lineNo-2], data);
			//	printf("data %s\n",data);
				r = getline(&data,&len,stream);
			}
			fclose(stream);
		}
		free(data);
	}
	free(histP);
	return;
}
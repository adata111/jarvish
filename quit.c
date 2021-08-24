#include "headers.h"
#include "history.h"

void quit(){
	if(!freed) saveHist();
	
	for(int i=0;i<bgCnt;i++){
		kill(bgP[i].pid, SIGKILL);
	}
	bgCnt=0;
	for(int i=0;i<30 && !freed;i++){
		free(histArr[i]);
	}
	freed=1;
	printf("Bye!\n");
	return;
}

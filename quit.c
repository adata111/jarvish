#include "headers.h"
#include "history.h"

void quit(){
	saveHist();
	
	for(int i=0;i<bgCnt;i++){
		kill(bgP[i].pid, SIGKILL);
	}
	for(int i=0;i<30;i++){
		free(histArr[i]);
	}
	printf("Bye!\n");
//	return;
	exit(0);
}

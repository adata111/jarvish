#include "headers.h"

void overkill(){
	for(int i=0;i<bgCnt;i++){
		kill(bgP[i].pid, SIGKILL);
	//	printf("\nhi\n");
	}
//	bgCnt=0;
}
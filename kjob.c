#include "headers.h"

void kjob(int jobnum, int signum){
	if(jobnum>bgCnt || jobnum<1){
		fprintf(stderr,"Jarvish: kjob: No job with the given number exists.\n");
	}
	else if(signum > 31 || signum<1){
		fprintf(stderr,"Jarvish: kjob: Incorrect signal number provided\n");

	}
	else{
		kill(bgP[jobnum-1].pid,signum);
	}
}
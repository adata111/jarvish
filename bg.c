#include "headers.h"

void bg(int jobnum){
	if(jobnum>bgCnt || jobnum<1){
		fprintf(stderr,"Jarvish: bg: No job with the given number exists.\n");
		exitCode = -1;
	}
	else {
			if(kill(bgP[jobnum-1].pid,SIGCONT)<0)
				exitCode=-1;
			else exitCode = 5;
		}
}
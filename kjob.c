#include "headers.h"

void kjob(int jobnum, int signum){
	if(jobnum>bgCnt || jobnum<1){	//counting starts from 1
		fprintf(stderr,"Jarvish: kjob: No job with the given number exists.\n");
		exitCode = -1;
	}
	else if(signum > 31 || signum<1){	// signal number 0 is SIGNULL used to check access to pid
		// other than that, signal number can't be more than 31
		fprintf(stderr,"Jarvish: kjob: Incorrect signal number provided\n");
		exitCode = -1;
	}
	else{
		if(kill(bgP[jobnum-1].pid,signum)<0){	//if kill returns less than 0 value, it means error
			perror("Jarvish: error");
			exitCode = -1;
		}
		// if no error while killing, kill was successful and our task is done
	}
}
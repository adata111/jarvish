#include "headers.h"
#include "command.h"

void fg(int jobnum){
	if(jobnum>bgCnt || jobnum<1){
		fprintf(stderr,"Jarvish: fg: No job with the given number exists.\n");
		exitCode = -1;
	}
	else {
		signal (SIGCHLD, childHandler);
		/* transfer controlling terminal */
			if ( tcsetpgrp (STDIN_FILENO, getpgid(bgP[jobnum-1].pid)) < 0) { /* we also do this in child */
			//	perror("tcsetpgrp 1");
			//	exit(1);
			}
			int status;
			if(kill(bgP[jobnum-1].pid,SIGCONT)<0){
				perror("kill");
				exitCode = -2;
				return;
			}
			fgT.pid=bgP[jobnum-1].pid;
			strcpy(fgT.name,bgP[jobnum-1].name);
			strcpy(fgT.fullN,bgP[jobnum-1].fullN);
			char *proc = delBg(fgT.pid);

			int wpid = waitpid(fgT.pid, &status, WUNTRACED);
			free(proc);
			/* Children completed: put the shell back in the foreground.  */
			if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
			//	perror("tcsetpgrp 2");
			//	exit(1);
			}
			if(WIFEXITED(status)){
	        	if(WEXITSTATUS(status) == 0){
	        	//	printf("normal exit\n");
	        		exitCode = 5;
	        	}
	     
	        	else {
	        	//	printf("not normal exit stat\n");
	        		exitCode = -2;
	        	}
	        }
	        else{
	        	//printf("not normal exit\n");
	        	exitCode = -2;
	        }
			if( WIFSTOPPED(status) ){
				strcpy(bgP[bgCnt].name, fgT.name);
				strcpy(bgP[bgCnt].fullN, fgT.fullN);
				bgP[bgCnt].pid = fgT.pid;
				bgCnt++;
				printf("\033[0;91m\n%s with pid %d stopped and sent to background\n\033[0m",fgT.name,fgT.pid);
				exitCode = -1;
			}
	}
}
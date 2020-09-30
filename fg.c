#include "headers.h"
#include "command.h"

void fg(int jobnum){
	if(jobnum>bgCnt || jobnum<1){
		fprintf(stderr,"Jarvish: fg: No job with the given number exists.\n");
	}
	else {
		signal (SIGCHLD, childHandler);
		/* transfer controlling terminal */
			if ( tcsetpgrp (STDIN_FILENO, getpgid(bgP[jobnum-1].pid)) < 0) { /* we also do this in child */
			//	perror("tcsetpgrp 1");
			//	exit(1);
			}
			int status;
			kill(bgP[jobnum-1].pid,SIGCONT);
			fgPid=bgP[jobnum-1].pid;
			strcpy(fgName,bgP[jobnum-1].name);
			char *proc = delBg(fgPid);

			int wpid = waitpid(fgPid, &status, WUNTRACED);
			free(proc);
			/* Children completed: put the shell back in the foreground.  */
			if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
			//	perror("tcsetpgrp 2");
			//	exit(1);
			}
			if( WIFSTOPPED(status) ){
				strcpy(bgP[bgCnt].name, fgName);
				bgP[bgCnt].pid = fgPid;
				bgCnt++;
				printf("\033[0;91m\n%s with pid %d stopped and sent to background\n\033[0m",fgName,fgPid);
			}
	}
}
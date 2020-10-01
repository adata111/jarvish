#include "headers.h"
#include "command.h"
#include "prompt.h"

int is_bg = 0;

char *delBg(pid_t pid){
	char *proc;
	proc = (char *)malloc(200 * sizeof(char)); 
        for(int i=0; i < bgCnt; i++)
        {
            if(bgP[i].pid == pid)
            {

                strcpy(proc, bgP[i].name);
                for(int j=i; j < bgCnt-1; j++)
			    {
			         strcpy(bgP[i].name, bgP[i+1].name);
			         strcpy(bgP[i].fullN, bgP[i+1].fullN);
			         bgP[i].pid = bgP[i+1].pid;
			    }
     
    			bgCnt-=1;    
                break;
            }
        }
    return proc;
}

void childHandler(int sig){
	int childStatus;
    pid_t pid = waitpid(-1, &childStatus, WNOHANG);
//    printf("process %d exited normally\n", pid);
    if(pid > 0)
    {
        char * proc = delBg(pid);
    //   printf("process %d exited normally %d\n", pid,childStatus);
        if(WIFEXITED(childStatus)){
        	if(WEXITSTATUS(childStatus) == 0){
        		fprintf(stderr,"\033[0;91m\n%s with pid %d exited normally\n\033[0m", proc, pid);
        		exitCode = 5;
        		fflush(stdout);
    		//	prompt();
        	}
     
        	else {
        		fprintf(stderr,"\033[0;91m\n%s with pid %d exited with exit status %d\n\033[0m",proc, pid, childStatus);
        		exitCode = -2;
        	}
        	fflush(stdout);
        	if(1) prompt();
        }
        else{
        	fprintf(stderr,"\033[0;91m\n%s with pid %d failed to exit normally. Status: %d\n\033[0m",proc, pid, childStatus);
        	fflush(stdout);
        	exitCode = -2;
    		prompt();
        }
        free(proc);

    }
	//if(bg) prompt();
    return;
}

void executeSys(char *tokens[200], int tokenCnt, int backgnd){
	is_bg=backgnd;
	 signal (SIGQUIT, SIG_IGN);
           signal (SIGTSTP, SIG_IGN);
           signal (SIGTTIN, SIG_IGN);
           signal (SIGTTOU, SIG_IGN);
//	printf("pgrp=%d pid=%d term=%d\n", getpgrp(), getpid(), tcgetpgrp(STDIN_FILENO));
	if(!backgnd)
		tokens[tokenCnt]=NULL;
	else
		tokens[tokenCnt-1]=NULL;
	
	int forkRet = fork();
	if(forkRet<0){
		perror("Jarvish: fork");
		exitCode = -2;
	}
	else if(forkRet == 0){
		//kiddo
		signal (SIGQUIT, SIG_DFL);
           signal (SIGTSTP, SIG_DFL);
           signal (SIGTTIN, SIG_DFL);
           signal (SIGTTOU, SIG_DFL);
           signal (SIGINT, SIG_DFL);
		if(backgnd) 
			setpgid(0,0);
			if(!backgnd){
			/* transfer controlling terminal */
		if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
		//	perror("child tcsetpgrp");
		//	exit(1);
			//bash checks if input is coming from terminal and hence throws error in case of piping
		}
	}
	//	printf("pgrp=%d pid=%d term=%d\n", getpgrp(), getpid(), tcgetpgrp(STDIN_FILENO));
		if(execvp(tokens[0],tokens)==-1){
			fprintf(stderr,"Jarvish: %s: Oops! Command not found :(\n",tokens[0]);
			freeToks();
			exitCode = -2;
			exit(1);
		//	return;
		}

	}	
	else{
		//parent
		//setpgid(forkRet, forkRet);
		signal (SIGCHLD, childHandler);
		int status;

		if(!backgnd){
	//	wait(NULL);
			/* transfer controlling terminal */
			if ( tcsetpgrp (STDIN_FILENO, getpgid(forkRet)) < 0) { // we also do this in child 
			//	perror("tcsetpgrp 1");
			//	exit(1);
			}
			fgT.pid=forkRet;
			strcpy(fgT.name,tokens[0]);
			strcpy(fgT.fullN, tokens[0]);
			for(int y = 1;y<tokenCnt ; y++){
				strcat(fgT.fullN, " ");
				strcat(fgT.fullN, tokens[y]);
			}
			int wpid = waitpid(forkRet, &status, WUNTRACED);

			/* Children completed: put the shell back in the foreground.  */
			if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
			//	perror("tcsetpgrp 2");
			//	exit(1);
			}
			//if(status != 0) exitCode= -2;
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
				strcpy(bgP[bgCnt].name, tokens[0]);
				bgP[bgCnt].pid = forkRet;
				strcpy(bgP[bgCnt].fullN, tokens[0]);
				for(int y = 1;y<tokenCnt ; y++){
					strcat(bgP[bgCnt].fullN, " ");
					strcat(bgP[bgCnt].fullN, tokens[y]);
				}
				bgCnt++;
				printf("\033[0;91m\nChild process %s with process id %d stopped and sent to background\n\033[0m",fgT.name,fgT.pid);
				exitCode = -2;
			}

		}
		else{
			strcpy(bgP[bgCnt].name, tokens[0]);
			bgP[bgCnt].pid = forkRet;
			strcpy(bgP[bgCnt].fullN, tokens[0]);
		//	printf("%d\n", tokenCnt);
			for(int y = 1;y<tokenCnt-1 ; y++){
				strcat(bgP[bgCnt].fullN, " ");
				strcat(bgP[bgCnt].fullN, tokens[y]);
			}
			bgCnt++;
			printf("[%d] %d\n", bgCnt, forkRet);

			if(strcmp(tokens[0],"vi")==0 || !(strcmp(tokens[0],"vim"))){
				kill(forkRet, SIGSTOP);
			}
	/*		if (WIFEXITED(status) != 0) {
      printf("process %d exited normally\n", forkRet);
      printf("exit status from child is %d\n", WEXITSTATUS(status));
   } else {
      printf("process %d not exited normally\n", forkRet);
   }*/
		}   
	}
	if(exitCode >= 0 ){
		exitCode = 5;
	}
}
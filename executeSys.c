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
    pid_t pid = waitpid(-1, &childStatus, WNOHANG);		// waitpid waits for any child process to end
	/* WNOHANG
    Demands status information immediately. If status information is immediately available on an appropriate child process, 
	waitpid() returns this information. Otherwise, waitpid() returns immediately with an error code indicating that 
	the information was not available. In other words, WNOHANG checks child processes without causing the caller to be suspended.
	*/

    if(pid > 0)
    {
        char * proc = delBg(pid);
        if(WIFEXITED(childStatus)){		//if child exited normally
        	if(WEXITSTATUS(childStatus) == 0){
        		fprintf(stderr,"\033[0;91m\n%s with pid %d exited normally\n\033[0m", proc, pid);
        		exitCode = 5;
        		fflush(stdout);
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
    return;
}

void executeSys(char *tokens[200], int tokenCnt, int backgnd){
	is_bg=backgnd;
	signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
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
			/* transfer controlling terminal 
			stdin is assigned same process group id as this child */
			if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
			}
		}
		if(execvp(tokens[0],tokens)==-1){
			fprintf(stderr,"Jarvish: %s: Oops! Command not found :(\n",tokens[0]);
			freeToks();
			exitCode = -2;
			exit(1);
		}

	}	
	else{
		//parent
		signal (SIGCHLD, childHandler);		//when parent gets to know child died, execute childHandler
		int status;

		if(!backgnd){
			/* transfer controlling terminal
			stdin is assigned the same process group id as the child */
			if ( tcsetpgrp (STDIN_FILENO, getpgid(forkRet)) < 0) { // we also do this in child 
			
			}
			fgT.pid=forkRet;	// child is the fg process now
			strcpy(fgT.name,tokens[0]);
			strcpy(fgT.fullN, tokens[0]);
			for(int y = 1;y<tokenCnt ; y++){
				strcat(fgT.fullN, " ");
				strcat(fgT.fullN, tokens[y]);
			}
			int wpid = waitpid(forkRet, &status, WUNTRACED);	// wait for child to get over
			// WUNTRACED - Reports on stopped child processes as well as terminated ones. 

			/* Children completed: put the shell back in the foreground.  */
			if( tcsetpgrp (STDIN_FILENO, getpgrp()) < 0) {
			}

			// check the exit status of child to decide the exitCode
			if(WIFEXITED(status)){
	        	if(WEXITSTATUS(status) == 0){
	        		exitCode = 5;
	        	}
	     
	        	else {
	        		exitCode = -2;
	        	}
	        }
	        else{
	        	exitCode = -2;
	        }
			if( WIFSTOPPED(status) ){	// ctrl Z
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
			// background process
			strcpy(bgP[bgCnt].name, tokens[0]);
			bgP[bgCnt].pid = forkRet;
			strcpy(bgP[bgCnt].fullN, tokens[0]);
			for(int y = 1;y<tokenCnt-1 ; y++){
				strcat(bgP[bgCnt].fullN, " ");
				strcat(bgP[bgCnt].fullN, tokens[y]);
			}
			bgCnt++;
			printf("[%d] %d\n", bgCnt, forkRet);

			if(strcmp(tokens[0],"vi")==0 || !(strcmp(tokens[0],"vim"))){ // vi & does something weird 
				// add however u want to handle this
			}
		}   
	}
	if(exitCode >= 0 ){
		exitCode = 5;
	}
}
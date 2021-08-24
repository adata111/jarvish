#include "headers.h"
#include "command.h"


int pip(char *comm){
	char comCopy[2000];
	int x;
	strcpy(comCopy,comm); 
	char *pipToks[100];
	for(x=0;x<100;x++){
		pipToks[x] = (char *)calloc(2000, sizeof(char));
	}
	int pipCnt=0;
	char *pipTok= strtok(comm,"|");
	while(pipTok!=NULL){
		strcpy(pipToks[pipCnt],pipTok);
		pipCnt++;
		pipTok = strtok(NULL,"|");
	}
	int fd[2];
	pid_t childPid;
	int s;
	x=1;
	for(int i=0;i<pipCnt;i++){
		if(pipe(fd)<0){
			perror("Pipe");
			exitCode = -1;
			break;
		}
		else{
			exitCode = 5;
		}
		int forkRet = fork();
		if(forkRet<0){
			exitCode = -1;
			perror("Pipe fork");
			break;
		}
		else if(forkRet>0){
			//parent - write to pipe
			wait(NULL);

			close(fd[1]);	//close write end
			s=fd[0];
		}
		else if(forkRet == 0){
			//child - read from pipe
			if(i!=0)
			if(dup2(s,0)<0){	//replace stdin of pipe rhs with read end of lhs
				perror("dup2 stdin");
				exitCode = -1;
				close(fd[1]);
				close(fd[0]);
				exit(1);
			}
			
			if(i<pipCnt-1){
				if(dup2(fd[1],1)<0){ //replace stdout of lhs with write end of rhs
					perror("dup2 stdin");
					exitCode = -1;
					close(fd[1]);
					close(fd[0]);
					exit(1);
				}
			}
			if(!chkRedir(pipToks[i]))//input from fd[0]
			 x=0;
			close(fd[0]);	//close read end
			close(fd[1]);	//close write end
			exit(1);
		}
	}
	for(s=0;s<100;s++){
		free(pipToks[s]);
	}
	return x;
}
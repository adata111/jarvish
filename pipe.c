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
	//	printf("%d %s\n",pipCnt,pipTok);
		strcpy(pipToks[pipCnt],pipTok);
		pipCnt++;
		pipTok = strtok(NULL,"|");
	}
	int fd[2];
	pid_t childPid;
	int s;
//	backup_in=dup(STDIN_FILENO);
	for(int i=0;i<pipCnt;i++){
		if(pipe(fd)<0){
			perror("Pipe");
			exitCode = -1;
			exit(1);
		}
		else{
			exitCode = 5;
		}
		int forkRet = fork();
		if(forkRet<0){
			exitCode = -1;
			perror("Pipe fork");
			exit(1);
		}
		else if(forkRet>0){
			//parent - write to pipe
		//	close(fd[0]);
		//	int backup_out=dup(STDOUT_FILENO);
		//	dup2(fd[1],STDOUT_FILENO);//redirecting O/P 
		//	if(!chkRedir(pipToks[0]))//outputs got written to fd[1]
		//	 x=0;
			wait(NULL);

		//	dup2(backup_out,1);	//restoring previous O/P
		//	close(backup_out);
			close(fd[1]);	//close write end
			s=fd[0];
		}
		else if(forkRet == 0){
			//child - read from pipe
		//	close(fd[0]);
			if(i!=0)
			dup2(s,0);	//replace stdin of pipe rhs with read end of lhs
			if(i<pipCnt-1){
			//	printf("%s\n", pipToks[i]);
				dup2(fd[1],1); //replace stdout of lhs with write end of rhs
			}
			if(!chkRedir(pipToks[i]))//input from fd[0]
			 x=0;
		//	close(backup_in);
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
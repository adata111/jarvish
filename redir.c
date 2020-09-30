#include "headers.h"
#include "command.h"

int chkRedir(char * comm){
	char * inpF =strchr(comm,'<');int x=1;
			char * outOver = strstr(comm,">>");
			char * outF = strchr(comm,'>');
			if(inpF && outOver){
				char *com;
				if(strlen(inpF)>strlen(outOver)){	//< occurs before >>
					com = strtok(comm,"<");
					inpF = strtok(NULL,">>");
					outOver = strtok(NULL,"\n");
				}
				else{
					com = strtok(comm,">>");
					outOver = strtok(NULL,"<");
					inpF = strtok(NULL,"\n");
				}	
				x=command(com, inpF, outOver+1, 1);
			}
			else if(inpF && outF){
				char * com;
				if(strlen(inpF)>strlen(outF)){	//< occurs before >
					com = strtok(comm,"<");
					inpF = strtok(NULL,">");
					outF = strtok(NULL,"\n");
				}
				else{
					com = strtok(comm,">");
					outF = strtok(NULL,"<");
					inpF = strtok(NULL,"\n");
				}		
				x=command(com, inpF, outF, 0);
			}
			else if(outOver){
				x=command(strtok(comm,">>"),"",outOver+2,1);
			}
			else if(outF){
				x=command(strtok(comm,">"),"",outF+1,0);
			}
			else if(inpF){
				x=command(strtok(comm,"<"),inpF+1,"",0);
			}
			else
			x=command(comm,"","",0);
	return x;
}

void redir(char *tokens[200], int tokenCnt){
	int forkRet = fork();
	tokens[tokenCnt]=NULL;
	if(forkRet == 0){
		if(execvp(tokens[0],tokens)==-1){
			fprintf(stderr,"jarvish: %s: Oops! Command not found :(\n",tokens[0]);
		//	perror("ref");
			exit(1);
		}
	}	
	else{
		//parent
		int status;
		int wpid = waitpid(forkRet, &status, WUNTRACED);
	}   
}
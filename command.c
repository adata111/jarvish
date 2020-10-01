#include "headers.h"
#include "command.h"
#include "history.h"

	char *tokens[200];
	int tokenCnt = 0;
//	printf("he %s\n",token);
void splitCom(char *comm){
//	printf("he %s\n",comm);
	char *token = strtok(comm," \t");
//		printf("hel %s\n",token);
	while(token!=NULL){
		strcpy(tokens[tokenCnt],token);

	//	printf("%s\n",token);
		++tokenCnt;
		token = strtok(NULL, " \t"); 
	}
}

void freeToks(){
	for(int i=0;i<200;i++){
	//	if(i==199) printf("freed\n");
		free(tokens[i]);
	}
}

int command(char *com,char *inpF,char *outF, int nover){
	tokenCnt = 0;
//	printf("COM%sINP%sOUT%sOVER%d\n",com,inpF,outF,nover);
	//remove tabs
	for(int i=0;i<200;i++){
		tokens[i] = (char *)calloc(2000, sizeof(char));
	}
	for(long int i = 0;i<(strlen(com));i++){
		if(com[i]=='\t'){
			com[i]=' ';
		}
	}
	int i;int backup_out;int backup_in;

	for(i=0;i<strlen(inpF);i++){
		if(inpF[i]==' ' || inpF[i]=='\t')
		{}//	outF[i]="";
		else break;
	}
	inpF+=i;
	for(i=strlen(inpF)-1;i>=0;i--){
		if(inpF[i]==' ' || inpF[i]=='\t')
			inpF[i]='\0';
		else break;
	}

	if(strcmp(inpF,"")){
		backup_in=dup(STDIN_FILENO);
		int fd_in;
                fd_in = open(inpF, O_RDONLY);
            if(fd_in < 0)
            {
                perror("jarvish: Input Redirection");
                freeToks();
				exitCode = -1;
                return 1;
            }
            
            if(dup2(fd_in, 0)<0){
            	perror("jarvish: inp dup2");
            	exitCode=-1;
            	return 1;
            }         
            close(fd_in);
	}


	for(i=0;i<strlen(outF);i++){
		if(outF[i]==' ' || outF[i]=='\t')
		{}//	outF[i]="";
		else break;
	}
	outF+=i;
	for(i=strlen(outF)-1;i>=0;i--){
		if(outF[i]==' ' || outF[i]=='\t')
			outF[i]='\0';
		else break;
	}

	if(strcmp(outF,"")){
		
	//printf("COM%sINP%sOUT%sOVER%d\n",com,inpF,outF,nover);
		backup_out=dup(STDOUT_FILENO);
		int fd_out;
            if(nover == 0)
                fd_out = open(outF, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            else if(nover==1)
                fd_out = open(outF, O_WRONLY | O_CREAT | O_APPEND, 0644);

            if(fd_out < 0)
            {
                perror("jarvish: Output Redirection");
                freeToks();
				exitCode = -1;
                return 1;
            }
            if(dup2(fd_out, 1)<0){
            	perror("jarvish: outp dup2");
            	exitCode = -1;
            	return 1;
            }       
            close(fd_out);
	}


	char comCopy[2000];
	strcpy(comCopy,com);
//		printf("he %s\n",comCopy);
	splitCom(com);
//	printf("he %s\n",tokens[0]);

/*	if(strcmp(inpF,"")){
		redir(tokens, tokenCnt);
	}
*/
	if(tokenCnt>0){
	 if(strcmp(tokens[tokenCnt-1],"&")==0){
		executeSys(tokens, tokenCnt, 1);
	}
	else{
//	printf("%s %d\n", tokens[0], tokenCnt);
	if(tokens[0]!=NULL){
		if(strcmp(tokens[0],"cd")==0){
		//	printf("cd\n");
		//	token = strtok(NULL, " ");
			if(tokenCnt==1){
				cd("~");
			}
			else{
				if(tokenCnt>2){
					fprintf(stderr,"jarvish: cd: too many arguments\n");
					exitCode = -1;
				}
				else{
					cd(tokens[1]);
				}
			}
		}
		else if(strcmp(tokens[0], "pwd")==0){
			if(getcwd(cwd,2000)==NULL){
				perror("Get current working directory ERROR");
				exitCode = -1;
			}
			else{
				printf("%s\n",cwd);
				exitCode = 5;
			}
		}
		else if(strcmp(tokens[0], "echo")==0){

//				printf("jbkjb%s",comCopy);
			echo(comCopy);
			exitCode = 5;
		}
		else if(strcmp(tokens[0], "ls")==0){
			lsSeparate(tokens, tokenCnt);
		}
		else if(strcmp(tokens[0], "quit")==0 || strcmp(tokens[0],"exit")==0){
			
			quit();
			
			run = 0;
			
		}
		else if(strcmp(tokens[0], "pinfo")==0){
			pinfo(tokens, tokenCnt);
		}
		else if(strcmp(tokens[0], "overkill")==0){
			if(tokenCnt>1) {
				fprintf(stderr,"jarvish: overkill: Too many arguments detected\n");
				exitCode = -1;
			}
			else overkill();
		}
		else if(strcmp(tokens[0], "jobs")==0){
			if(tokenCnt>1) {
				fprintf(stderr,"jarvish: jobs: Too many arguments detected\n");
				exitCode = -1;
			}
			else jobs();
		}
		else if(strcmp(tokens[0], "kjob")==0){
			if(tokenCnt!=3) {
				fprintf(stderr,"jarvish: kjob: Incorrect number of arguments. Format 'kjob <job number> <signal number>'\n");
				exitCode = -1;
			}

			else{ 
				kjob(atoi(tokens[1]),atoi(tokens[2]));
			}
		}
		else if(strcmp(tokens[0], "setenv")==0){
			if(tokenCnt!=2 && tokenCnt!=3) {
				fprintf(stderr,"jarvish: setenv: Incorrect number of arguments. Format 'setenv var [value]'\n");
				exitCode = -1;
			}

			else{ 
				if(tokenCnt==3){
					if(setenv(tokens[1],tokens[2],1)<0){
						perror("jarvish: setenv");
						exitCode = -1;
					}
					else
						exitCode = 5;
				}
				else{
					if(setenv(tokens[1],"",1)<0){
						perror("jarvish: setenv");
						exitCode = -1;
					}
					else
						exitCode = 5;
				}
			}
		}
		else if(strcmp(tokens[0], "unsetenv")==0){
			if(tokenCnt!=2) {
				fprintf(stderr,"jarvish: unsetenv: Incorrect number of arguments. Format 'unsetenv var'\n");
				exitCode = -1;
			}

			else{ 
				if(unsetenv(tokens[1])<0){
					perror("jarvish: unsetenv");
					exitCode = -1;
				}
				else{
					exitCode = 5;
				}
			}
		}
		else if(strcmp(tokens[0], "fg")==0){
			if(tokenCnt!=2) {
				fprintf(stderr,"jarvish: fg: Incorrect number of arguments. Format 'fg <job number>'\n");
				exitCode = -1;
			}

			else{ 
				fg(atoi(tokens[1]));
			}
		}
		else if(strcmp(tokens[0], "bg")==0){
			if(tokenCnt!=2) {
				fprintf(stderr,"jarvish: bg: Incorrect number of arguments. Format 'bg <job number>'\n");
				exitCode = -1;
			}

			else{ 
				bg(atoi(tokens[1]));
			}
		}
		else if(strcmp(tokens[0], "nightswatch")==0){
			nightswatch(tokens, tokenCnt);
		}
		else if(strcmp(tokens[0], "history")==0){
			history(comCopy, tokens, tokenCnt);
		}
		else{
		//	printf("Command not found :(:%s\n",tokens[0]);
			executeSys(tokens, tokenCnt, 0);
		}
	//	printf("command over\n");

	}	
	}
	}
	if(strcmp(outF,"")){
			if(dup2(backup_out, 1)<0){
            	perror("jarvish: stdout dup2");
            	exitCode=-1;
            	return 1;
            }
			close(backup_out);
		}
		if(strcmp(inpF,"")){
			if(dup2(backup_in, 0)<0){
            	perror("jarvish: stdin dup2");
            	exitCode=-1;
            	return 1;
            }
			close(backup_in);
		}
		fflush(stdout);
	freeToks();
	//printf("%d\n", run);
	if(run) return 1;
	else return 0;
	//get the command and arguments
	//token = strtok(newCom, " ");
}
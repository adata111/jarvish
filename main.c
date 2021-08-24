#include "prompt.h"
#include "headers.h"
#include "command.h"
#include "history.h"

void ctrlC(int sig){
	pid_t nowP = getpid();		//id of the process that called ctrlC
	if(nowP<0){
		perror("getpid");
		exitCode = -1;
		return;
	}
	else if(nowP != jarvPid)	// if the shell didnt call it then don't do anything
		// we just needd to kill child processes not the shell itself
		return;
	if(fgT.pid==-1){}	// error handling, fgT.pid is initialized with -1
	else{
		// kill fg process
		kill(fgT.pid,SIGINT);
		exitCode = -1;
		fgT.pid=-1;
	}
	signal(SIGINT, ctrlC);	//let parent know that the child died
}

void ctrlZ(int sig){
	pid_t nowP = getpid();
	if(nowP<0){
		perror("getpid");
	//	exitCode = -1;
		return;
	}
	else if(nowP != jarvPid)
		return;

	if(fgT.pid!=-1){
		if(kill(fgT.pid,SIGSTOP)<0){
			perror("kill");
		}
		else{
			exitCode=-1;
		}
	}
	fgT.pid=-1;
}

char miniCom[20][2000];
int num;int op[25];

char **chain(char *lilCom){
	char *copy = (char *)calloc(2000,sizeof(char));
	strcpy(copy,lilCom);
	char *mini = strtok(copy,"@$");
	num=0;
	while(mini!=NULL){
		strcpy(miniCom[num],mini);
		++num;
		mini = strtok(NULL, "@$"); 
	}
	char *and = strchr(lilCom,'@');
	char *or = strchr(lilCom,'$');int n=0;
	for(int x=0;x<strlen(lilCom);x++){
		if(lilCom[x]=='@'){
			op[n]=2;
			n++;
		}
		else if(lilCom[x]=='$'){
			op[n++]=1;
		}
	}
	free(copy);
}

int main()
{
	welc();
	jarvPid = getpid();  // pid of jarvish
	gethostname(host,sizeof(host));
	getlogin_r(user,sizeof(user));
	if(getcwd(myhome,sizeof(myhome))==NULL){
		perror("Get home directory ERROR");
		exit(1);
	}
	strcpy(prevwd,myhome);
	strcpy(cwd,myhome);
	for(int i=0;i<30;i++){
		histArr[i] = (char *)calloc(2000, sizeof(char));
	}
	freed=0;
	bgCnt=0;
	loadHist();
	char lilCom[100][2000];
	run=1;
	signal(SIGINT, ctrlC);
	signal(SIGTSTP, ctrlZ);
	exitCode=0;
    while (1)
    {
    	fgT.pid=-1;
        prompt();
        exitCode = 5;
        // TAKE INPUT HERE
        size_t inpSize = 0;
        char *input; char inp[2000];
        input= (char *)malloc(2000 * sizeof(char));
        if(getline(&input,&inpSize,stdin)==-1){
        	if(errno>=0){

        		printf("\n");
        		quit();
        	}
        	else{
        		for(int i=0;i<30;i++){
				 	free(histArr[i]);
				 }
			//	 printf("%d\n", errno);
        		perror("Jarvish: input");
        		exitCode = -1;
        	}
        	free(input);
        	break;
        }
    	fflush(stdout);
        strcpy(inp,input);
        exitCode = 5;
        addToHistArr(inp);
        char* token = strtok(input, ";\n");
        
        int numCom = 0;  
        while (token != NULL) { 
			strcpy(lilCom[numCom],token);
			++numCom;
			token = strtok(NULL, ";\n"); 
			
		} 
    	free(input);

		int x=5;int j=0;int cumExit;
		for(int i=0;i<numCom;i++){
			exitCode=5;
			chain(lilCom[i]);//split based on and or, and store and or in order, then execute following commands
			for(j=0;j<num;j++){
				if(j){
					if(op[j-1]==1){//OR
						if(cumExit>0){
							continue;
						}
					}
					else if(op[j-1]==2){//AND
						if(cumExit<0){
							continue;
						}
					}
				}
				exitCode = 5;
				if(strchr(miniCom[j],'|')){
					x= pip(miniCom[j]);
					continue;
				}
				x=chkRedir(miniCom[j]);
				if(j==0) cumExit = exitCode;
				else{
					cumExit=exitCode;
				}

				if(x==0) return 0;
			}
			
		}
		if(x==0) break;

    }
    return 0;
}

#include "prompt.h"
#include "headers.h"
#include "command.h"
#include "history.h"

void ctrlC(int sig){
//	printf("ctrl C\n");
//	printf("%d\n",getpid());
//	quit();
//	prompt();
//	printf("\n");
	if(fgT.pid==-1){}
	else{
		kill(fgT.pid,SIGINT);
		exitCode = -1;
		fgT.pid=-1;
	}
}
void ctrlZ(int sig){
	kill(fgT.pid,SIGSTOP);
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
	/*for(int i=0;i<256;i++){
		lilCom[i] = (char *)malloc(2000 * sizeof(char));
	}*/
//	printf("%s@%s:~%s",user,host,myhome);
//	fflush(stdout);
	run=1;
	signal(SIGINT, ctrlC);
	signal(SIGTSTP, ctrlZ);
	exitCode=0;
    while (1)
    {
    	fgT.pid=-1;
   // 	printf("hi%d\n", exitCode);
        prompt();
        exitCode = 5;
        // TAKE INPUT HERE
        size_t inpSize = 0;
        char *input; char inp[2000];
        input= (char *)malloc(2000 * sizeof(char));
     //   printf("\nSTDIN_FILENO %d\n",STDIN_FILENO);
        if(getline(&input,&inpSize,stdin)==-1){
        //	printf("%s %ld\n",input,inpSize);
        	if(errno==0){

        		printf("\n");
        		quit();
        	}
        	else{
        		for(int i=0;i<30;i++){
				 	free(histArr[i]);
				 }
        		perror("Jarvish: input");
        		exitCode = -1;
        	}
        	free(input);
        	break;
        }
   // 	printf("b%d\n", exitCode);
    	fflush(stdout);
        strcpy(inp,input);
        //inp = strtok(input,"\n");
    //   printf("%s\n", inp);
    //    for(int i=0;i<10;i++){}
        exitCode = 5;
        addToHistArr(inp);
      //  printf("h %s\n", input);
      //  for(int i=0;i<10;i++){}
        char* token = strtok(input, ";\n");
    //	printf("by%d\n", exitCode);
        
        int numCom = 0;  
        while (token != NULL) { 
		//	command(token); 
		//	printf("%s\n",token );
			strcpy(lilCom[numCom],token);
			++numCom;
			token = strtok(NULL, ";\n"); 
			
		} 
    	free(input);
		//printf("%c\n",lilCom[numCom][1]);

		int x=5;int j=0;int cumExit;
		for(int i=0;i<numCom;i++){
	//		printf("%s\n",lilCom[i]);
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

				//else break;
	//			printf("%d\n", x);
				if(x==0) return 0;
			}
			
		}
	//	if(cumExit!=0) exitCode=cumExit;
		if(x==0) break;
    //	printf("run=%d\n",x);

    }
//    printf("end\n");
	/*for(int i=1;i<20;i++){
		printf("%d\n",i);
	 	free(histArr[i]);
	//	free(lilCom[i]);
	}*/
    return 0;
}

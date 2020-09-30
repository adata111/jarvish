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
	if(fgPid==-1){}
	else{
		kill(fgPid,SIGINT);
		fgPid=-1;
	}
}
void ctrlZ(int sig){
	kill(fgPid,SIGSTOP);
	fgPid=-1;
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
	for(int i=0;i<30;i++){
		histArr[i] = (char *)calloc(2000, sizeof(char));
	}
	bgCnt=0;
	loadHist();
	char lilCom[256][2000];
	/*for(int i=0;i<256;i++){
		lilCom[i] = (char *)malloc(2000 * sizeof(char));
	}*/
//	printf("%s@%s:~%s",user,host,myhome);
//	fflush(stdout);
	run=1;
	signal(SIGINT, ctrlC);
	signal(SIGTSTP, ctrlZ);
    while (1)
    {
    	fgPid=-1;
        prompt();
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
        	}
        	free(input);
        	break;
        }
        strcpy(inp,input);
        //inp = strtok(input,"\n");
    //    printf("%s\n", inp);
    //    for(int i=0;i<10;i++){}
        addToHistArr(inp);
      //  printf("h %s\n", input);
      //  for(int i=0;i<10;i++){}
        char* token = strtok(input, ";\n");
        
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
		int x=5;
		for(int i=0;i<numCom;i++){
	//		printf("%s\n",lilCom[i]);
			if(strchr(lilCom[i],'|')){
				x= pip(lilCom[i]);
				continue;
			}
			chkRedir(lilCom[i]);
			//else break;
//			printf("%d\n", x);
			if(x==0) return 0;
			
		}
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

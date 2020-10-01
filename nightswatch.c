#include "headers.h"
#include <sys/time.h>

void interr(int interval){
	char *intrrptFile;int boo=0;
	char *input; long inpSize;
	intrrptFile = malloc(50 * sizeof(char));
	strcpy(intrrptFile, "/proc/interrupts");
	
    struct timeval tv;
    int retval;

    /* Watch stdin (fd 0) to see when it has input. */
    
    /* Wait up to five seconds. */
    tv.tv_sec = interval;
    tv.tv_usec = 0;
	while(1){
		fd_set rfds;
		FD_ZERO(&rfds);
    FD_SET(0, &rfds);
		retval = select(1, &rfds, NULL, NULL, &tv);
    /* Don’t rely on the value of tv now! */

    if (retval == -1){
        perror("select()");
        exitCode = -1;
    }
    else if (retval){
     //   printf("Data is available now.\n");
        char c = fgetc(stdin);
        char d = fgetc(stdin);
     //  	printf("%c %c",d,c);
       	if(c=='q'){
			return;
		}
    }
        /* FD_ISSET(0, &rfds) will be true. */
    else{
      //  printf("No data within five seconds.\n");
	//	printf("printing every 2 sec\n");
	//	fflush(stdout);
	FILE *stream = fopen(intrrptFile, "r");
	if(stream == NULL){
		perror("jarvish: nightswatch: interrupt");
		exitCode = -1;
		return;
	}
	else{
		char *data = NULL;
		long len=0;
		long r = getline(&data,&len,stream);
		char *toPrint = data + 5;
		long lineNo=0;
		while(r>0){
			lineNo++;
			if(lineNo==1 && !boo){
				printf("%s",toPrint);
				boo=1;
			}
			else if(lineNo==2 || lineNo==3){
			//	printf("%s\n", toPrint);
				char *t = strstr(toPrint, "  IR-IO-APIC");
				if(strstr(t,"i8042")){
			//	printf("%s\n",(t));
				toPrint[t-toPrint]='\0';
				printf("%s\n", toPrint);
				break;
				}
			}
			r = getline(&data,&len,stream);
			toPrint = data+5;
		}
		fclose(stream);
	}
	}
//	getline(&input,&inpSize,stdin);
//	char c = fgetc(stdin);
//	if(c=='q'){
//		return;
//	}
	sleep(interval);
	}
}

void newb(int interval){
	struct timeval tv;
    int retval;int k,j,n,x,maxi=0;
    char *loadavgFile;
    loadavgFile = malloc(50 * sizeof(char));
	strcpy(loadavgFile, "/proc/loadavg");
	

    /* Watch stdin (fd 0) to see when it has input. */
    
    /* Wait up to five seconds. */
    tv.tv_sec = interval;
    tv.tv_usec = 0;
	while(1){
		fd_set rfds;
		FD_ZERO(&rfds);
    FD_SET(0, &rfds);
		retval = select(1, &rfds, NULL, NULL, &tv);
    /* Don’t rely on the value of tv now! */

    if (retval == -1){
        perror("select()");
        exitCode = -1;
    }
    else if (retval){
     //   printf("Data is available now.\n");
        char c = fgetc(stdin);
        char d = fgetc(stdin);
     //  	printf("%c %c",d,c);
       	if(c=='q'){
			return;
		}
    }
        /* FD_ISSET(0, &rfds) will be true. */
    else{
      //  printf("No data within five seconds.\n");
	//	printf("printing every 2 sec\n");
	//	fflush(stdout);
	/*	struct dirent **namelist;
		n = scandir("/proc", &namelist, NULL, alphasort);
		if (n == -1) {
	    	perror("jarvish: proc directory");
	    }
	    else{


	    	for(j=0;j<n;j++) {
	//	    	printf("to print list");
		    	char *name = namelist[j]->d_name;
		    //	printf("%s\n",name);
		    //	if(name[0]>'9'){
		    //		break;
		    //	}
		    	for(k=0;k<strlen(name);k++){
		    		if(name[k]>='0' && name[k]<='9'){
		    			continue;
		    		}
		    		else break;
		    	}
		    	if(k==strlen(name)){
		    		x=atoi(name);
		    		if(maxi<x) maxi=x;
		    	}
		    }
		    printf("%d\n",maxi);
	    }
	    */
	    FILE *stream = fopen(loadavgFile, "r");
		if(stream == NULL){
			perror("jarvish: nightswatch: newborn");
			exitCode = -1;
			return;
		}
		else{
			char *data = NULL;
			long len=0;
			long r = getline(&data,&len,stream);
			if(r>0){
				printf("%s",(strrchr(data,' ')+1));
			}
		}
	}
//	getline(&input,&inpSize,stdin);
//	char c = fgetc(stdin);
//	if(c=='q'){
//		return;
//	}
	sleep(interval);
	}
}

void nightswatch(char *tokens[200], int tokenCnt){
	
    
	if(tokenCnt!=4){
		fprintf(stderr,"jarvish: nightswatch: Incorrect number of arguments provided\n");
		exitCode = -1;
		return;
	}
	if(strcmp(tokens[3],"interrupt")==0){
		interr(atoi(tokens[2]));
	}
	else if(strcmp(tokens[3],"newborn")==0){
		newb(atoi(tokens[2]));
	}
	else{
		fprintf(stderr,"jarvish: nightswatch: Please enter either `interrupt` or `newborn` as command");
		exitCode = -1;
	}
}

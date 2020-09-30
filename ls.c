#include "headers.h"
#include "pwd.h"
#include "grp.h"
#include "time.h"
#include "command.h"

void lsL(char *dirPath, char *name){
	struct stat stats;
	char *perm;
	perm = (char *)malloc(50 * sizeof(char));
	//printf("hi\n");
	int dir = stat(dirPath,&stats);
	if(dir==0 && S_ISDIR(stats.st_mode)){// S_ISDIR to check if input directory is a directory or not
	    strcpy(perm,"d");
	}
	else strcpy(perm,"-");
	if(stats.st_mode & S_IRUSR)
    	strcat(perm,"r");
    else
   		strcat(perm,"-");
	if(stats.st_mode & S_IWUSR)
    	strcat(perm,"w");
    else
   		strcat(perm,"-");
	if(stats.st_mode & S_IXUSR)
    	strcat(perm,"x");
    else
   		strcat(perm,"-");
//	printf("%s\n",perm);
	if(stats.st_mode & S_IRGRP)
    	strcat(perm,"r");
    else
   		strcat(perm,"-");
	if(stats.st_mode & S_IWGRP)
    	strcat(perm,"w");
    else
   		strcat(perm,"-");
	if(stats.st_mode & S_IXGRP)
    	strcat(perm,"x");
    else
   		strcat(perm,"-");
//	printf("%s\n",perm);
	if(stats.st_mode & S_IROTH)
    	strcat(perm,"r");
    else
   		strcat(perm,"-");
	if(stats.st_mode & S_IWOTH)
    	strcat(perm,"w");
    else
   		strcat(perm,"-");
	if(stats.st_mode & S_IXOTH)
    	strcat(perm,"x");
    else
   		strcat(perm,"-");
//	printf("%s\n",perm);
//	printf("%s\n",name);
//   	perm[10]='\0';
   	struct group *grp; 
   	grp = getgrgid(stats.st_gid);
   	struct passwd *pwd;
   	pwd = getpwuid(stats.st_uid);
   	char *datTim; 
   	time_t curTstamp;
   	struct tm *fileTstruct;struct tm *curTstruct;
   	curTstamp = time(NULL);
 //  	printf("hkbkbi\n");
   	curTstruct = localtime(&curTstamp);
 //  	printf("%s\n",ctime(&curTstamp));
   //	printf("%d\n",curTstruct->tm_year);
   	datTim = (char *)malloc(500 * sizeof(char));
 //  	printf("hkbkbi\n");
   	int currY = curTstruct->tm_year;
   	fileTstruct = localtime(&(stats.st_mtime));
   	//printf("%d\n",curTstruct->tm_year);
 //  	printf("hkbkbi\n");
 //  	printf("%d %d\n",fileTstruct->tm_year,curTstruct->tm_year);
   	if(fileTstruct->tm_year == currY){
   		strftime(datTim, 500, "%b %d %H:%M", fileTstruct);
   	//	printf("hkbkbi\n");
   	}
   	else{
   	strftime(datTim, 500, "%b %d %Y", fileTstruct);
   	}
   	printf("%s\t%ld\t%s\t%s\t%ld\t%s\t%s\n",perm, stats.st_nlink, pwd->pw_name, grp->gr_name,stats.st_size,datTim,name);
	free(datTim);
	free(perm);
}

void ls(char **files, int noOfFiles, int showHid, int showL){
	struct dirent **namelist;
    int n, i, j;
    for(i=0;i<noOfFiles;i++){
    	char *fileP = (char *)malloc(1000 * sizeof(char));
	    n = scandir(files[i], &namelist, NULL, alphasort);
	    if (n == -1) {
	    	if(errno == ENOTDIR){
	    		printf("%s\n",files[i]);
	    	}
	    	else{

	    //		printf("%s\n",files[i]);
	    		perror("Jarvish: ls");
	    	}

	    }
	    else{ 
//	    	printf("%d %d\n",showHid,showL);
	    	if(noOfFiles>1) printf("%s:\n",files[i]);
//	    	printf("%d\n",n);
		    for(j=0;j<n;j++) {
//		    	printf("to print list\n");
		    	if(showHid && !showL)
		    	    printf("%s\n", namelist[j]->d_name);
		    	else if(showHid && showL){
	//	    		printf("hello");
		    		strcpy(fileP,files[i]);
	//	    		printf("hell");
		    		strcat(fileP,"/");
		    		strcat(fileP,namelist[j]->d_name);
//		    		printf("\n%s\n",fileP);
		    		lsL(fileP, namelist[j]->d_name);
		    	}
		    	else if(!showHid && showL){
//		    		printf("long");
		    		if((namelist[j]->d_name)[0]!='.'){
	//		    		printf("hello");
			    		strcpy(fileP,files[i]);
	//		    		printf("hell");
			    		strcat(fileP,"/");
			    		strcat(fileP,namelist[j]->d_name);
//			    		printf("hel");
			    		lsL(fileP, namelist[j]->d_name);
			    	}
		    	}
		    	else if(!showHid && !showL){
		    		if((namelist[j]->d_name)[0]!='.'){
		    			printf("%s\n", namelist[j]->d_name);
		    		}
		    	}
		    	
		       // free(namelist[j]);
		    }
		    free(namelist);
		}
		free(fileP);
	}
}

void lsSeparate(char *tokens[200], int tokenCnt){
	char flags[10];
	//char **files = (char **)malloc(200* sizeof(char *));
	char *files[200];
	for(int i = 0; i < 200; i++)
		files[i] = (char*)malloc(1000 * sizeof(char)); 
//	printf("%s\n",argus);
	char *argCopy;
	int i, j, fgCnt,fiCnt;
//	strcpy(argCopy, argus);
//	printf("heo\n");
/*	char *token = strtok(argus," ");
//	printf("he %s\n",token);
	while(token!=NULL){
		tokens[tokenCnt] = token;
//		printf("%s",token);
		++tokenCnt;
		token = strtok(NULL, " "); 
	}
	*/
//	printf("\nhell\n");
	//tokens contains args separated by space. Eg:"ls -l hi -a";tokens={ls,-l,hi,-a}
//	printf("hello\n");
	fgCnt=0;fiCnt=0;
	for(i=1;i<tokenCnt;i++){
		if(tokens[i][0]=='-'){
	//		printf("hellfjo\n");
			for(j=1;j<strlen(tokens[i]);j++){
	//			printf("ellfjo\n");
				flags[fgCnt]=tokens[i][j];
	//			printf("hefjo\n");
				fgCnt++;
			}
		}
		else{
		//	for(j=0;j<strlen(tokens[i]);j++){
	//		printf("hlo\n");
			if(tokens[i][0]=='~'){
			//	char * t = strchr(tokens[i],'~');
				strcpy(files[fiCnt],myhome);
	//			printf("%s\n", files[fiCnt]);
				strcat(files[fiCnt],(tokens[i]+1));
			//	printf("%s\n", files[fiCnt]);
				fiCnt++;
			}
			else{
	//			printf("h8lo\n");
				strcpy(files[fiCnt],tokens[i]);
	//			printf("h89lo\n");
				fiCnt++;
			}
			}
	//		printf("%d %d\n",i,tokenCnt);
		
	}
	if(fiCnt == 0){
		files[fiCnt]=".";
		++fiCnt;
	//	printf("no args\n");
	}
	flags[fgCnt]='\0';
	if(!strcmp(flags,"l")){
	//	printf("l\n");
		ls(files,fiCnt,0,1);
	}
	else if(!strcmp(flags,"a")){
		ls(files,fiCnt,1,0);
	}
	else if(strcmp(flags,"la")==0 || strcmp(flags,"al")==0){
		ls(files,fiCnt,1,1);
	}
	else{//no flag or some wrong flag
	//	printf("%d\n", fgCnt);
		if(fgCnt>0){
			executeSys(tokens,tokenCnt,0);
		}
		else
			ls(files,fiCnt,0,0);
	}
	for(int i = 1; i <200; i++){
	//	printf("%d\n",i);
		free(files[i]); 
	}
	//free(files[0]);
}

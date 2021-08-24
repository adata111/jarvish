#include "headers.h"
#include "pwd.h"
#include "grp.h"
#include "time.h"
#include "command.h"

void lsL(char *dirPath, char *name){
	struct stat stats;
	char *perm;
	perm = (char *)malloc(50 * sizeof(char));
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

   	struct group *grp; 
   	grp = getgrgid(stats.st_gid);
   	struct passwd *pwd;
   	pwd = getpwuid(stats.st_uid);
   	char *datTim; 
   	time_t curTstamp;
   	struct tm *fileTstruct;struct tm *curTstruct;
   	curTstamp = time(NULL);
   	curTstruct = localtime(&curTstamp);
   	datTim = (char *)malloc(500 * sizeof(char));
   	int currY = curTstruct->tm_year;
   	fileTstruct = localtime(&(stats.st_mtime));
   	if(fileTstruct->tm_year == currY){
   		strftime(datTim, 500, "%b %d %H:%M", fileTstruct);
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

	    		exitCode = -1;
	    		perror("Jarvish: ls");
	    	}

	    }
	    else{ 
	    	if(noOfFiles>1) printf("%s:\n",files[i]);
		    for(j=0;j<n;j++) {
		    	if(showHid && !showL)
		    	    printf("%s\n", namelist[j]->d_name);
		    	else if(showHid && showL){
		    		strcpy(fileP,files[i]);
		    		strcat(fileP,"/");
		    		strcat(fileP,namelist[j]->d_name);
		    		lsL(fileP, namelist[j]->d_name);
		    	}
		    	else if(!showHid && showL){
		    		if((namelist[j]->d_name)[0]!='.'){
			    		strcpy(fileP,files[i]);
			    		strcat(fileP,"/");
			    		strcat(fileP,namelist[j]->d_name);
			    		lsL(fileP, namelist[j]->d_name);
			    	}
		    	}
		    	else if(!showHid && !showL){
		    		if((namelist[j]->d_name)[0]!='.'){
		    			printf("%s\n", namelist[j]->d_name);
		    		}
		    	}
		    	
		    }
		    free(namelist);
		}
		free(fileP);
	}
}

void lsSeparate(char *tokens[200], int tokenCnt){
	char flags[10];
	char *files[200];
	for(int i = 0; i < 200; i++)
		files[i] = (char*)malloc(1000 * sizeof(char)); 
	char *argCopy;
	int i, j, fgCnt,fiCnt;

	fgCnt=0;fiCnt=0;
	for(i=1;i<tokenCnt;i++){
		if(tokens[i][0]=='-'){
			for(j=1;j<strlen(tokens[i]);j++){
				flags[fgCnt]=tokens[i][j];
				fgCnt++;
			}
		}
		else{
			if(tokens[i][0]=='~'){
				strcpy(files[fiCnt],myhome);
				strcat(files[fiCnt],(tokens[i]+1));
				fiCnt++;
			}
			else{
				strcpy(files[fiCnt],tokens[i]);
				fiCnt++;
			}
		}
		
	}
	if(fiCnt == 0){
		files[fiCnt]=".";
		++fiCnt;
	}
	flags[fgCnt]='\0';
	if(!strcmp(flags,"l")){
		ls(files,fiCnt,0,1);
	}
	else if(!strcmp(flags,"a")){
		ls(files,fiCnt,1,0);
	}
	else if(strcmp(flags,"la")==0 || strcmp(flags,"al")==0){
		ls(files,fiCnt,1,1);
	}
	else{//no flag or some wrong flag
		if(fgCnt>0){
			executeSys(tokens,tokenCnt,0);
		}
		else
			ls(files,fiCnt,0,0);
	}
	for(int i = 1; i <200; i++){
		free(files[i]); 
	}
}

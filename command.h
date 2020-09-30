#ifndef __COMMAND_H
#define __COMMAND_H

char host[1030], user[1030], cwd[2050], myhome[2050];
int command(char *com, char * inpF, char *outF, int over);
void freeToks();
void echo(char *toPrint);
void cd(char *newDir);
void executeSys(char *tokens[200], int tokenCnt, int backGnd);
int chkRedir(char *comm);
void redir(char *tokens[200], int tokenCnt);
int pip(char *comm);
//void ls(char *files[20], int noOfFiles, char *flags);
void lsSeparate(char *tokens[200], int tokenCnt);
void pinfo(char *tokens[200], int tokenCnt);
void nightswatch(char *tokens[200], int tokenCnt);
void quit();
void jobs();
char * delBg(pid_t pid);
void childHandler(int sig);
void kjob(int jobnum, int signum);
void fg(int jobnum);
void bg(int jobnum);
void overkill();

#endif
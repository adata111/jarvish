#ifndef __HISTORY_H
#define __HISTORY_H

#include "math.h"
void saveHist();
void loadHist();
char *histArr[30];
int histNo;
void addToHistArr(char *comm);
void history(char *com, char *tokens[200], int tokenCnt);

#endif
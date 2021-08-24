#include "headers.h"

void echo(char *toPrint){
	char *token = strtok(toPrint, " ");
	token = strtok(NULL, " "); 
		if(token!=NULL){
			printf("%s",token);
			while (token != NULL) { 
					
				token = strtok(NULL, " "); 
				if(token!=NULL) printf(" %s",token);
			} 
		}
	printf("\n");
}
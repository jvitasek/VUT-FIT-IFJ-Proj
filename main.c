/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * main.c
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

int main(int argc, char **argv)
{
	FILE *input;
	string attr;		// vytvorim si string
	strInit(&attr);		// inicializujem string
	T_Type token;		// vytvorim si premennu token
	if((argc < 2) || (argc > 2))		// kontrola ci bol zadany spravny pocet parametrov na prikaz. riadke
	{
		fprintf(stderr,"Spatne patametry prikazove radky.\n");
		return 99;
	}
	
	if((input = (fopen(argv[1],"r"))) == 0)		// otvorime vstupny subor na citanie
	{
		fprintf(stderr,"Zlyhanie otvarania suboru.\n");
		return 99;
	}
	
	do
	{
		token = getToken(input,&attr);
		if(token == T_Error) exit(1);
		printf("%s %d\n",attr.str,token);
	}while(token != 35);	//T_EOF
	
	fclose(input);		// zavre vstupny subor
	strFree(&attr);		// uvolni string z pamete
	return 0;
}


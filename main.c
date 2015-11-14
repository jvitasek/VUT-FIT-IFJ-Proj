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
#include "parser.h"
#include <stdbool.h>

int main(int argc, char **argv)
{
	FILE *input;
	string attr;		// vytvorim si string
	strInit(&attr);		// inicializujem string
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
	if(func(input,&attr) != true)
	{
		fprintf(stderr,"Program nie je syntakticky spravny!\n");	//pociatocny neterminal
		return 2;	// Syntakticka chyba
	}else printf("Vsetko OK!\n");
	
	fclose(input);		// zavre vstupny subor
	strFree(&attr);		// uvolni string z pamete
	return 0;
}


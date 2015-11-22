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
#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "parser.h"

int main(int argc, char **argv)
{
	FILE *input;
	string attr;		// vytvorim si string
	strInit(&attr);		// inicializujem strin
	final_error = "";
	if((argc < 2) || (argc > 2))		// kontrola ci bol zadany spravny pocet parametrov na prikaz. riadke
	{
		print_error(ERUN_IN, 0);
	}
	
	if((input = (fopen(argv[1],"r"))) == 0)		// otvorime vstupny subor na citanie
	{
		print_error(ERUN_IN, 0);
	}

	/* HLAVNI FUNKCE */
	print_error(parse(input,&attr), token.line);
	if(strcmp(final_error, "") != 0)
	{
		fprintf(stderr, "%s\n", final_error);
	}
	
	fclose(input);		// zavre vstupny subor
	strFree(&attr);		// uvolni string z pamete
	return 0;
}


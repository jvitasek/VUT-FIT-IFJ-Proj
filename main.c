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
	string attr;		// vytvorime si string
	strInit(&attr);		// inicializujeme string
	if((argc < 2) || (argc > 2))		// kontrola ci bol zadany spravny pocet parametrov na prikaz. riadke
	{
		print_error(ERUN_IN, 0);
	}
	
	if((input = (fopen(argv[1],"r"))) == 0)		// otvorime vstupny subor na citanie
	{
		print_error(ERUN_IN, 0);
	}

	int result;
	result = parse(input, &attr); // syntakticka analyza
	switch(result)
	{
		case ENOP:
			// vse v poradku
			printf("SUCCESS: vse v poradku!\n");
			break;
		default:
			// nastala chyba v prekladu
			/**
			 * @todo uvolneni tabulky symbolu
			 * @todo uvolneni instruction listu
			 */
			print_error(result, token.line);
			fclose(input);
			strFree(&attr);
			return -result;
		break;
	}
	/**
	 * @todo interpretace (predani tabulky symbolu a instruction listu)
	 */
	
	/**
	 * @todo uvolneni tabulky symbolu
	 * @todo uvolneni instruction listu
	 */
	fclose(input);		// zavre vstupny subor
	strFree(&attr);		// uvolni string z pamete
	return 0;
}


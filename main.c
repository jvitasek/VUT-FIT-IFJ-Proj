/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * main.c
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 * 			xvitas02 - Jakub Vitasek
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "parser.h"
#include "interpret.h"

#define DEBUG 1

int main(int argc, char **argv)
{
	FILE *input;
	listInit(&List);

	if((argc < 2) || (argc > 2))		// kontrola ci bol zadany spravny pocet parametrov na prikaz. riadke
	{
		print_error(ERUN_IN, 0);
	}
	
	if((input = (fopen(argv[1],"r"))) == 0)		// otvorime vstupny subor na citanie
	{
		print_error(ERUN_IN, 0);
	}
	
	int result;
	result = parse(input); // syntakticka analyza
	
	switch(result)
	{
		case ENOP:
			// vse v poradku
			fprintf(stderr, "SUCCESS: vse v poradku!\n");
			break;
		default:
			// nastala chyba v prekladu
			/**
			 * @todo uvolneni tabulky symbolu
			 * @todo uvolneni instruction listu
			 */
			print_error(result, token.line);
			fclose(input);
			//strFree(&attr);
			return result;
		break;
	}
	
	int interRes = interpret(&List);
	if(interRes != 0) printf("!!!! nastala chyba v interprete !!!!\n");
	/**
	 * @todo interpretace (predani tabulky symbolu a instruction listu)
	 */
	
	/**
	 * @todo uvolneni tabulky symbolu
	 * @todo uvolneni instruction listu
	 */
	#ifdef DEBUG
	printElementsOfList(List);
	#endif
	listDispose(&List);
	fclose(input);		// zavre vstupny subor
	//strFree(&attr);		// uvolni string z pamete
	return 0;
}


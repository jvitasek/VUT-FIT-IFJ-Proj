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

/**
 * HLAVNÍ FUNKCE PROGRAMU.
 * @param  argc Počet argumentů.
 * @param  argv Pole řetězců zadaných uživatelem.
 * @return      Návratový typ.
 */
int main(int argc, char **argv)
{
	FILE *input;
	listInit(&List);

	// kontrola ci bol zadany spravny pocet parametrov na prikaz. riadke
	if((argc < 2) || (argc > 2))
	{
		print_error(ERUN_IN, 0);
	}
	
	// otvorime vstupny subor na citanie
	if((input = (fopen(argv[1],"r"))) == 0)
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
			htClearAll(paraTable);
			htClearAll(funcTable);
			htClearAll(commTable);
			print_error(result, token.line);
			fclose(input);
			return result;
		break;
	}
	int interRes = interpret(&List);
	if(interRes != 0) printf("!!!! nastala chyba v interprete !!!!\n");

	#ifdef DEBUG
	printElementsOfList(List);
	#endif
	listDispose(&List);
	fclose(input);		// zavre vstupny subor
	return 0;
}


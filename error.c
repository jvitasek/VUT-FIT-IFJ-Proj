/**
* -----------------------------------------------------------------------------
*
* Implementace interpretu imperativniho jazyka IFJ15
*
* Soubor       : error.c
* Popis        : Chybové hlášky
* Datum        : Listopad 2015
* Autori       : xvitas02 – Jakub Vitásek
*              
* -----------------------------------------------------------------------------
**/

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/**
 * Funkce na vypis chyby a vraceni adekvatniho kodu.
 * @param code Cislo chyby.
 * @param line Cislo radku vyskytu chyby.
 */
void print_error(TError code, int line) {
	const char *error_msg[] = {
		[ENOP] = "SUCCESS: Vse v poradku",
		[ENOTFOUND] = "NOTFOUND",
		[EEMPTY] = "EEMPTY",
		[ELEX] = "ERROR: Lexikalni analyza",
		[ESYN] = "ERROR: Syntakticka analyza",
		[ESEM_DEF] = "ERROR: Semanticka analyza, nedefinovana funkce/redefinice",
		[ESEM_TYP] = "ERROR: Semanticka analyza, chyba typove kompatibility",
		[ETYP] = "ERROR: Chyba pri odvozovani datoveho typu promenne",
		[ESEM] = "ERROR: Semanticka analyza",
		[ERUN_IN] = "ERROR: Behova chyba pri nacitani ciselne hodnoty ze vstupu",
		[ERUN_UNINIT] = "ERROR: Behova chyba, prace s neinicializovanou promennou",
		[ERUN_DIV] = "ERROR: Behova chyba deleni nulou",
		[EOTH] = "ERROR: Nespecifikovana behova chyba",
		[EINT] = "ERROR: Interni chyba interpretu"
	};

	// pokud bylo specifikovano cislo radku
	if(line != 0 && code != ENOP)
	{
		fprintf(stderr, "%s, LINE: %d\n", error_msg[code], line);
	}
	else
	{
		fprintf(stderr, "%s\n", error_msg[code]);
	}
	exit(code);
}

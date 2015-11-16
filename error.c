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
#include "error.h"

/**
 * Funkce na vypis chyby
 * @param code Kod chyby
 */
void print_error(TError code) {
	const char *error_msg[] = {
		[ELEX] = "Lexikalni analyza",
		[ESYN] = "Syntakticka analyza",
		[ESEM_DEF] = "Semanticka analyza, nedefinovana funkce/redefinice",
		[ESEM_TYP] = "Semanticka analyza, chyba typove kompatibility",
		[ETYP] = "Chyba pri odvozovani datoveho typu promenne",
		[ESEM] = "Semanticka analyza",
		[ERUN_IN] = "Behova chyba pri nacitani ciselne hodnoty ze vstupu",
		[ERUN_UNINIT] = "Behova chyba, prace s neinicializovanou promennou",
		[ERUN_DIV] = "Behova chyba deleni nulou",
		[EOTH] = "Nespecifikovana behova chyba",
		[EINT] = "Interni chyba interpretu"
	};

	fprintf(stderr, "ERROR: %s\n", error_msg[code]);
	return;
}
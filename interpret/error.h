/**
* -----------------------------------------------------------------------------
*
* Implementace interpretu imperativniho jazyka IFJ15
*
* Soubor       : error.h
* Popis        : Chybové hlášky
* Datum        : Listopad 2015
* Autori       : xvitas02 – Jakub Vitásek
*              
* -----------------------------------------------------------------------------
**/

#ifndef _ERROR_H_
#define _ERROR_H_

typedef enum TError {
	ENOP = 00, // No problem
	ELEX = 10, // Lexikalni analyza
	ESYN = 20, // Syntakticka analyza
	ESEM_DEF = 30, // Semanticka analyza, redefinice
	ESEM_TYP = 40, // Semanticka analyza, typova kompatibilita
	ETYP = 50, // Odvozovani datoveho typu
	ESEM = 60, // Semanticka analyza, ostatni
	ERUN_IN = 70, // Behova chyba, vstup
	ERUN_UNINIT = 80, // Behova chyba, neinicializovana promenna
	ERUN_DIV = 90, // Deleni nulou
	EOTH = 100, // Ostatni chyby
	EINT = 99, // Interni chyba
	EEMPTY = 123, // Epsilon pravidlo
	ENOTFOUND = 321 // Nenalazeno pravidlo
} TError;

/**
 * Funkce na vypis chyby
 * @param code Kod chyby
 */
void print_error(TError code, int line);

#endif

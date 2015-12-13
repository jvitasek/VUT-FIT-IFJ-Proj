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
	ENOP = 0, // No problem
	ELEX = 1, // Lexikalni analyza
	ESYN = 2, // Syntakticka analyza
	ESEM_DEF = 3, // Semanticka analyza, redefinice
	ESEM_TYP = 4, // Semanticka analyza, typova kompatibilita
	ETYP = 5, // Odvozovani datoveho typu
	ESEM = 6, // Semanticka analyza, ostatni
	ERUN_IN = 7, // Behova chyba, vstup
	ERUN_UNINIT = 8, // Behova chyba, neinicializovana promenna
	ERUN_DIV = 9, // Deleni nulou
	EOTH = 10, // Ostatni chyby
	EINT = 99, // Interni chyba
	EEMPTY = 123, // Epsilon pravidlo
	ENOTFOUND = 321 // Nenalazeno pravidlo
} TError;

/**
 * Funkce na vypis chyby a vraceni adekvatniho kodu.
 * @param code Cislo chyby.
 * @param line Cislo radku vyskytu chyby.
 */
void print_error(TError code, int line);

#endif

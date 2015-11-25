/**
* -----------------------------------------------------------------------------
*
* Implementace interpretu imperativniho jazyka IFJ15
*
* Soubor       : ial.c
* Popis        : IAL funkce
* Datum        : Listopad 2015
* Autori       : xvyrou05 - Marek Vyroubal
*              
* -----------------------------------------------------------------------------
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "ial.h"

int HTSIZE = MAX_HTSIZE;


/**
 * -----------------------------------------------------------------------------
 * Vyhledavani podretezce v retezci - Boyer-Mooreuv algoritmus
 * -----------------------------------------------------------------------------
 **/

 // TODO


/**
 * -----------------------------------------------------------------------------
 * Razeni - algoritmus Heap sort
 * -----------------------------------------------------------------------------
 **/

// TODO


/**
 * -----------------------------------------------------------------------------
 * Tabulka symbolu - tabulka s rozptylenymi polozkami
 * -----------------------------------------------------------------------------
 **/

/* 
** Rozptylovaci funkce 
*/
int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky
*/
void htInit ( tHTable* ptrht ) {

	if ((*ptrht) == NULL) {
		return;
	}

	for (int i = 0; i < HTSIZE; i++) {
		(*ptrht)[i] = NULL;
	}
}

/* 
** Vyhledani prvku v TRP ptrht podle zadaného klice key. Pokud je
** dany prvek nalezen, vraci se ukazatel na dany prvek. Pokud prvek nalezen neni, 
** vraci se hodnota NULL.
**
*/
tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	// pokud bude tabulka neinicializovaná
	if ((*ptrht) == NULL) {
		return NULL;
	}

	tHTItem* item; // pomocna promena
	item = (*ptrht)[hashCode(key)]; 

	// prochazi jednotlive synonyma v tabulce, pokud najde synonymum se spravnym klicem vrati ukazatel na nej
	while (item != NULL) {
		if (item -> key == key) {
			return item; // nasli jsme
		}
		else {
			item = item -> ptrnext;
		}
	}

	// klic nenalezen vratime NULL
	return NULL;
}

/* 
** Tato funkce vklada do tabulky ptrht polozku s klicem key a s daty
** data.
*/
void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	if ((*ptrht) == NULL) {
		return;
	}

	tHTItem *item = htSearch(ptrht, key); // najdeme polozku

	if (item != NULL) {		
		// pokud jsem polozku nasel, tak prepisu data
		item -> data = data;
		return;
	}
	else {
		// pokud jsem polozku nenasel, tak vytvorim a zaradim na zacatek
		tHTItem *itemNew = malloc(sizeof(struct tHTItem));
		itemNew -> key = key;
		itemNew -> data = data;
		itemNew -> ptrnext = (*ptrht)[hashCode(key)];
		(*ptrht)[hashCode(key)] = itemNew;
	}
}

/*
** Tato funkce zjistuje hodnotu datove casti polozky zadane klicem.
** Pokud je polozka nalezena, vraci funkce ukazatel na polozku
** Pokud polozka nalezena nebyla, vraci se funkcni hodnota NULL
*/
tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem* item = htSearch(ptrht, key); // najdu prvek podle klice

	if (item != NULL) {
		// nalezen v tabulce zaznam s timto klicem
		return &(item -> data);
	}
	else {
		// polozka nenalezena vratim NULL
		return NULL;
	}
}

/*
** Funkce vyjme polozku s klicem key z tabulky ptrht.  
*/
void htDelete ( tHTable* ptrht, tKey key ) {

	if ((*ptrht) == NULL) {
		return;
	}

	int code = hashCode(key); // vypocitame index do tabulky
	tHTItem* itemDelete = (*ptrht)[code];
	tHTItem* item = itemDelete;

	while (itemDelete != NULL) {
		if (itemDelete -> key == key) {
			if (itemDelete != (*ptrht)[code]) {				
				item -> ptrnext = itemDelete -> ptrnext;
			}
			else {
				(*ptrht)[code] = itemDelete -> ptrnext;
			}
			free(itemDelete);
			return;
		}
		item = itemDelete;
		itemDelete = itemDelete -> ptrnext;
	}
	return;
}

/*
** Funkce zrusi vsechny polozky tabulky
*/
void htClearAll ( tHTable* ptrht ) {

	if ((*ptrht) == NULL) {
		return;
	}

	tHTItem* item; // pomocna promena

	// prochazime pres celou tabulku
	for (int i = 0; i < HTSIZE; i++) {
		while ((*ptrht)[i] != NULL) {
			item = ((*ptrht)[i]) -> ptrnext;
			free((*ptrht)[i]);
			(*ptrht)[i] = item;
		}
		(*ptrht)[i] = NULL;
	}
}

/**
 * [length description]
 * @param  s [description]
 * @return   [description]
 */
int length(string s)
{
	return strlen(s.str);
}


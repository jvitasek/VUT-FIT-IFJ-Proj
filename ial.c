/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * ial.c  -  Zakladni algoritmy
 * 
 * Autori:
 * 			xvyrou05 – Marek Vyroubal
 * 			xvitas02 – Jakub Vitasek
 */


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

/**
 * @todo
 */


/**
 * -----------------------------------------------------------------------------
 * Razeni - algoritmus Heap sort
 * (podle opory IAL)
 * -----------------------------------------------------------------------------
 **/

/**
 * Funkce proseti hromadou.
 * @param a     [description]
 * @param Left  [description]
 * @param Right [description]
 */
void siftDown(char* a, int Left, int Right) {
    int j, i;
    // pomocna promenna tehoz typu jako polozka pole
    char Temp;
    // ridici promenna cyklu
    int Cont = 0;
    i = Left;
    // index leveho syna
    j = 2 * i;
    Temp = a[i];
    Cont = (j <= Right);

    while (Cont){
    	// uzel ma oba synovske uzly
        if (j < Right) {
            // pravy syn je vetsi
            if(a[j] < a[j + 1]) {
                // nastav jako vetsiho z dvojice synu
                j++;
            }
        }

        // prvek Temp jiz byl posunut na sve misto; cyklus konci
        if(Temp >= a[j]) {
            Cont = 0;
        // prvek Temp propada niz, a[j] vyplouva o uroven vys
        }
        else {
            a[i] = a[j];
            // syn se stane otcem pro pristi cyklus
            i = j;
            // pristi levy syn
            j = 2 * i;
            // podmínka : "j není terminální uzel"
            Cont = (j <= Right);
        }

    }
    // konecne umisteni proseteho uzlu
    a[i] = Temp;
}

/**
 * Funkce razeni hromadou heap sort.
 * @param a [description]
 */
void heapSort (char* a) {
	int i, Left, Right;
    int Temp;

    // pocet prvku
    int delka = strlen(a);

    // ustaveni hromady
    Left = (delka / 2);
    Right = (delka - 1);

    for (i = Left; i >= 0; i--) {
    	siftDown(a, i, Right);
    }

    // heapSort
    for (Right = delka-1; Right >= 1; Right--) {
        // vymena korene s akt. poslednim prvkem
        Temp = a[0];
        a[0] = a[Right];
        a[Right] = Temp;
        // znovu ustaveni hromady
        siftDown(a, 0, Right-1);
    }
}


/**
 * -----------------------------------------------------------------------------
 * Tabulka symbolu - tabulka s rozptylenymi polozkami
 * -----------------------------------------------------------------------------
 **/

/**
 * Rozptylovaci funkce.
 * @param  key [description]
 * @return     [description]
 */
int hashCode (tKey key) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/**
 * Inicializace tabulky.
 * @param ptrht [description]
 */
void htInit (tHTable* ptrht) {

	if ((*ptrht) == NULL) {
		return;
	}

	for (int i = 0; i < HTSIZE; i++) {
		(*ptrht)[i] = NULL;
	}
}

/**
 * Vyhledani prvku v TRP ptrht podle zadaneho klice key. Pokud je
 * dany prvek nalezen, vraci se ukazatel na dany prvek. Pokud prvek nalezen neni, 
 * vraci se hodnota NULL.
 * @param  ptrht [description]
 * @param  key   [description]
 * @return       [description]
 */
tHTItem* htSearch (tHTable* ptrht, tKey key) {

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

/**
 * Tato funkce vklada do tabulky ptrht polozku s klicem key a s daty
 * data.
 * @param ptrht [description]
 * @param key   [description]
 * @param data  [description]
 */
void htInsert (tHTable* ptrht, tKey key, tData data) {

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

/**
 * Tato funkce zjistuje hodnotu datove casti polozky zadane klicem.
 * Pokud je polozka nalezena, vraci funkce ukazatel na polozku
 * Pokud polozka nalezena nebyla, vraci se funkcni hodnota NULL
 * @param  ptrht [description]
 * @param  key   [description]
 * @return       [description]
 */
tData* htRead (tHTable* ptrht, tKey key) {

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

/**
 * Funkce vyjme polozku s klicem key z tabulky ptrht. 
 * @param ptrht [description]
 * @param key   [description]
 */
void htDelete (tHTable* ptrht, tKey key) {

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

/**
 * Funkce zrusi vsechny polozky tabulky
 * @param ptrht [description]
 */
void htClearAll (tHTable* ptrht) {

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
 * Vraci pocet znaku retezce.
 * @param  s String k vypoctu znaku.
 * @return   Pocet znaku.
 */
int length(string s)
{
	return strlen(s.str);
}


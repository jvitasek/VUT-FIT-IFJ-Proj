/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * ial.c  -  Zakladni algoritmy
 * 
 * Autori:
 * 			xvyrou05 – Marek Vyroubal
 * 			xvitas02 – Jakub Vitasek
 * 			xvidaj00 – Juraj Vida
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ial.h"

int HTSIZE = MAX_HTSIZE;

string concat(string s1, string s2);
int length(string s);


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
int hashCode ( char *key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/**
 * [htInit  description]
 * @param ptrht [description]
 */
void htInit ( tHTable* ptrht ) {

	if(ptrht) // we have something to work with
	{
		for(int i = 0; i < HTSIZE; i++) // get all the items
			(*ptrht)[i] = NULL; // and set them to null
	}
	else // nothing to do
		return; // end here
}

/**
 * [htSearch  description]
 * @param  ptrht [description]
 * @param  key   [description]
 * @return       [description]
 */
tHTItem* htSearch ( tHTable* ptrht, char *key ) {

	if(ptrht)
	{
		int rkey = hashCode(key); // we decode our key and store it
		tHTItem *temp = (*ptrht)[rkey]; // helper placeholder

		while(temp && (strcmp(temp->key,key) != 0)) // until there is something to search + we haven't yet found it
			temp = temp->ptrnext; // move onto the next value
		return temp; // found it, return it
	}
	else // nothing to do or we ran out of values
		return NULL; // end here, haven't found anything
}

/**
 * [htInsert  description]
 * @param ptrht [description]
 * @param key   [description]
 * @param data  [description]
 */
void htInsert ( tHTable* ptrht, char *key, tData data ) {
	if(ptrht) // it there is something to work with
	{
		tHTItem *temp = NULL; // our helper placeholder
		int rkey = hashCode(key); // decode the key specified

		if((temp = htSearch(ptrht, key))) // we have found our key position
		{
			tHTItem *ntemp = (*ptrht)[rkey]; // new helper placeholder
	
			while(ntemp) // until there is no more items
			{
				if(strcmp(ntemp->key,key) == 0) // if we have found our key position
				{
					ntemp->data.timesUsed = data.timesUsed; // insert the data
					ntemp->data.type = data.type;
					return; // end here
				}
				ntemp = ntemp->ptrnext; // otherwise, move onto the next item
			}
		}
		else // we found an empty place or a synonym
		{
			if(ptrht[rkey]) // empty place
			{
				temp = malloc(sizeof(tHTable));
				if(!temp) // malloc went wrong
					return; // end here
				temp->data.timesUsed = data.timesUsed;
				temp->data.type = data.type;
				temp->key = malloc(sizeof(char)*strlen(key)+1);
				strcpy(temp->key, key);

				temp->ptrnext = (*ptrht)[rkey]; // append our new item
				(*ptrht)[rkey] = temp; // move it to the right position
			}
			else // synonym
			{
				(*ptrht)[rkey] = malloc(sizeof(tHTable)); // allocating a new place
				if(!((*ptrht)[rkey])) // malloc went wrong
					return; // end here
				
				(*ptrht)[rkey]->data.timesUsed = data.timesUsed; // passing the data specified
				(*ptrht)[rkey]->data.type = data.type;
				(*ptrht)[rkey]->key = malloc(sizeof(char)*strlen(key)+1);
				strcpy((*ptrht)[rkey]->key, key);
				(*ptrht)[rkey]->ptrnext = NULL; // nowhere else to go

			}
		}
	}
	else // nothing to work with
		return; // end here
}

/**
 * [htRead  description]
 * @param  ptrht [description]
 * @param  key   [description]
 * @return       [description]
 */
tData* htRead ( tHTable* ptrht, char *key ) {

	if(!ptrht || !((*ptrht)[hashCode(key)]) ) // nowhere to look or not initialized
		return NULL; // end here, let us know we found nothing
	else // we have somewhere to look
	{
		tHTItem *temp = htSearch(ptrht, key); // search for the item with our key

		if(temp) // if we found the item
			return &(temp->data); // return its data
		else // haven't found it
			return NULL; // end here, let us know we found nothing
	}
	return NULL; // end here, let us know we found nothing
}

/**
 * [htDelete  description]
 * @param ptrht [description]
 * @param key   [description]
 */
void htDelete ( tHTable* ptrht, char *key ) {
	if ((*ptrht) == NULL) {
		return;
	}

	int code = hashCode(key); // vypocitame index do tabulky
	tHTItem* itemDelete = (*ptrht)[code];
	tHTItem* item = itemDelete;

	while (itemDelete != NULL) {
		if (itemDelete->key == key) {
			if (itemDelete != (*ptrht)[code]) {				
				item->ptrnext = itemDelete->ptrnext;
			}
			else {
				(*ptrht)[code] = itemDelete->ptrnext;
			}
			free(itemDelete);
			return;
		}
		item = itemDelete;
		itemDelete = itemDelete->ptrnext;
	}
	return;
}

/**
 * [htClearAll  description]
 * @param ptrht [description]
 */
void htClearAll ( tHTable* ptrht ) {

	if(!ptrht) // nothing to clean up
		return; // end here
	else // something to clean up
	{
		for(int i = 0; i < HTSIZE; i++) // cycle to go through everything
		{
			tHTItem *temp = (*ptrht)[i]; // placeholder variable for the cleanup

			while((*ptrht)[i]) // while there is an item
			{
				temp = (*ptrht)[i]; // assign it to the placeholder
				(*ptrht)[i] = (*ptrht)[i]->ptrnext; // move onto the next item
				free(temp); // clean up
			}
		}
	}
}

/**
 * [outputSymbolTable description]
 * @param ptrht [description]
 */
void outputSymbolTable(tHTable* ptrht)
{
	printf ("------------HASH TABLE--------------\n");
	for ( int i=0; i<HTSIZE; i++ ) {
		printf ("%i:",i);
		tHTItem *ptr = malloc(sizeof(tHTItem));
		ptr = (*ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s,%d,%d)", ptr->key, ptr->data.type, ptr->data.timesUsed);
			ptr = ptr->ptrnext;
		}
		printf ("\n");
	}
	printf ("------------------------------------\n");
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

/**
 * Konkatenace dvou retezcu.
 * @param  s1 Prvni retezec.
 * @param  s2 Druhy retezec.
 * @return    Konkatenovane retezce.
 */
string concat(string s1, string s2)
{
	string res;
	strInit(&res);
	/**
	 * @todo malloc by asi mel obsahovat sizeof char krat to co tam je ted
	 */
	char *result = malloc(length(s1)+length(s2)+1); // +1 ukonceni \0
	if(result != NULL)
	{
		strcpy(result, s1.str);
		strcat(result, s2.str);
		res = toString(result);
		return res;
	}
	return res;
}

/**
 * Vrati substring daneho stringu od i do n (basic_string c++).
 * @param  s Zdrojovy string.
 * @param  i Prvni pozice.
 * @param  n Posledni pozice.
 * @return   Nalezeny substring.
 */
string substr(string s, int i, int n)
{
	string res;
	strInit(&res);
	char *input = s.str;
	/**
	 * @todo malloc by asi mel obsahovat sizeof char krat to co tam je ted
	 */
	char *result = malloc(n+1);
	if(result != NULL)
	{
		int index = i;
		int res_index = 0;
		if(length(s) >= n)
		{
			while(input[index] != '\0' && index <= n)
			{
				result[res_index] = input[index];
				++res_index;
				++index;
			}
			result[res_index+1] = '\0';
			res = toString(result);
		}
	}
	return res;
}

/**
 * Najde prvni vyskyt substringu a vrati jeho index.
 * Vyuziva Boyer-Mooreuv algoritmus.
 * @param  s      Haystack.
 * @param  search Needle
 * @return        Pozice prvniho vyskytu
 */
int find(string s, string search)
{
	/**
	 * @todo Jarda
	 */
	return 0;
}

/**
 * Seradi string podle ordinalnich hodnot
 * Vyuziva algoritmus Heap sort.
 * @param  s String k serazeni.
 * @return   Serazeny string.
 */
string sort(string s)
{
	char *temp = s.str;
	heapSort(temp);
	s = toString(temp);
	return s;
}

/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * ial.c  -  Zakladni algoritmy
 * 
 * Autori:
 * 			xvitas02 – Jakub Vitasek
 * 			xvidaj00 – Juraj Vida
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ial.h"

int HTSIZE = MAX_HTSIZE;

/**
 * Funkce proseti hromadou.
 * @param a     Retezec na protrideni.
 * @param Left  Levy index.
 * @param Right Pravy index.
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

    while(Cont)
    {
    	// uzel ma oba synovske uzly
        if (j < Right)
        {
            // pravy syn je vetsi
            if(a[j] < a[j + 1])
            {
                // nastav jako vetsiho z dvojice synu
                j++;
            }
        }

        // prvek Temp jiz byl posunut na sve misto; cyklus konci
        if(Temp >= a[j])
        {
            Cont = 0;
        // prvek Temp propada niz, a[j] vyplouva o uroven vys
        }
        else
        {
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
 * @param a Retezec na protrideni.
 */
void heapSort(char* a)
{
	int i, Left, Right;
    int Temp;

    // pocet prvku
    int delka = strlen(a);

    // ustaveni hromady
    Left = (delka / 2);
    Right = (delka - 1);

    for (i = Left; i >= 0; i--)
    {
    	siftDown(a, i, Right);
    }

    // heapSort
    for (Right = delka-1; Right >= 1; Right--)
    {
        // vymena korene s akt. poslednim prvkem
        Temp = a[0];
        a[0] = a[Right];
        a[Right] = Temp;
        // znovu ustaveni hromady
        siftDown(a, 0, Right-1);
    }
}

/**
 * Rozptylovaci funkce.
 * @param  key Klic polozky.
 * @return     Poradove cislo pro umisteni polozky.
 */
int hashCode(char *key)
{
	int retval = 1;
	int keylen = strlen(key);
	for(int i=0; i<keylen; i++)
	{
		retval += key[i];
	}
	return (retval % HTSIZE);
}

/**
 * Inicializuje tabulku symbolu.
 * @param ptrht Tabulka symbolu.
 */
void htInit(tHTable* ptrht)
{
	// je s cim pracovat
	if(ptrht)
	{
		// vsechny polozky nastavime na null
		for(int i = 0; i < HTSIZE; i++)
		{
			(*ptrht)[i] = NULL;
		}
	}
	else
	{
		// konec
		return;
	}
}

/**
 * Najde polozku podle klice.
 * @param  ptrht Tabulka symbolu.
 * @param  key   Klic polozky.
 * @return       Data polozky.
 */
tHTItem* htSearch(tHTable* ptrht, char *key)
{
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
 * Najde polozku podle klice a poradi.
 * @param  ptrht Tabulka symbolu.
 * @param  key   Klic polozky.
 * @param  order Poradi.
 * @return       Data polozky.
 */
tHTItem* htSearchOrder(tHTable* ptrht, char *key, int order)
{
	if(ptrht)
	{
		int rkey = hashCode(key); // we decode our key and store it
		tHTItem *temp = (*ptrht)[rkey]; // helper placeholder
		while(temp && (strcmp(temp->key,key) == 0)) // until there is something to search + we haven't yet found it
		{
			if(temp->data.orderParams == order)
			{
				return temp;
			}
			temp = temp->ptrnext; // move onto the next value
		}
		return temp; // found it, return it
	}
	else // nothing to do or we ran out of values
		return NULL; // end here, haven't found anything
}

/**
 * Najde polozku podle klice a cisla ramce.
 * @param  ptrht Tabulka symbolu.
 * @param  key   Klic polozky.
 * @param  scope Cislo ramce.
 * @return       Data polozky.
 */
tHTItem* htSearchScope(tHTable* ptrht, char *key, int scope)
{
	if(ptrht)
	{
		int rkey = hashCode(key); // we decode our key and store it
		tHTItem *temp = (*ptrht)[rkey]; // helper placeholder
		while(temp && (strcmp(temp->key,key) == 0)) // until there is something to search + we haven't yet found it
		{
			#ifdef DEBUG
			fprintf(stderr, "proveruji %s, scope: %d\n", key, temp->data.scope);
			#endif
			if(temp->data.scope == scope)
			{
				#ifdef DEBUG
				fprintf(stderr, "nasel jsem %s, scope: %d = %d \n", key, temp->data.scope, scope);
				#endif
				return temp;
			}
			temp = temp->ptrnext; // move onto the next value
		}
		return temp; // found it, return it
	}
	else // nothing to do or we ran out of values
		return NULL; // end here, haven't found anything
}

/**
 * Vlozi data, pokud polozku s klicem najde, vlozi synonymum.
 * @param ptrht Tabulka symbolu.
 * @param key   Klic polozky.
 * @param data  Data polozky.
 */
void htInsert(tHTable* ptrht, char *key, tData data)
{		
	if(ptrht != NULL)
	{
		tHTItem *tmp;
		tHTItem *new;
		tmp = htSearch(ptrht,key);
		if(tmp != NULL)	// nasli sme hladany prvok, tak prepisem jeho obsah
		{
			new = malloc(sizeof(tHTItem));	// alokujeme miesto pre novu polozku
			if(new != NULL)
			{
				new->data.timesUsed = data.timesUsed; // passing the data specified
				new->data.type = data.type;
				new->data.retType = data.retType;
				new->data.value = data.value;
				new->data.varType = data.varType;
				new->data.orderParams = data.orderParams;
				new->data.isDefined = data.isDefined;
				new->data.isDeclared = data.isDeclared;
				new->data.scope = data.scope;

				//new->key = key;
				new->key = malloc(sizeof(char)*strlen(key)+1);
				strcpy(new->key, key);
				
				new->ptrnext = tmp->ptrnext;
				tmp->ptrnext = new;
			}
		}
		else // nenasli sme
		{
			new = malloc(sizeof(tHTItem));	// alokujeme miesto pre novu polozku
			if(new != NULL)
			{
				int index;
				index = hashCode(key);		// zistime index do tabulky
				new->data.timesUsed = data.timesUsed; // passing the data specified
				new->data.type = data.type;
				new->data.retType = data.retType;
				new->data.value = data.value;
				new->data.varType = data.varType;
				new->data.orderParams = data.orderParams;
				new->data.isDefined = data.isDefined;
				new->data.isDeclared = data.isDeclared;
				new->data.scope = data.scope;
				new->key = malloc(sizeof(char)*strlen(key)+1);
				strcpy(new->key, key);
				new->ptrnext = (*ptrht)[index];	// nastavim ptrnext na dalsiu polozku v retazenych synonymach
				(*ptrht)[index] = new;	// nastavim (*ptrht)[index] na novu polozku
			}
		}
	}
}

/**
 * Prepise data na zadane polozce.
 * @param ptrht Tabulka symbolu.
 * @param key   Klic polozky.
 * @param data  Data polozky.
 */
void htInsertData(tHTable *ptrht, char *key, tData data)
{
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
					ntemp->data.retType = data.retType;
					ntemp->data.value = data.value;
					ntemp->data.varType = data.varType;
					ntemp->data.orderParams = data.orderParams;
					ntemp->data.isDefined = data.isDefined;
					ntemp->data.isDeclared = data.isDeclared;
					ntemp->data.scope = data.scope;
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
				temp->data.retType = data.retType;
				temp->data.value = data.value;
				temp->data.varType = data.varType;
				temp->data.orderParams = data.orderParams;
				temp->data.isDefined = data.isDefined;
				temp->data.isDeclared = data.isDeclared;
				temp->data.scope = data.scope;

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
				(*ptrht)[rkey]->data.retType = data.retType;
				(*ptrht)[rkey]->data.value = data.value;
				(*ptrht)[rkey]->data.varType = data.varType;
				(*ptrht)[rkey]->data.orderParams = data.orderParams;
				(*ptrht)[rkey]->data.isDefined = data.isDefined;
				(*ptrht)[rkey]->data.isDeclared = data.isDeclared;
				(*ptrht)[rkey]->data.scope = data.scope;


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
 * Najde data polozky.
 * @param  ptrht Tabulka symbolu.
 * @param  key   Klic polozky.
 * @return       Data polozky.
 */
tData* htRead(tHTable* ptrht, char *key)
{

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
 * Najde data polozky podle klice a poradi.
 * @param  ptrht Tabulka symbolu.
 * @param  key   Klic polozky.
 * @param  scope Poradi.
 * @return       Data polozky.
 */
tData* htReadOrder(tHTable* ptrht, char *key, int order)
{

	if(!ptrht || !((*ptrht)[hashCode(key)]) ) // nowhere to look or not initialized
		return NULL; // end here, let us know we found nothing
	else // we have somewhere to look
	{
		tHTItem *temp = htSearchOrder(ptrht, key, order); // search for the item with our key

		if(temp) // if we found the item
			return &(temp->data); // return its data
		else // haven't found it
			return NULL; // end here, let us know we found nothing
	}
	return NULL; // end here, let us know we found nothing
}

/**
 * Najde data polozky podle klice a cisla ramce.
 * @param  ptrht Tabulka symbolu.
 * @param  key   Klic polozky.
 * @param  scope Cislo ramce.
 * @return       Data polozky.
 */
tData* htReadScope(tHTable* ptrht, char *key, int scope)
{

	if(!ptrht || !((*ptrht)[hashCode(key)]) ) // nowhere to look or not initialized
		return NULL; // end here, let us know we found nothing
	else // we have somewhere to look
	{
		#ifdef DEBUG
		fprintf(stderr, "hledam: %s ve scopu %d\n", key, scope);
		#endif
		tHTItem *temp = htSearchScope(ptrht, key, scope); // search for the item with our key

		if(temp) // if we found the item
		{
			#ifdef DEBUG
			fprintf(stderr, "nasel jsem: %d\n", scope);
			#endif
			return &(temp->data); // return its data
		}
		else // haven't found it
		{
			#ifdef DEBUG
			fprintf(stderr, "NULL\n");
			#endif
			return NULL; // end here, let us know we found nothing
		}
	}
	return NULL; // end here, let us know we found nothing
}

/**
 * Zrusi polozku v tabulce symbolu.
 * @param ptrht Tabulka symbolu.
 * @param key 	Klic polozky.
 */
void htDelete(tHTable* ptrht, char *key)
{
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
 * Zrusi celou tabulku symbolu.
 * @param ptrht Tabulka symbolu.
 */
void htClearAll(tHTable* ptrht)
{

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

/**
 * [outputSymbolTable description]
 * @param ptrht [description]
 */
void outputSymbolTable(tHTable* ptrht)
{
	printf ("------------HASH TABLE--------------\n");
	for ( int i = 0; i < HTSIZE; i++ ) {
		printf ("%i:", i);
		tHTItem *ptr = malloc(sizeof(tHTItem));
		ptr = (*ptrht)[i];

		while ( ptr != NULL ) {
			if(ptr->data.varType == T_Integ)
			{
				printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d, value: %d)", 
					ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
					ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope,ptr->data.value.i);
			}
			else if(ptr->data.varType == T_Doub)
			{
				printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d, value: %f)", 
					ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
					ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope,ptr->data.value.d);
			}
			else if(ptr->data.varType == T_Str)
			{
				printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d)", 
					ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
					ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope);
			}
			else
			{
				tHTItem *pom = ptr->data.value.ptrTS;
				if(pom != NULL)
				{
					if(pom->data.varType == T_Integ)
					{
						printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d, ptrTS: %p, ptrTSvalue: %d)", 
							ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
							ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope,ptr->data.value.ptrTS, pom->data.value.i);
					}else if(pom->data.varType == T_Doub)
					{
						printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d, ptrTS: %p, ptrTSvalue: %f)", 
							ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
							ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope,ptr->data.value.ptrTS, pom->data.value.d);
					}else if(pom->data.varType == T_Str)
					{
						printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d, ptrTS: %p, ptrTSvalue: %s)", 
							ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
							ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope,ptr->data.value.ptrTS, pom->data.value.str);
					}
				}else
				{
					printf (" (%s, dT: %d, tU: %d, vT: %d, rT: %d, oP: %d, isDef: %d, isDec: %d, sc: %d, ptrTS: %p, ptrTSvalue: %p)", 
						ptr->key, ptr->data.type, ptr->data.timesUsed, ptr->data.varType, ptr->data.retType, ptr->data.orderParams, 
						ptr->data.isDefined, ptr->data.isDeclared, ptr->data.scope,ptr->data.value.ptrTS,pom);
				}
			}
			
			ptr = ptr->ptrnext;
		}
		printf ("\n");
	}
	printf ("------------------------------------\n");
}


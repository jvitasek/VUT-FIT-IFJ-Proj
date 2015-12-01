/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * ial.c  -  Zakladni algoritmy
 * 
 * Autori:
 * 			xvyrou05 – Marek Vyroubal
 * 			xvitas02 – Jakub Vitasek
 */

#ifndef _IAL_H_
#define _IAL_H_

#include "scanner.h"

/**
 * -----------------------------------------------------------------------------
 * Razeni - algoritmus Heap sort
 * -----------------------------------------------------------------------------
 **/

 /* Prototypy funkci */
void heapSort (char* a);
void siftDown (char* a, int Left, int Right);


/**
 * -----------------------------------------------------------------------------
 * Tabulka symbolu - tabulka s rozptylenymi polozkami
 * -----------------------------------------------------------------------------
 **/

/* Maximalni velikost pole pro implementaci vyhledavaci tabulky. */
#define MAX_HTSIZE 101

/**
 * typ symbolu
 */
typedef enum symbolType {
	NONE,
	VAR,
	FUNC
} symbolType;

/**
 * typ obsahu
 */
typedef struct tData {
	symbolType type;
	int timesUsed;
} tData;

/**
 * Datova polozka TRP s explicitne retezenymi synonymy
 */
typedef struct tHTItem {
   char *key;            // klic 
   tData data;          // obsah 
   struct tHTItem* ptrnext;   // ukazatel na dalsi synonymum
} tHTItem;

/* TRP s explicitne zretezenymi synonymy. */
typedef tHTItem* tHTable[MAX_HTSIZE];
extern int HTSIZE;

/* Prototypy funkci */
int hashCode (char *key);
void htInit (tHTable* ptrht);
tHTItem* htSearch (tHTable* ptrht, char *key);
void htInsert (tHTable* ptrht, char *key, tData data);
tData* htRead (tHTable* ptrht, char *key);
void htDelete (tHTable* ptrht, char *key);
void htClearAll (tHTable* ptrht);


// vestavene funkce
int length(string s);
string concat(string s1, string s2);
string substr(string s, int i, int n);
int find(string s, string search);
string sort(string s);

#endif

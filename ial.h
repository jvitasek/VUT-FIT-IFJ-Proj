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

/* typ klice */
typedef char* tKey;

/* typ obsahu */
typedef float tData;

/* Datova polozka TRP s explicitne retezenymi synonymy */
 typedef struct tHTItem{
   tKey key;            // klic 
   tData data;          // obsah 
   struct tHTItem* ptrnext;   // ukazatel na dalsi synonymum
} tHTItem;

/* TRP s explicitne zretezenymi synonymy. */
typedef tHTItem* tHTable[MAX_HTSIZE];
extern int HTSIZE;

/* Prototypy funkci */
int hashCode (tKey key);
void htInit (tHTable* ptrht);
tHTItem* htSearch (tHTable* ptrht, tKey key);
void htInsert (tHTable* ptrht, tKey key, tData data);
tData* htRead (tHTable* ptrht, tKey key);
void htDelete (tHTable* ptrht, tKey key);
void htClearAll (tHTable* ptrht);


// vestavene funkce
/*int length(string s);
string substr(string s, int i, int n);
string concat(string s1, string s2);
int find(string s, string search);
string sort(string s);*/

#endif

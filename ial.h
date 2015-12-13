/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * ial.c  -  Zakladni algoritmy + struktura polozek tabulky symbolu
 * 
 * Autori:
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

union tValue {
	int i;
	double d;
	char str[100];
	void *ptrTS;
	void *ptrI;

};

/**
 * typ obsahu
 */
typedef struct tData {
	char *paramName;
	symbolType type;
	T_Type varType;
	T_Type retType;
	union tValue value;
	int timesUsed;
	int orderParams;
	int isDefined;
	int isDeclared;
	int scope;
} tData;

/**
 * Datova polozka TRP s explicitne retezenymi synonymy
 */
typedef struct HTItem {
   char *key;            // klic 
   tData data;          // obsah 
   struct HTItem* ptrnext;   // ukazatel na dalsi synonymum
} tHTItem;

/* TRP s explicitne zretezenymi synonymy. */
typedef tHTItem* tHTable[MAX_HTSIZE];
extern int HTSIZE;

/* Prototypy funkci */
int hashCode (char *key);
void htInit (tHTable* ptrht);
tHTItem* htSearch (tHTable* ptrht, char *key);
tHTItem* htSearchOrder(tHTable* ptrht, char *key, int order);
tHTItem* htSearchScope(tHTable* ptrht, char *key, int scope);
void htInsert (tHTable* ptrht, char *key, tData data);
void htInsertData(tHTable *ptrht, char *key, tData data);
tData* htRead (tHTable* ptrht, char *key);
tData* htReadOrder(tHTable* ptrht, char *key, int order);
tData* htReadScope(tHTable* ptrht, char *key, int scope);
void htDelete (tHTable* ptrht, char *key);
void htClearAll (tHTable* ptrht);
void arraySearch(int search_pole[], char *search_str, int search_length);
void arrayAscii(int ascii_pole[], char *str, int length);
void outputSymbolTable(tHTable* ptrht);


// vestavene funkce
int length(string s);
string concat(string s1, string s2);
string substr(string s, int i, int n);
string sort(string s);

#endif

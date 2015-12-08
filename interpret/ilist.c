/**
* -----------------------------------------------------------------------------
*
* Implementace interpretu imperativniho jazyka IFJ15
*
* Soubor       : ilist.c
* Popis        : Seznam instrukci
* Datum        : Listopad 2015
* Autori       : xvyrou05 - Marek Vyroubal
*              
* -----------------------------------------------------------------------------
**/

#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "ilist.h"


void listInit(tListOfInstr *L) {
// funkce inicializuje seznam instrukci
  L->First = NULL;
  L->Last = NULL;
  L->Act = NULL;
}

void listFree(tListOfInstr *L) {
// funkce dealokuje seznam instrukci
  tListItem *item;

  while (L->First != NULL) {
    item = L->First;
    L->First = L->First->NextItem;
    // uvolnime celou polozku
    free(item);
  }
}

int listInsertLast(tListOfInstr *L, tInst I) {
// vlozi novou instruci na konec seznamu
  tListItem *newItem;
  newItem = malloc(sizeof(tListItem));
  if (newItem == NULL)
     return EINT;

  newItem->Instruction = I;
  newItem->NextItem = NULL;

  if (L->First == NULL) {
    L->First = newItem;
  }
  else
    L->Last->NextItem = newItem;
  L->Last = newItem;

  return ENOP;
}

void listFirst(tListOfInstr *L) {
// zaktivuje prvni instrukci
  L->Act = L->First;
}

void listNext(tListOfInstr *L) {
// aktivni instrukci se stane nasledujici instrukce
  if (L->Act != NULL)
    L->Act =  L->Act->NextItem;
}

void listGoto(tListOfInstr *L, void *gotoInstr) {
// nastavime aktivni instrukci podle zadaneho ukazatele
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
  L->Act = (tListItem*) gotoInstr;
}

void *listGetPointerLast(tListOfInstr *L) {
// vrati ukazatel na posledni instrukci
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
  return (void*) L->Last;
}

tInst *listGetData(tListOfInstr *L) {
// vrati aktivni instrukci
  if (L->Act == NULL) 
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }

  else return &(L->Act->Instruction);
}


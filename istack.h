/**
* -----------------------------------------------------------------------------
*
* Implementace zasobniku
*
* Soubor       : istack.h
* Popis        : Zasobnik
* Datum        : Prosinec 2015
* Autori       : xvitas02 – Jakub Vitasek
* 				 xvalec00 – Dusan Valecky
*              
* -----------------------------------------------------------------------------
**/

#ifndef ISTACK_H_
#define ISTACK_H_

#include "ial.h"

typedef struct stackItem {
	struct stackItem *Lptr;
	tHTable *table;
} *stackItemPtr;


typedef struct stack {
	stackItemPtr first;
	stackItemPtr top;
} stack;

TError gStackInit(stack *stack);
TError gStackDispose(stack *stack);
TError gStackPop(stack *stack);
TError gStackPush(stack *stack, tHTable *table);

#endif

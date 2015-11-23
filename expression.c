/**
* Implementace interpretu imperativniho jazyka IFJ15 *
* 
* expression.c  -  Zpracování výrazů
* 
* Autori:
* 			xvitas02 – Jakub Vitasek
* 			xvalec00 – Dusan Valecky
*/

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "ial.h"
#include "expression.h"


tHTable* globalTS;
Tsymbols *exprStr;
Tstack stack;
TError error;

int preceden_tab[18][18]= {
//	  +		-			*   /      ==     !=    <=      >=     >      <      id    f    (      )      ,       $
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// +
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// -
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// *
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// /
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// ==
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// !=
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// <=
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// >=
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// >
	{great, great, less, less, great, great, great, great, great, great, less, less, less, empty, empty, great},		// <
	{great, great, less, less, great, great, great, great, great, great, empty, empty, empty, great, great, great},	// id
	{empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, equal, empty, empty, empty},	// f
	{less, less, less, less, less, less, less, less, less, less, less, empty, less, equal, equal, great},					// (
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, great, empty, great},	// )
	{empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, less, less, empty, equal, equal, empty},	// ,
	{less, less, less, less, less, less, less, less, less, less, less, empty, less, less, empty, empty}					// $
};

int StackInit(Tstack *stack)
{
	if (stack == NULL)
	{
		error = ERUN_UNINIT;
		return error;
	}

	stack->top = NULL;
	stack->first = NULL;

	TstackElemPtr tempPtr = NULL;
	tempPtr = malloc(sizeof(struct TstackElem));
	if (tempPtr != NULL)
	{
		tempPtr->Lptr = NULL;
		tempPtr->Rptr = NULL;
		tempPtr->symbols = Tother;
		tempPtr->idType = Tother;
		tempPtr->data = NULL;
		stack->first = tempPtr;
		stack->top = tempPtr;
	}
	else
	{
		error = EINT;
		return error;
	}

	return ENOP;
}


//void StackDispose(Tstack *stack);

/**
 * Vrati dalsi token ze vstupu
 * @param stack
 */
void StackPop(Tstack *stack)
{
	TstackElemPtr tempPtr = NULL;

	if (stack->top != NULL)
	{
		tempPtr = stack->top;
		if (stack->top->data != NULL)
		{
			free(stack->top->data);

		}

		if (stack->top == stack->first)
		{
			stack->top = NULL;
			stack->first = NULL;
		}
		else
		{
			stack->top = stack->top->Lptr;
			stack->top->Rptr = NULL;
		}

		free(tempPtr);
	}
}


/*void StackPush(Tstack *stack, T_Token token);
TstackElemPtr StackTop(Tstack *stack);
void StackShift(Tstack *stack, T_Token token);
void StackEmpty(Tstack *stack);*/



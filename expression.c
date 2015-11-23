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
#include "parser.h"
#include "expression.h"



tHTable* globalTS;
Tsymbols *exprStr;
//Tstack stack;
TError error;

/**
 * 
 */
int preceden_tab[16][16] = {
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

/**
 * Inicializace zasobniku.
 * @param  stack [description]
 * @return       [description]
 */
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

/**
 * Zruseni zasobniku
 * @param stack
 */
void StackDispose(Tstack *stack)
{
	TstackElemPtr tempPtr;
	tempPtr = stack->top;

	while (stack->top != NULL)
	{
		tempPtr = stack->top;
		stack->top = stack->top->Lptr;
		free(tempPtr);
	}
	stack->first = NULL;
}

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

/**
 * Vlozeni tokenu na zasobnik.
 * @param  stack [description]
 * @return       [description]
 */
int StackPush(Tstack *stack/*, T_Token token*/)
{
	TstackElemPtr tempPtr;
	tempPtr = malloc(sizeof(struct TstackElem));

	if (tempPtr == NULL)
	{
		error = ERUN_UNINIT;
		return error;
	}

	tempPtr->idType = Tother;
	tempPtr->Lptr = stack->top;
	tempPtr->Rptr = NULL;
	stack->top->Rptr = tempPtr;
	stack->top = tempPtr;

	error = ENOP;
	return error;
}

/**
 * Vraci token na vrcholu zasobniku.
 * @param  stack [description]
 * @return       [description]
 */
TstackElemPtr StackTop(Tstack *stack);
/*** @TODO ****/

/**
 * [StackInit description]
 * @param  stack [description]
 */
void StackShift(Tstack *stack, T_Token token);
/*** @TODO ****/

/**
 * Kontrola, jestli je zasobnik prazdny.
 * @param  stack [description]
 * @return       [description]
 */
int StackEmpty(Tstack *stack)
{
	if (stack->first == stack->top)
	{
		error = ENOP;
	}
	
	return error;
}

/**
* Funkce precte cely vyraz.
*/
/*int readExpr(FILE *input, string *attr)
{

}*/

/**
 * Simuluje pravidla vyrazu.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError expr(FILE *input, string *attr)
{
	TError error = ESYN;
	#ifdef DEBUG
	printf("expr\n");
	#endif
	error = ENOP;
	
	return error;
}


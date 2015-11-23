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

#define DEBUG 1


tHTable* globalTS;
Tsymbols *exprStr;
//Tstack stack;
TError error;

/**
 * Precedencni tabulka 
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
* Funkce vraci index do tabulky
*/
int getInd(int tokenType)
{
	int index = 0;

	switch (tokenType)
	{
		case T_Id:						//0
		case T_Integ:					// cele cisla
		case T_Doub:	 				// desatinne cisla
		case T_Str: index = Tiden; break;	// retazec 15				
		case T_Plus: index = Tplus; break;		// + 16
		case T_Min: index = Tminus; break;			// - 17
		case T_Mul:	index = Tmul; break;					// * 18
		case T_Div:	index = Tdiv; break;					// / 19
		case T_LessThan: index = Tless; break;			// < 20
		case T_LessEqual: index = TlessEq; break; 	// <= 21
		case T_LeftShift: index = TleftShift; break;	// << 22
		case T_GreaterThan: index = Tgreat; break;	// > 23
		case T_GreaterEqual: index = TgreatEq; break;	// >= 24
		case T_RightShift: index = TrightShift; break;	// >> 25
		case T_Equal: index = Tequal; break;			// == 26
		case T_Assig: index = Tassign; break;			// = 27
		case T_NotEqual: index = TnotEq; break;				// != 28
		case T_Comma: index = Tcomma; break; 					// ,	29
		case T_LeftParenthesis: index = TleftP; break;		// ( 31
		case T_RightParenthesis: index = TrightP; break; 	// ) 32
		case T_EOF: index = Tdollar; break;
		default: index = ESYN; break;
	}

	return index;
}

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


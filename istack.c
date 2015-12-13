/**
* -----------------------------------------------------------------------------
*
* Implementace zasobniku
*
* Soubor       : istack.c
* Popis        : Zasobnik
* Datum        : Prosinec 2015
* Autori       : xvitas02 – Jakub Vitasek
* 				 xvalec00 – Dusan Valecky
*              
* -----------------------------------------------------------------------------
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "istack.h"

/**
 * [gStackInit description]
 * @param  stack [description]
 * @return       [description]
 */
TError gStackInit(stack *stack)
{
	TError error = ENOP;

	#ifdef DEBUG
		printf("StackInit in progress.\n");
	#endif
	if (stack == NULL)
	{
		#ifdef DEBUG
			printf("StackInit stack == NULL.\n");
		#endif
		print_error(EINT, 0);
		exit(EINT);
	}

	stack->top = NULL;
	stack->first = NULL;
	if (((stack->top = malloc(sizeof(struct stackItem))) == NULL) || 
		((stack->first = malloc(sizeof(struct stackItem))) == NULL))
	{
		print_error(EINT, 0);
		exit(EINT);
	}

	stackItemPtr tempPtr = NULL;
	if ((tempPtr = malloc(sizeof(struct stackItem))) == NULL)
	{
		print_error(EINT, 0);
		exit(EINT);
	}

	if (tempPtr != NULL)
	{
		tempPtr->Lptr = NULL;
		tempPtr->table = NULL;
		stack->first = tempPtr;
		stack->top = tempPtr;
	}
	else
	{
		print_error(EINT, 0);
		exit(EINT);
	}
	return error;
}

/**
 * Zruseni zasobniku.
 * @param stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError gStackDispose(stack *stack)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackDispose in progress.\n");
	#endif

	stackItemPtr tempPtr;

	if ((tempPtr = malloc(sizeof(struct stackItem))) == NULL)
	{
		print_error(EINT, 0);
		exit(EINT);
	}

	// postupne rusim vsechny prvky na zasobniku
	while (stack->top != NULL)
	{
		// if (stack->top->data != NULL)
		// {
		// 	free(stack->top->data);
		// }
		tempPtr = stack->top;
		stack->top = stack->top->Lptr;
		free(tempPtr);
	}

	stack->first = NULL;
	stack->top = NULL;

	return error;
}

/**
 * Vrati dalsi token ze vstupu
 * @param stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError gStackPop(stack *stack)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackPop in progress.\n");
	#endif

	stackItemPtr tempPtr = NULL;


	if (stack->top != NULL)
	{
		tempPtr = stack->top;

		if (stack->top->Lptr != NULL)
		{
			#ifdef DEBUG
				printf("StackPop mazu vrchol zasobniku.\n");
			#endif
			stack->top = stack->top->Lptr;
			free(tempPtr);
		}
		else if (stack->top == stack->first)
		{
			#ifdef DEBUG
				printf("StackPop stack->top == stack->first.\n");
			#endif
			stack->top = NULL;
			stack->first = NULL;
		}
		else
		{
			#ifdef DEBUG
				printf("StackPop ... ELSE!!.\n");
			#endif
		}
	}
	else
	{
		fprintf(stderr, "Chyba StackPop!\n");
		error = ESYN;
		return error;
	}

	return error;
}

/**
 * Vlozeni tokenu na zasobnik.
 * @param  stack   Zasobnik termu.
 * @param  tokterm Terminal prevedeny z tokenu.
 * @return error	Navratova hodnota chyby.
 */
TError gStackPush(stack *stack, tHTable *table)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackPush in progress.\n");
	#endif

	struct stackItem *tempPtr;
	if((tempPtr = malloc(sizeof(struct stackItem))) == NULL)
	{
		print_error(EINT, 0);
		exit(EINT);
	}

	tempPtr->table = malloc(sizeof(tHTable));
	memcpy(tempPtr->table, table, sizeof(tHTable));
	tempPtr->Lptr = stack->top;
	stack->top = tempPtr;

	error = ENOP;
	return error;
}

/**
* Implementace interpretu imperativniho jazyka IFJ15 *
* 
* interpret.c  
* 
* Autor:
*       xvidaj00 - Juraj Vida
*/

#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include "ial.h"

TError interpret(tInstList *L)
{
  #ifdef DEBUG
	fprintf(stderr, "Interpret\n");
	printElementsOfList(*L);
  #endif
	listFirst(L);
	tInstruct *ins;
	tHTItem *op1;
	tHTItem *op2;
	tHTItem *res;
	while(1)
	{
		ins = listGetData(L);
		if(ins == NULL) break;
		op1 = (tHTItem*)ins->op1;
		op2 = (tHTItem*)ins->op2;
		res = (tHTItem*)ins->result;
		if(ins->instCode == C_Assign)
		{

			res->data.value.d = op1->data.value.d;
		}
    else if(ins->instCode == C_Add)
		{

			res->data.value.d = op1->data.value.d + op2->data.value.d;
		}
    else if(ins->instCode == C_Sub)
    {
      res->data.value.d = op1->data.value.d - op2->data.value.d;
    }
    else if(ins->instCode == C_Mul)
    {
      res->data.value.d = op1->data.value.d * op2->data.value.d;
    }
    else if(ins->instCode == C_Div)
    {
      res->data.value.d = op1->data.value.d / op2->data.value.d;
    }

		listNext(L);
	}
  #ifdef DEBUG
	fprintf(stderr, "%s %f\n",res->key,res->data.value.d);
  #endif
	return ENOP;
}


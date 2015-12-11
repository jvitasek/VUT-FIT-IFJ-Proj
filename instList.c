/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * instList.c  -  Zoznam instrukcii trojadresneho kodu
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */
 
#include "instList.h"

#include <stdio.h>
#include <stdlib.h>
#include "ial.h"

/**
 * Inicializacia zoznamu
 */
void listInit(tInstList *L)
{
	if (L != NULL)
	{
		L->First = NULL;
		L->Act = NULL;
		L->Last = NULL;
	}
}

/**
 * Rusenie zoznamu
 */
void listDispose(tInstList *L)
{
	if(L != NULL)
	{
		tListItem *tmp;
		while(L->First != NULL)	// dokym nie je zoznam prazdny
		{
			tmp = L->First;
			L->First = L->First->nextItem;
			free(tmp);
		}
		L->Act = NULL;
		L->Last = NULL;
	}
}

/**
 * Vlozi novy prvok na koniec zoznamu
 */
void listInsertLast(tInstList *L, tInstruct I)
{
	if(L != NULL)
	{
		tListItem *newItem;
		newItem = malloc(sizeof(tListItem));
		if(newItem == NULL)
		{
			//chyba
		}else
		{
			newItem->instruct = I;
			//printf("\n\n \tCODE:%d|||||OPE1 %p %d |||||OPE2 %p |||||Vysl %p %d\n\n",I.instCode,(void*)I.op1,*(int *)I.op1,(void*)I.op2,(void*)I.result,*(int *)I.result);
			newItem->nextItem = NULL;
			if(L->First == NULL) L->First = newItem;
			else L->Last->nextItem = newItem;
			L->Last = newItem;
		}
	}

}

/**
 * Nastavi aktivitu zoznamu na jeho prvy prvok
 */
void listFirst(tInstList *L)
{
	if(L != NULL)
	{
		L->Act = L->First;
	}
}


/**
 * Zrusi prvy prvok zoznamu
 */
 /*
void listDeleteFirst(tInstList *L)
{
	if(L != NULL)
	{
		tListItem *tmp;
		if(L->First != NULL)	//ci nie je zoznam prazdny
		{
			if(L->Act == L->First)	//ci je ruseny aktivnym prvok
			{
				L->Act = NULL;
			}
			tmp = L->First;
			L->First = L->First->nextItem;
			free(tmp);
		}
	}else
	{
		//chyba zrejme
	}
}
*/

/**
 * Vlozi novy prvok za aktivny prvok
 */
 /*
void listPostInsert(tInstList *L, tInstCode instCode, void *op1, void *op2, void *result)
{
	if((L != NULL) && (L->Act != NULL))	//ci zoznam nie je prazdny a ci obsahuje aktivny prvok
	{
		tListItem *newItem;
		newItem = malloc(sizeof(tListItem));
		if(newItem == NULL)
		{
			//chuba zrejme
		}else
		{
			newItem->instruct.instCode = instCode;
			newItem->instruct.op1 = op1;
			newItem->instruct.op2 = op2;
			newItem->instruct.result = result;
			newItem->nextItem = L->Act->nextItem;
			L->Act->nextItem = newItem;
		}
	}else
	{
		//chyba zrejme
	}
}*/

/**
 * Posunie aktivitu na dalsi prvok
 */
void listNext(tInstList *L)
{
	if(L != NULL)
	{
		if(L->Act != NULL)
		{
			L->Act = L->Act->nextItem;
		}
	}
}

/**
 * Skoci na zadanu adresu intrukcie
 */
void listGoTo(tInstList *L, void *gotoInstr)
{
	if((L != NULL) && (gotoInstr != NULL))
	{
		L->Act = (tListItem*) gotoInstr;
	}
}

/**
 * Funkcia vracia ukazatel na poslednu instrukciu
 */
tInstruct *listGetPointerLast(tInstList *L)
{
	if(L != NULL)
	{
		if(L->Last != NULL)	//ci nie je prazdny
		{
			return &(L->Last->instruct);
		}else
		{
			return NULL;
		}
	}else
	{
		return NULL;
	}
}

/**
 * Funkcia vracia aktivnu instrukciu
 */
tInstruct *listGetData(tInstList *L)
{
	if(L != NULL)
	{
		if(L->Act != NULL)	//ci je nejaka instrukcia aktivna
		{
			return &(L->Act->instruct);
		}else
		{
			return NULL;
		}
	}else
	{
		return NULL;
	}
}






/** PRE TESTOVANIE */
void print_elements_of_list(tInstList L)
{
    int MaxListLength = 100;
	tInstList TempList;
	TempList = L;
	int CurrListLength = 0;
	printf("\n-----------------\n");
	while ((TempList.First!=NULL) && (CurrListLength<MaxListLength))	{
		if(TempList.First->instruct.instCode == C_Assign)
		{
			tHTItem *op1 = TempList.First->instruct.op1;
			tHTItem *res = TempList.First->instruct.result;
			printf("\n \tCODE:%d|OPE1 %s %d ||Vysl %s",C_Assign,op1->key,op1->data.value.i, res->key);
		}
		
		TempList.First=TempList.First->nextItem;
		CurrListLength++;
	}
    if (CurrListLength>=MaxListLength)
        printf("\nList exceeded maximum length!");
	printf("\n-----------------\n");
}

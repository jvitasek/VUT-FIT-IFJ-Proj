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

#include "parser.h"
#include "expression.h"

//#define DEBUG 1

int *counteerVar;	// sluzi pri tvorbe pomocnych premennych

//tHTable* globalTS;
PSymbols *exprStr;
Tstack stack;
PSymbols popped;

/**
 * Precedencni tabulka.
 */
int preceden_tab[16][16] = {
//	  +		-			*   /      ==     !=    <=      >=     >      <      id    f    (      )      ,       $
	{great, great, less, less, great, great, great, great, great, great, less, less, less, great, great, great},		// +
	{great, great, less, less, great, great, great, great, great, great, less, less, less, great, great, great},		// -
	{great, great, great, great, great, great, great, great, great, great, less, less, less, great, great, great},		// *
	{great, great, great, great, great, great, great, great, great, great, less, less, less, great, great, great},		// /
	{less, less, less, less, great, great, great, great, great, great, less, less, less, great, great, great},			// ==
	{less, less, less, less, great, great, great, great, great, great, less, less, less, great, great, great},			// !=
	{less, less, less, less, great, great, great, great, great, great, less, less, less, great, great, great},			// <=
	{less, less, less, less, great, great, great, great, great, great, less, less, less, great, great, great},			// >=
	{less, less, less, less, great, great, great, great, great, great, less, less, less, great, great, great},			// >
	{less, less, less, less, great, great, great, great, great, great, less, less, less, great, great, great},			// <
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, great, great, great},	// id
	{empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, equal, empty, empty, empty},	// f
	{less, less, less, less, less, less, less, less, less, less, less, empty, less, equal, equal, empty},					// (
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, great, empty, great},	// )
	{empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, less, less, empty, equal, equal, empty},	// ,
	{less, less, less, less, less, less, less, less, less, less, less, empty, less, less, empty, empty}					// $
};

/**
 * Inicializace zasobniku.
 * @param  stack Zasobnik termu.
 * @return       ENOP v pripade uspechu.
 */
TError StackInit(Tstack *stack)
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
		error = ERUN_UNINIT;
		return error;
	}

	stack->top = NULL;
	stack->first = NULL;
	if (((stack->top = malloc(sizeof(struct TstackElem))) == NULL) || 
		((stack->first = malloc(sizeof(struct TstackElem))) == NULL))
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		error = ERUN_UNINIT;
		return error;
	}

	TstackElemPtr tempPtr = NULL;
	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		error = ERUN_UNINIT;
		return error;
	}

	if (tempPtr != NULL)
	{
		tempPtr->Lptr = NULL;
		tempPtr->Rptr = NULL;
		tempPtr->termType = PDollar;
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
	#ifdef DEBUG
		printf("StackInit tempPtr->termType: %d.\n", tempPtr->termType);
	#endif
	return error;
}

/**
 * Zruseni zasobniku.
 * @param stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError StackDispose(Tstack *stack)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackDispose in progress.\n");
	#endif

	TstackElemPtr tempPtr;

	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		exit(EINT);
	}

	// postupne rusim vsechny prvky na zasobniku
	while (stack->top != NULL)
	{
		if (stack->top->data != NULL)
		{
			free(stack->top->data);
		}
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
TError StackPop(Tstack *stack)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackPop in progress.\n");
	#endif

	TstackElemPtr tempPtr = NULL;

	if (stack->top != NULL)
	{
		tempPtr = stack->top;

		if (stack->top->data != NULL)
		{
			free(stack->top->data);
		}

		if (stack->top->Lptr != NULL)
		{
			#ifdef DEBUG
				printf("StackPop mazu vrchol zasobniku.\n");
			#endif
			stack->top = stack->top->Lptr;
			stack->top->Rptr = NULL;
			popped = tempPtr->termType;
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
	}

	return error;
}

/**
 * Vlozeni tokenu na zasobnik.
 * @param  stack   Zasobnik termu.
 * @param  tokterm Terminal prevedeny z tokenu.
 * @return error	Navratova hodnota chyby.
 */
TError StackPush(Tstack *stack, int tokterm)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackPush in progress.\n");
	#endif

	TstackElemPtr tempPtr = NULL;
	
	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		error = ERUN_UNINIT;
		return error;
	}

	#ifdef DEBUG
		printf("StackPush - vkladam termType: %d\n", tempPtr->termType);
	#endif

	tempPtr->termType = tokterm;

	// vkladame teprve 1. prvek
	if (stack->top->Lptr == NULL)
	{
		stack->first->Rptr = tempPtr;
		tempPtr->Lptr = stack->first;
		tempPtr->Rptr = NULL;
		stack->top = tempPtr;
	}
	else
	{
		stack->top->Rptr = tempPtr;
		tempPtr->Lptr = stack->top;
		tempPtr->Rptr = NULL;
		stack->top = tempPtr;
	}

	error = ENOP;
	return error;
}

/**
 * Vraci terminal nejblize vrcholu zasobniku.
 * @param  stack Zasobnik termu.
 * @return       Term nejblize vrcholu.
 */
TstackElemPtr StackTop(Tstack *stack)
{
	#ifdef DEBUG
		printf("StackTop in progress.\n");
	#endif

	TstackElemPtr tempPtr = NULL;
	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		exit(99);
	}

	// terminal je hned na vrcholu zasobniku
	if (stack->top->termType <= PDollar)
	{
		return stack->top;
	}
	// terminal neni na vrcholu zasobniku a existuje prvek vlevo
	else if (stack->top->Lptr != NULL)
	{	
		// prvek pod vrcholem obsahuje terminal
		if (stack->top->Lptr->termType <= PDollar)
		{
			return stack->top->Lptr;
		}
		// vytvorim pomocny prvek (2. ze shora) a hledam terminal hloubeji
		else
		{
			tempPtr = stack->top->Lptr;
			tempPtr->Lptr = stack->top->Lptr->Lptr;
			tempPtr->Rptr = stack->top;
			tempPtr->termType = stack->top->Lptr->termType;
			tempPtr->idType = stack->top->Lptr->idType;
			tempPtr->data = stack->top->Lptr->data;

			while(tempPtr->termType > PDollar)
			{
				// existuje prvek vlevo
				if (tempPtr->Lptr != NULL)
				{
					tempPtr = tempPtr->Lptr;
				}
				// chyba, uz neni kde hledat
				else
				{
					#ifdef DEBUG
						printf("StackTop CHYBA.\n");
					#endif

					tempPtr = NULL;
					return tempPtr;
				}
			}
		}
	}
	// chyba
	else
	{
		#ifdef DEBUG
			printf("StackTop CHYBA.\n");
		#endif

		tempPtr = NULL;
		return tempPtr;
	}

	return tempPtr;
}

/**
 * Pridani handle na zasobnik a pushnuti tokenu.
 * @param  stack Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError StackShift(Tstack *stack, int tokterm)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackShift in progress.\n");
	#endif

	TstackElemPtr temp = NULL;
	TstackElemPtr tempTop = StackTop(stack); // zasobnik s nejvrchnejsim termem
	if(tempTop == NULL)
	{
		error = ESYN;
		return error;
	}

	if ((temp = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		error = ERUN_UNINIT;
		return error;
	}

	// vlozeni handle za term
	temp->termType = PLessReduc; 
	temp->idType = Tother;
	temp->data = NULL;
	temp->Lptr = tempTop;
	temp->Rptr = tempTop->Rptr;
	
	if (tempTop == stack->top)
	{
		stack->top = temp;
	}
	else
	{
		tempTop->Rptr->Lptr = temp;
	}

	tempTop->Rptr = temp;

	// pushnu token na zasobnik
	if ((error = StackPush(stack, tokterm)) != ENOP)
	{
		#ifdef DEBUG
		fprintf(stderr, "Chyba pri StackPush.\n");
		#endif
		StackDispose(stack);
		return error;
	}

	error = ENOP;
	return error;
}

/**
 * Kontrola, jestli je zasobnik prazdny.
 * @param  stack Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError StackEmpty(Tstack *stack)
{
	TError error = ENOP;

	if (stack->top == stack->first)
	{
		error = ENOP;
	}
	
	return error;
}

#ifdef DEBUG
/**
 * Pomocna funkce pro vypis celeho zasobniku.
 * @param stack
 */
void whatsInStacks(Tstack *stack)
{
	int i = 1;
	TstackElemPtr temp = stack->top;

	temp->Lptr = stack->top->Lptr;
	temp->Rptr = stack->top->Rptr;
	temp->termType = stack->top->termType;
	temp->idType = stack->top->idType;
	temp->data = stack->top->data;
	
	printf("|--VRCHOL- -top->termType- -%d-\t-top->idType- -%d-\t-akt. token- -%d--|\n"
		"|--VRCHOL- -termType- -%d-\t-idType- -%d-\t\t-akt. token- -%d--|\n", 
		stack->top->termType, stack->top->idType, tokToTerm(token.type),
		temp->termType, temp->idType, tokToTerm(token.type));
	
	while (temp->Lptr != NULL)
	{
		temp = temp->Lptr;
		printf("|--%d.-\t\t-termType- -%d-\t-idType- -%d-\t\t-akt. token- -%d--|\n", 
			i, temp->termType, temp->idType, tokToTerm(token.type));
		i++;
	}

	printf("|--FIRST- -first->termType- -%d- -first->idType- -%d-\t-akt. token- -%d--|\n", 
		stack->first->termType, stack->first->idType, tokToTerm(token.type));
}
#endif

/**
* Funkce vraci index do tabulky
* @param  tokenType	Typ tokenu.
* @return index 		PSymbol z tokenu.
*/
int tokToTerm(int tokenType)
{
	int index = 0;

	switch (tokenType)
	{
		case T_Id:						//0
		case T_Integ:					// cele cisla
		case T_Doub:	 				// desatinne cisla
		case T_Str: index = PIden; break;	// retazec 15				
		case T_Plus: index = PPlus; break;		// + 16
		case T_Min: index = PMinus; break;			// - 17
		case T_Mul:	index = PMul; break;					// * 18
		case T_Div:	index = PDiv; break;					// / 19
		case T_LessThan: index = PLess; break;			// < 20
		case T_LessEqual: index = PLessEq; break; 	// <= 21
		case T_GreaterThan: index = PGreat; break;	// > 23
		case T_GreaterEqual: index = PGreatEq; break;	// >= 24
		case T_Equal: index = PEqual; break;			// == 26
		case T_NotEqual: index = PNotEq; break;				// != 28
		case T_Comma: index = PComma; break; 					// ,	29
		case T_LeftParenthesis: index = PLeftP; break;		// ( 31
		case T_RightParenthesis: index = PRightP; break; 	// ) 32
		case T_Semicolon: index = PDollar; break;
		default: index = ESYN; break;
	}

	return index;
}

/**
 * Funkce vraci symbol z precedencni tabulky (<, >, = nebo 'empty')
 * @param  ter1  Term na zasobniku.
 * @param  ter2  Term ze vstupu.
 * @return       Vysledny symbol z prec. tabulky.
 */
int getPrecSymbol(int ter1, int ter2)
{
	#ifdef DEBUG
	printf("getPrecSymbol([%d][%d])\n", ter1, ter2);
	#endif
	return preceden_tab[ter1][ter2];
}

/**
 * Funkce pro hledani pravidla pro vyrazy.
 * @param  stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError findRule(Tstack *stack, ruleType rule)
{
	TError error = ENOTFOUND;
	TstackElemPtr tempPtr = NULL;

	switch(rule)
	{
		case ADD_RULE:	// E -> E + E
			if ((stack->top->termType != PNonTerm) || 
				(stack->top->Lptr->Lptr->termType != PNonTerm))
			{
				fprintf(stderr, "Chyba pravidla pro scitani.\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// nejdrive se zbavim: < E + E (4x pop)
			StackPop(stack);			
			StackPop(stack);			
			StackPop(stack);			
			StackPop(stack);
			

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo ADD po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case SUB_RULE:	// E -> E - E
			if ((stack->top->termType != PNonTerm) || 
				(stack->top->Lptr->Lptr->termType != PNonTerm))
			{
				fprintf(stderr, "Chyba pravidla pro odcitani.\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */			
			
			// nejdrive se zbavim: < E - E (4x pop)
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo SUB_RULE po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case MUL_RULE:	// E -> E * E
			if ((stack->top->termType != PNonTerm) || 
				(stack->top->Lptr->Lptr->termType != PNonTerm))
			{
				fprintf(stderr, "Chyba pravidla pro nasobeni.\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */			
			
			// nejdrive se zbavim: < E * E (4x pop)
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo MUL_RULE po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case DIV_RULE:	// E -> E / E
			if ((stack->top->termType != PNonTerm) || 
				(stack->top->Lptr->Lptr->termType != PNonTerm))
			{
				fprintf(stderr, "Chyba pravidla pro deleni.\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// nejdrive se zbavim: < E / E (4x pop)
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo DIV_RULE po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case LESSGREAT_RULE: // E -> E > E, E -> E >= E, ...
			if ((stack->top->termType != PNonTerm) || 
				(stack->top->Lptr->Lptr->termType != PNonTerm))
			{
				fprintf(stderr, "Chyba pravidla pro porovnavani ( <, > ).\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// nejdrive se zbavim: < E (<, >, <=, >=) E (4x pop)
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo LESSGREAT_RULE po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case EQ_RULE:		// E -> E == E, E -> E != E
			if ((stack->top->termType != PNonTerm) || 
				(stack->top->Lptr->Lptr->termType != PNonTerm))
			{
				fprintf(stderr, "Chyba pravidla pro porovnavani ( ==, != ).\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// nejdrive se zbavim: < E (==, !=) E (4x pop)
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo EQ_RULE po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case PAR_RULE:	// E -> (E)
			if (stack->top->Lptr->termType != PNonTerm ||
				stack->top->Lptr->Lptr->termType != PLeftP)
			{
				fprintf(stderr, "Chyba pravidla PAR_RULE.\n");
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// nejdrive se zbavim: <, (, E, ) (4x pop)
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);
			StackPop(stack);

			// pushnu neterminal na zasobnik
			if ((error = StackPush(stack, PNonTerm)) != ENOP)
			{
				fprintf(stderr, "Chyba pri StackPush.\n");
				StackDispose(stack);
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo PAR_RULE po pop a push\n");
				whatsInStacks(stack);
			#endif
		break;

		case ID_E_RULE:	// E -> i ... i pro string, int, double
			if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
			{
				fprintf(stderr, "Chyba pri malloc.\n");
				error = ERUN_UNINIT;
				return error;
			}
			
			tempPtr->termType = PNonTerm;
			tempPtr->idType = stack->top->idType;
			tempPtr->data = NULL; /////////////////// zatim NULL

			// nejdrive se zbavim: < i (2x pop)
			StackPop(stack);			
			StackPop(stack);			

			// nastavim vrchol zasobniku
			tempPtr->Rptr = NULL;
			tempPtr->Lptr = stack->top;
			stack->top->Rptr = tempPtr;
			stack->top = tempPtr;
			#ifdef DEBUG
			 	printf("Pravidlo ID_E_RULE po pop.\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;

		case FUNC_RULE:
			/**
			 *
			 * @TODO pravidlo pro funkci !!!!!!!!
			 * 
			 */
		break;
	}

	return error;
}

/**
 * Funkcia, ktora nam generuje pomocne premenne $x,
 * do ktorych sa budu ukladat medzivypocty pri generovani 3AK.
 * 
 * @param	var	 	String vytvaranej premennej.
 * @param 	counter	Pocitadlo potrebne pri tvorbe pomocnych premennych.
 */
void generateVariable(string *var, int *counter)
{
	strClear(var);
	strAppend(var,'$');
	int i = *counter;
	while(i != 0)
	{
		strAppend(var, (char)(i % 10 + '0'));
		i = i / 10;
	}
	(*counter)++;
}

/**
 * Funkcia, ktora vytvori a vlozi novu instrukciu do zoznamu instrukcii.
 * 
 * @param  instType  Typ danej instrukcie.
 * @param  op1		 Operand1 (ukazatel do tabulky symbolov)
 * @param  op2		 Operand2 (ukazatel do tabulky sumbolov)
 * @param  res		 Vysledok
 */
void generateInst(tInstCode instType, void *op1, void *op2, void *res)
{
	//tInst inst;
	//inst.instType = instType;
	//inst.op1 = op1;
	//inst.op2 = op2;
	//inst.res = res;
	//listInsertLast(list,inst);	// list instrukci trba vytvorit v parsru
}

/**
 * Hlavni funkce vyrazu.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @param  count Pocitadlo potrebne pri tvorbe pomocnych premennych.
 * @return       Index do enumerace chyb.
 */
TError expr(FILE *input, string *attr, int semi_or_par, int *count)
{
	counteerVar = count;
	TError error = ENOTFOUND;
	TstackElemPtr tempStack = NULL;
	int counter = 0; // pro pocitani zavorek
	//char *tempData = NULL;
	int tokterm = 0;

	#ifdef DEBUG
	printf("expr\n");
	#endif

	if ((tempStack = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		error = ERUN_UNINIT;
		StackDispose(&stack);
		return error;
	}

	// inicializace zasobniku 
	if ((error = StackInit(&stack)) != ENOP)
	{
		fprintf(stderr, "Inicializace se nezdarila\n");
		StackDispose(&stack);

		error = EINT;
		return error;
	}

	/**
	 * Zpracovavani vyrazu ukonceneho strednikem.
	 */
	if(semi_or_par == 0)
	{
		#ifdef DEBUG
			printf("Resim vyraz ukonceny strednikem.\n");
		#endif
		
		int index = 0;
		do {
			tempStack = StackTop(&stack); // nejvrchnejsi terminal na zasobniku
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose(&stack);
				return error;
			}

			// prevedu si token na term (PSymbols)
			tokterm = tokToTerm(token.type);

			// kontrola zavorek
			if(tokterm == PRightP)
			{
				counter--;
				#ifdef DEBUG
				printf("##### DEKREMENTUJU COUNTER, NYNI: %d\n", counter);
				#endif
			}
			else if(tokterm == PLeftP)
			{
				counter++;
				#ifdef DEBUG
				printf("##### INKREMENTUJU COUNTER, NYNI: %d\n", counter);
				#endif
			}

			#ifdef DEBUG
				printf("----- ---- --- --%d.-- Cyklus while do-- --- ---- -----\n", index);
				whatsInStacks(&stack);
				printf("-------------------------------------------------------\n");
			#endif

			++index;
			switch (getPrecSymbol(tempStack->termType, tokterm))
			{				
				case equal:
					#ifdef DEBUG
						printf("Case equal.\n");
					#endif
					if ((error = StackPush(&stack, tokterm)) != ENOP)
					{
						fprintf(stderr, "Chyba pri StackPush.\n");
						StackDispose(&stack);
						return error;
					}
					getNextToken(input, attr);
				break;
				case less:
					#ifdef DEBUG
						printf("Case less.\n");
					#endif
					if ((error = StackShift(&stack, tokterm)) != ENOP)
					{
						fprintf(stderr, "Chyba pri StackShift.\n");
						StackDispose(&stack);
						return error;
					}
					getNextToken(input, attr);
				break;
				case great:
					#ifdef DEBUG
						printf("Case great.\n");
					#endif

					switch(tempStack->termType)
					{
						case PPlus:
							if ((error = findRule(&stack, ADD_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PMinus:
							if ((error = findRule(&stack, SUB_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PMul:
							if ((error = findRule(&stack, MUL_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PDiv:
							if ((error = findRule(&stack, DIV_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PLess:
						case PGreat:
						case PLessEq:
						case PGreatEq:
							if ((error = findRule(&stack, LESSGREAT_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PEqual:
						case PNotEq:
							if ((error = findRule(&stack, EQ_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PRightP:
							if ((error = findRule(&stack, PAR_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PIden:
							if ((error = findRule(&stack, ID_E_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PIdFun:
							/**
							 * 
							 * @todo pravidlo f()
							 * 
							 */
						break;
					}
				break;
				case empty:
					if (counter == 0 && tokterm == PDollar)
					{
						#ifdef DEBUG
							printf("Empty - Counter == 0 && tokterm == $.\n");
						#endif
						continue;
					}

					#ifdef DEBUG
						printf("Empty - CHYBA.\n");
					#endif

					fprintf(stderr, "Chyba vyrazu.\n");
					StackDispose(&stack);
					return ESYN;
				break;
				default:
					fprintf(stderr, "Chyba vyrazu.\n");
					error = ESYN;
					StackDispose(&stack);
					return error;
				break;			
			}
			tempStack = StackTop(&stack);
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose(&stack);
				return error;
			}

			#ifdef DEBUG
				printf("<<<<<<<<<< Counter == %d <<<<<<.\n", counter);
			#endif
			
			#ifdef DEBUG
				printf("----- ---- --%d.-- ?? KONEC WHILE DO ?? -- ---- -----\n", index);
				if ((tempStack->termType == PDollar) && (tokToTerm(token.type) == PDollar))
				{
					printf("-KONCIM!!!! Stack top == DOLAR a akt. token == DOLAR -.\n");
				}
				else
				{
					printf("-NE-KONCIM!! protoze stack.top->term: %d a akt. token: %d\n", 
						tempStack->termType, tokToTerm(token.type));
				}
				printf("-------------------------------------------------------\n");
			#endif

		} while(!((tempStack->termType == PDollar) && (tokToTerm(token.type) == PDollar)));
	}
	/**
	 * Zpracovavani vyrazu ukonceneho pravou zavorkou.
	 */
	else if(semi_or_par == 1)
	{
		#ifdef DEBUG
			printf("Resim vyraz ukonceny pravou zavorkou.\n");
		#endif		
		
		counter = 1;
		int index = 0;

		// prevedu si token na term (PSymbols)
		tokterm = tokToTerm(token.type);	

		do {
			tempStack = StackTop(&stack); // nejvrchnejsi terminal na zasobniku
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose(&stack);
				return error;
			}

			if (tokterm != PDollar)
			{
				tokterm = tokToTerm(token.type);
			}
			
			// kontrola zavorek
			if(tokterm == PRightP)
			{
				counter--;
				#ifdef DEBUG
				printf("##### DEKREMENTUJU COUNTER, NYNI: %d, tokterm: %d!!!!\n", counter, tokterm);
				#endif
			}
			else if(tokterm == PLeftP)
			{
				counter++;
				#ifdef DEBUG
				printf("##### INKREMENTUJU COUNTER, NYNI: %d, tokterm: %d!!!\n", counter, tokterm);
				#endif
			}

			#ifdef DEBUG
				printf("----- ---- --- --%d.-- Cyklus while do-- --- ---- -----\n", index);
				whatsInStacks(&stack);
				printf("-------------------------------------------------------\n");
			#endif

			++index;

			switch (getPrecSymbol(tempStack->termType, tokterm))
			{				
				case equal:
					if ((error = StackPush(&stack, tokterm)) != ENOP)
					{
						fprintf(stderr, "Chyba pri StackPush.\n");
						StackDispose(&stack);
						return error;
					}
					getNextToken(input, attr);
				break;
				case less:
					#ifdef DEBUG
						printf("Case less.\n");
					#endif
					if ((error = StackShift(&stack, tokterm)) != ENOP)
					{
						fprintf(stderr, "Chyba pri StackShift.\n");
						StackDispose(&stack);
						return error;
					}
					getNextToken(input, attr);
				break;
				case great:
					#ifdef DEBUG
						printf("Case great.\n");
					#endif

					switch(tempStack->termType)
					{
						case PPlus:
							if ((error = findRule(&stack, ADD_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PMinus:
							if ((error = findRule(&stack, SUB_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PMul:
							if ((error = findRule(&stack, MUL_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PDiv:
							if ((error = findRule(&stack, DIV_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PLess:
						case PGreat:
						case PLessEq:
						case PGreatEq:
							if ((error = findRule(&stack, LESSGREAT_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PEqual:
						case PNotEq:
							if ((error = findRule(&stack, EQ_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PRightP:
							if ((error = findRule(&stack, PAR_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
						case PIden:
							if ((error = findRule(&stack, ID_E_RULE)) != ENOP)
							{
								StackDispose(&stack);
								return error;
							}
						break;
					}
				break;
				case empty:
					#ifdef DEBUG
						printf("Empty - CHYBA.\n");
					#endif

					fprintf(stderr, "Chyba vyrazu.\n");
					StackDispose(&stack);
					return ESYN;
				break;
				default:
					fprintf(stderr, "Chyba vyrazu.\n");
					error = ESYN;
					StackDispose(&stack);
					return error;
				break;			
			}

			tempStack = StackTop(&stack);
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose(&stack);
				return error;
			}

			if (stack.top->termType == PNonTerm && 
				stack.top->Lptr->termType == PDollar && counter <= 0)
 			{
 				error = ENOP;
 				StackDispose(&stack);
				return error;				
			}

			
			#ifdef DEBUG
				printf("----- ---- --%d.-- ?? KONEC WHILE DO ?? -- ---- -----\n", index);
				if ((tempStack->termType == PDollar) && (tokterm == PDollar))
				{
					printf("-KONCIM!!!! Stack top == DOLAR a akt. token == DOLAR -.\n");
				}
				else
				{
					printf("-NE-KONCIM!! protoze stack.top->term: %d a akt. token: %d\n", 
						tempStack->termType, tokterm);
				}
				printf("-------------------------------------------------------\n");
				printf("############# COUTNER STATE: %d #############\n", counter);
			#endif

		} while(!((tempStack->termType == PDollar) && (tokterm == PDollar)));
	}

	StackDispose(&stack);

	return error;
}
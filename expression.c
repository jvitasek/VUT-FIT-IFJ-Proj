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

#define DEBUG 1

int *counteerVar;	// sluzi pri tvorbe pomocnych premennych

//tHTable* globalTS;
PSymbols *exprStr;
Tstack stack;
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
 * @param  stack Zasobnik termu.
 * @return       ENOP v pripade uspechu.
 */
int StackInit(Tstack *stack)
{
	#ifdef DEBUG
		printf("StackInit in progress.\n");
	#endif
	if (stack == NULL)
	{
		error = ERUN_UNINIT;
		return error;
	}

	stack->top = NULL;
	stack->first = NULL;

	TstackElemPtr tempPtr1 = NULL;
	tempPtr1 = malloc(sizeof(struct TstackElem));
	if (tempPtr1 != NULL)
	{
		tempPtr1->Lptr = NULL;
		tempPtr1->Rptr = NULL;
		tempPtr1->termType = PDollar;
		tempPtr1->idType = Tother;
		tempPtr1->data = NULL;
		stack->first = tempPtr1;
		stack->top = tempPtr1;
	}
	else
	{
		error = EINT;
		return error;
	}

	return ENOP;
}

/**
 * Zruseni zasobniku.
 * @param stack 	Zasobnik termu.
 */
void StackDispose(Tstack *stack)
{
	#ifdef DEBUG
		printf("StackDispose in progress.\n");
	#endif
	TstackElemPtr tempPtr2;
	tempPtr2 = stack->top;

	while (stack->top != NULL)
	{
		tempPtr2 = stack->top;
		stack->top = stack->top->Lptr;
		free(tempPtr2);
	}
	stack->first = NULL;
}

/**
 * Vrati dalsi token ze vstupu
 * @param stack 	Zasobnik termu.
 */
void StackPop(Tstack *stack)
{
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
 * @param  stack   Zasobnik termu.
 * @param  tokterm Terminal prevedeny z tokenu.
 * @return       ENOP v pripade uspechu.
 */
int StackPush(Tstack *stack, int tokterm)
{
	#ifdef DEBUG
		printf("StackPush in progress.\n");
	#endif
	TstackElemPtr tempPtr = NULL;
	tempPtr = malloc(sizeof(struct TstackElem));

	if (tempPtr == NULL)
	{
		error = ERUN_UNINIT;
		return error;
	}

	tempPtr->termType = tokterm; ////
	printf("StackPush - vkladam termType: %d\n", tempPtr->termType);
	tempPtr->Lptr = stack->top;
	tempPtr->Rptr = NULL;
	stack->top->Rptr = tempPtr;
	stack->top = tempPtr;

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
	tempPtr = stack->top;
	tempPtr->Lptr = stack->top->Lptr;
	tempPtr->Rptr = stack->top->Rptr;
	tempPtr->termType = stack->top->termType;
	tempPtr->idType = stack->top->idType;
	tempPtr->data = stack->top->data;

	printf("lptr: %d\n", tempPtr->Lptr);

	// na vrcholu zasobniku muze byt i neterm. nebo <
	while (tempPtr->termType > PDollar)
	{
		printf("term type: %d\n", tempPtr->termType);
		if(tempPtr->Lptr != NULL)
		{
			printf("neni null\n");
			tempPtr = tempPtr->Lptr;
		}
		else
		{
			printf("je null\n");
			return tempPtr;
		}
	}

	return tempPtr;
}

/**
 * Pridani handle na zasobnik a pushnuti tokenu.
 * @param  stack Zasobnik termu.
 * @return       ENOP v pripade uspechu.
 */
int StackShift(Tstack *stack, int tokterm)
{
	#ifdef DEBUG
		printf("StackShift in progress.\n");
	#endif

	TstackElemPtr temp = NULL;
	TstackElemPtr tempTop = StackTop(stack); // zasobnik s nejvrchnejsim termem

	if ((temp = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		error = ERUN_UNINIT;
		return error;
	}

	// vlozeni handle za term
	temp->termType = PLessReduc; 
	temp->data = NULL;
	temp->Lptr = tempTop;
	temp->Rptr = tempTop->Rptr;
	temp->idType = Tother;
	
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
		fprintf(stderr, "Chyba pri StackPush.\n");
		StackDispose(stack);
		return error;
	}

	error = ENOP;
	return error;
}

/**
 * Kontrola, jestli je zasobnik prazdny.
 * @param  stack Zasobnik termu.
 * @return       ENOP v pripade true.
 */
int StackEmpty(Tstack *stack)
{
	error = -5;

	if (stack->top == stack->first)
	{
		error = ENOP;
	}
	
	return error;
}

#ifdef DEBUG
/**
 * Pomocna funkce pro vypis celeho zasobniku.
 * @param stack [description]
 */
void whatsInStacks(Tstack *stack)
{
	int i = 1;
	TstackElemPtr temp = stack->first;

	temp->Lptr = stack->first->Lptr;
	temp->Rptr = stack->first->Rptr;
	temp->termType = stack->first->termType;
	temp->idType = stack->first->idType;
	temp->data = stack->first->data;
	
	printf("|--DNO-\t-termType- -%d-\t-idType- -%d-\t-akt. token- -%d--|\n", 
		temp->termType, temp->idType, token.type);
	
	while (temp->Rptr != NULL)
	{
		temp = temp->Rptr;
		printf("|--%d. -\t-termType- -%d-\t-idType- -%d-\t-akt. token- -%d--|\n", 
			i, temp->termType, temp->idType, token.type);
		i++;
	}
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
	printf("[%d][%d]\n", ter1, ter2);
	#endif
	return preceden_tab[ter1][ter2];
}

/**
 * Funkce pro hledani pravidla pro vyrazy.
 * @param  stack Zasobnik termu.
 * @return       ENOP v pripade uspechu.
 */
TError findRule(Tstack *stack, ruleType rule)
{
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
			
			#ifdef DEBUG
			 	printf("Pravidlo add pred pop\n");
				whatsInStacks(stack);
			#endif
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
			 	printf("Pravidlo add po pop a push\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;
		case SUB_RULE:	// E -> E - E

		break;
		case MUL_RULE:	// E -> E * E

		break;
		case DIV_RULE:	// E -> E / E

		break;
		case LESSGREAT_RULE: // E -> E > E, E -> E >= E, ...

		break;
		case EQ_RULE:		// E -> E == E, E -> E != E

		break;
		case PAR_RULE:	// E -> (E)

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
			tempPtr->data = NULL;
			
			#ifdef DEBUG
			 	printf("Pravidlo ID_E_RULE pred pop\n");
				whatsInStacks(stack);
			#endif

			// nejdrive se zbavim: < i (2x pop)
			StackPop(stack);
			StackPop(stack);

			// nastavim vrchol zasobniku
			tempPtr->Rptr = NULL;
			tempPtr->Lptr = stack->top->Rptr;
			stack->top->Rptr = tempPtr;
			stack->top = tempPtr;
			#ifdef DEBUG
			 	printf("Pravidlo ID_E_RULE po pop.\n");
				whatsInStacks(stack);
			#endif

			error = ENOP;
		break;
		case FUNC_RULE:

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
	//char *tempData = NULL;
	int tokterm;

	#ifdef DEBUG
	printf("expr\n");
	#endif

	if ((tempStack = malloc(sizeof(struct TstackElem))) == NULL)
	{
		fprintf(stderr, "Chyba pri malloc.\n");
		error = ERUN_UNINIT;
		return error;
	}
	// 45: <EXPR> -> ( <EXPR> )
	/*if(token.type == T_LeftParenthesis)
	{
		getNextToken(input, attr);
		error = expr(input, attr);
		#ifdef DEBUG
		printf("expr: expr vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			getNextToken(input, attr);
			if(token.type == T_RightParenthesis)
			{
				return ENOP;
			}
			else
			{
				return ESYN;
			}
		}
		else if(error == ESYN)
		{
			return error;
		}
	}*/

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
		//tempStack = StackTop(&stack); // nejvrchnejsi terminal na zasobniku
		
		do {
			//tempStack = NULL;
			tempStack = StackTop(&stack); // nejvrchnejsi terminal na zasobniku

			// prevedu si token na term (PSymbols)
			tokterm = tokToTerm(token.type);

			#ifdef DEBUG
				whatsInStacks(&stack);
			#endif
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
							/**
							 * @todo pravidlo subRule()
							 */
						break;
						case PMul:
							/**
							 * @todo pravidlo mulRule()
							 */
						break;
						case PDiv:
							/**
							 * @todo pravidlo divRule()
							 */
						break;
						case PLess:
						case PGreat:
						case PLessEq:
						case PGreatEq:
							/**
							 * @todo pravidlo comp1Rule()
							 */
						break;
						case PEqual:
						case PNotEq:
							/**
							 * @todo pravidlo comp2Rule()
							 */
						break;
						case PRightP:
							/**
							 * @todo pravidlo parRule()
							 */
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

					StackDispose(&stack);
					return ESYN;
				break;
				default:
					// OTHER ALGO
				break;			
			}
			tempStack = StackTop(&stack);
			printf("TOKEN TYPE: %d\n", token.type);

		} while(!((stack.top->termType == PDollar) && (tokToTerm(token.type) == PDollar)));
	}
	/**
	 * Zpracovavani vyrazu ukonceneho pravou zavorkou.
	 */
	else if(semi_or_par == 1)
	{
		#ifdef DEBUG
			printf("Resim vyraz ukonceny pravou zavorkou.\n");
		#endif

		TstackElemPtr temp = NULL; // pomocny zasobnik

		// leva zavorka jiz nactena
		int counter = 1;
		do {
			tempStack = StackTop(&stack); // nejvrchnejsi terminal na zasobniku

			// prevedu si token na term (PSymbols)
			tokterm = tokToTerm(token.type);

			switch (getPrecSymbol(stack.top->termType, tokterm))
			{
				case equal:
					// EQUAL ALGO
					StackPush(&stack, tokterm);
					getNextToken(input, attr);
				break;
				case less:
					// LESS ALGO
					if(token.type == T_LeftParenthesis)
					{
						++counter;
					}
					else if(token.type == T_RightParenthesis)
					{
						--counter;
					}

					StackPush(&stack, PLessReduc);
					StackPush(&stack, tokterm);
					getNextToken(input, attr);
				break;
				case great:
					// GREAT ALGO
					temp->Lptr = NULL;
					temp->Rptr = NULL;
					temp->termType = none;
					temp->idType = Tother;
					temp->data = NULL;
					do
					{
						if(temp->Lptr != NULL)
						{
							temp = temp->Lptr;
						}
						else
						{
							temp = stack.top->Lptr;
						}
					} while(temp->termType != PLessReduc);


					if(temp->Rptr->termType == PIden)
					{
						/**
						 * @todo redukce
						 */
					}
					else
					{
						switch(temp->Rptr->Rptr->termType)
						{
							case PPlus:
								/**
								 * @todo pravidlo addRule()
								 */
							break;
							case PMinus:
								/**
								 * @todo pravidlo subRule()
								 */
							break;
							case PMul:
								/**
								 * @todo pravidlo mulRule()
								 */
							break;
							case PDiv:
								/**
								 * @todo pravidlo divRule()
								 */
							break;
							case PLess:
							case PGreat:
							case PLessEq:
							case PGreatEq:
								/**
								 * @todo pravidlo comp1Rule()
								 */
							break;
							case PEqual:
							case PNotEq:
								/**
								 * @todo pravidlo comp2Rule()
								 */
							break;
							case PRightP:
								/**
								 * @todo pravidlo parRule()
								 */
							break;
							case PIden:
								/**
								 * @todo pravidlo idRule()
								 */
							break;

						}
					}
					free(temp);
				break;
				case empty:
					StackDispose(&stack);
					return ESYN;
				break;
				default:
					// OTHER ALGO
				break;
			}
			#ifdef DEBUG
			whatsInStacks(&stack);
			#endif
		} while((stack.top->termType == PDollar) && (tokToTerm(token.type) == PRightP) && (counter == 0));
	}

	printf("stack termType %d, prave nacteny token %d,\n"
		"stack top %d, stack first %d tokterm %d\n"
		, stack.first->termType, token.type, stack.top->termType, 
		stack.first->termType, tokterm);

	StackDispose(&stack);

	
	/**
	 * @todo dodelat dalsi pravidla, jinak vzdy vraci ENOP
	 */
	error = ENOP;
	return error;
}


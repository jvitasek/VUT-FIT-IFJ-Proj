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
#include <string.h>
#include "error.h"
#include "parser.h"
#include "expression.h"
#include "ial.h"

//#define DEBUG 1

int *counteerVar;	// sluzi pri tvorbe pomocnych premennych
Tstack stack;
tHTable **locTable;
tHTItem **expRes;

/**
 * Precedencni tabulka.
 */
int preceden_tab[19][19] = {
//	  +		  -		*      /     ==     !=    <=      >=     >      <    str   doub  int    id    f    (      )      ,       $
	{great, great, less, less, great, great, great, great, great, great, empty, less, less, less, less, less, great, great, great},		// +
	{great, great, less, less, great, great, great, great, great, great, empty, less, less, less, less, less, great, great, great},		// -
	{great, great, great, great, great, great, great, great, great, great, empty, less, less, less, less, less, great, great, great},	// *
	{great, great, great, great, great, great, great, great, great, great, empty, less, less, less, less, less, great, great, great},	// /
	{less, less, less, less, great, great, great, great, great, great, less, less, less, less, less, less, great, great, great},		// ==
	{less, less, less, less, great, great, great, great, great, great, less, less, less, less, less, less, great, great, great},		// !=
	{less, less, less, less, great, great, great, great, great, great, less, less, less, less, less, less, great, great, great},		// <=
	{less, less, less, less, great, great, great, great, great, great, less, less, less, less, less, less, great, great, great},		// >=
	{less, less, less, less, great, great, great, great, great, great, less, less, less, less, less, less, great, great, great},		// >
	{less, less, less, less, great, great, great, great, great, great, less, less, less, less, less, less, great, great, great},		// <
	{empty, empty, empty, empty, great, great, great, great, great, great, empty, empty, empty, empty, empty, empty, great, great, great},	// string
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, empty, empty, empty, great, great, great},	// double
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, empty, empty, empty, great, great, great},	// int
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, empty, empty, empty, great, great, great},	// id
	{empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, equal, empty, empty, empty},	// f
	{less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, equal, equal, empty},					// (
	{great, great, great, great, great, great, great, great, great, great, empty, empty, empty, empty, empty, empty, great, great, great},	// )
	{less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, equal, equal, empty},				// ,
	{less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, less, empty, empty}				// $
};

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
void generateInst(tInstCode instCode, void *op1, void *op2, void *res)
{
	tInstruct inst;
	inst.instCode = instCode;
	inst.op1 = op1;
	inst.op2 = op2;
	inst.result = res;
	listInsertLast(&List,inst);	// list instrukci trba vytvorit v parsru
}

/**
 * Inicializace zasobniku.
 * @param  stack Zasobnik termu.
 * @return       ENOP v pripade uspechu.
 */
TError StackInit()
{
	TError error = ENOP;

	#ifdef DEBUG
		printf("StackInit in progress.\n");
	#endif

	stack.top = NULL;
	stack.first = NULL;
	if (((stack.top = malloc(sizeof(struct TstackElem))) == NULL) || 
		((stack.first = malloc(sizeof(struct TstackElem))) == NULL))
	{
		StackDispose();
		print_error(EINT, token.line);
	}

	TstackElemPtr tempPtr = NULL;
	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		StackDispose();
		print_error(EINT, token.line);
	}

	if (tempPtr != NULL)
	{
		tempPtr->Lptr = NULL;
		tempPtr->Rptr = NULL;
		tempPtr->termType = PDollar;
		tempPtr->idType = Tother;
		tempPtr->data = "PDollar";
		stack.first = tempPtr;
		stack.top = tempPtr;
	}
	else
	{
		error = EINT;
		return error;
	}

	return error;
}

/**
 * Zruseni zasobniku.
 * @param stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError StackDispose()
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackDispose in progress.\n");
	#endif

	TstackElemPtr tempPtr;

	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		StackDispose();
		print_error(EINT, token.line);
	}

	// postupne rusim vsechny prvky na zasobniku
	while (stack.top != NULL)
	{
		// if (stack->top->data != NULL)
		// {
		// 	free(stack->top->data);
		// }
		tempPtr = stack.top;
		stack.top = stack.top->Lptr;
		free(tempPtr);
	}

	stack.first = NULL;
	stack.top = NULL;

	return error;
}

/**
 * Vrati dalsi token ze vstupu
 * @param stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError StackPop()
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackPop in progress.\n");
	#endif

	TstackElemPtr tempPtr = NULL;


	if (stack.top != NULL)
	{
		tempPtr = stack.top;

		if (stack.top->Lptr != NULL)
		{
			// #ifdef DEBUG
			// 	printf("StackPop mazu vrchol zasobniku.\n");
			// #endif
			stack.top = stack.top->Lptr;
			stack.top->Rptr = NULL;
			free(tempPtr);
		}
		else if (stack.top == stack.first)
		{
			// #ifdef DEBUG
			// 	printf("StackPop stack.top == stack->first.\n");
			// #endif
			stack.top = NULL;
			stack.first = NULL;
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
		#ifdef DEBUG
		fprintf(stderr, "Chyba StackPop!\n");
		#endif
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
TError StackPush(int tokterm, char *attr)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackPush in progress.\n");
	#endif

	TstackElemPtr tempPtr = NULL;
	
	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{

		print_error(EINT, token.line);
	}

	tempPtr->data = malloc(sizeof(char)*strlen(attr));

	tempPtr->termType = tokterm;
	// #ifdef DEBUG
	// 	printf("StackPush - vkladam termType: %d\n", tempPtr->termType);
	// #endif
	if (tokterm == PInt)
	{ // jedna se o integer
		tempPtr->idType = Tint;
		//tempPtr->data = attr;
		strcpy(tempPtr->data, attr);
	}
	else if (tokterm == PDouble)
	{ // jedna se o double
		tempPtr->idType = Tdouble;
		//tempPtr->data = attr;
		strcpy(tempPtr->data, attr);
	}
	else if (tokterm == PString)
	{ // jedna se o string
		tempPtr->idType = Tstring;
		//tempPtr->data = attr;
		strcpy(tempPtr->data, attr);
	}
	else if (tokterm == PIden)
	{ // jedna se o identifikator
		tempPtr->idType = Tid;
		tempPtr->data = "Tid";
	}
	else
	{ // jine
		tempPtr->idType = Tother;
		tempPtr->data = "*UNDEF*";
	}

	// vkladame teprve 1. prvek
	if (stack.top->Lptr == NULL)
	{
		stack.first->Rptr = tempPtr;
		tempPtr->Lptr = stack.first;
		tempPtr->Rptr = NULL;
		stack.top = tempPtr;
		stack.top->data = tempPtr->data; //////
	}
	else
	{
		stack.top->Rptr = tempPtr;
		tempPtr->Lptr = stack.top;
		tempPtr->Rptr = NULL;
		stack.top = tempPtr;
		stack.top->data = tempPtr->data; //////	
	}

	error = ENOP;
	return error;
}

/**
 * Vraci terminal nejblize vrcholu zasobniku.
 * @param  stack Zasobnik termu.
 * @return       Term nejblize vrcholu.
 */
TstackElemPtr StackTop()
{
	#ifdef DEBUG
		printf("StackTop in progress.\n");
	#endif

	TstackElemPtr tempPtr = NULL;
	if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
	{
		//StackDispose();
		print_error(EINT, token.line);
	}

	tempPtr->data = malloc(sizeof(char)*strlen(stack.top->data));

	// terminal je hned na vrcholu zasobniku
	if (stack.top->termType <= PDollar)
	{
		return stack.top;
	}
	// terminal neni na vrcholu zasobniku a existuje prvek vlevo
	else if (stack.top->Lptr != NULL)
	{	
		// prvek pod vrcholem obsahuje terminal
		if (stack.top->Lptr->termType <= PDollar)
		{
			return stack.top->Lptr;
		}
		// vytvorim pomocny prvek (2. ze shora) a hledam terminal hloubeji
		else
		{
			tempPtr = stack.top->Lptr;
			tempPtr->Lptr = stack.top->Lptr->Lptr;
			tempPtr->Rptr = stack.top;
			tempPtr->termType = stack.top->Lptr->termType;
			tempPtr->idType = stack.top->Lptr->idType;
			strcpy(tempPtr->data, stack.top->Lptr->data);
			// tempPtr->data = stack.top->Lptr->data;

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
TError StackShift(int tokterm, char *attr)
{
	TError error = ENOP;
	#ifdef DEBUG
		printf("StackShift in progress.\n");
	#endif

	TstackElemPtr temp = NULL;
	TstackElemPtr tempTop = StackTop(); // zasobnik s nejvrchnejsim termem
	if(tempTop == NULL)
	{
		error = ESYN;
		return error;
	}

	if ((temp = malloc(sizeof(struct TstackElem))) == NULL)
	{

		print_error(EINT, token.line);
	}

	// if ((temp->data = malloc(sizeof(char)*strlen(attr))) == NULL)
	// {
	// 	StackDispose(;
	// 	print_error(EINT, token.line);
	// }

	// vlozeni handle za term
	temp->termType = PLessReduc; 
	temp->idType = Tother;
	temp->data = "PLessReduc";
	temp->Lptr = tempTop;
	temp->Rptr = tempTop->Rptr;
	
	if (tempTop == stack.top)
	{
		stack.top = temp;
	}
	else
	{
		tempTop->Rptr->Lptr = temp;
	}

	tempTop->Rptr = temp;

	// pushnu token na zasobnik
	if ((error = StackPush(tokterm, attr)) != ENOP)
	{
		#ifdef DEBUG
		fprintf(stderr, "Chyba pri StackPush.\n");
		#endif
		
		return error;
	}

	error = ENOP;
	return error;
}

#ifdef DEBUG
/**
 * Pomocna funkce pro vypis celeho zasobniku.
 * @param stack
 */
void whatInStacks()
{
	int i = 1;
	TstackElemPtr temp = stack.top;

	//temp->data = malloc(sizeof(char)*strlen(stack.top->data));

	temp->Lptr = stack.top->Lptr;
	temp->Rptr = stack.top->Rptr;
	temp->termType = stack.top->termType;
	temp->idType = stack.top->idType;
	temp->data = stack.top->data;
	
	printf("|--akt. token- -%d--|\n"
		"|--VRCHOL- -top->termType- -%d-\t-top->idType--%d- -top->data-\"%s\"--|\n"
		"|--VRCHOL- \t-termType- -%d-\t-idType- -%d-\t-data- \"%s\"--|\n", 
		tokToTerm(token.type), stack.top->termType, stack.top->idType, stack.top->data,
		temp->termType, temp->idType, temp->data);
	
	while (temp->Lptr != NULL)
	{
		temp = temp->Lptr;
		printf("|--%d.-\t\t-termType- -%d-\t-idType- -%d-\t-data- \"%s\"--|\n", 
			i, temp->termType, temp->idType, temp->data);
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
		case T_Id: index = PIden; break; 		// 0
		case T_Integ: index = PInt; break;		// cele cisla
		case T_Doub: index = PDouble; break;	// desatinne cisla
		case T_Str: index = PString; break;		// retazec 15			
		case T_Plus: index = PPlus; break;		// + 16
		case T_Min: index = PMinus; break;		// - 17
		case T_Mul:	index = PMul; break;		// * 18
		case T_Div:	index = PDiv; break;		// / 19
		case T_LessThan: index = PLess; break;	// < 20
		case T_LessEqual: index = PLessEq; break; // <= 21
		case T_GreaterThan: index = PGreat; break; // > 23
		case T_GreaterEqual: index = PGreatEq; break; // >= 24
		case T_Equal: index = PEqual; break;		// == 26
		case T_NotEqual: index = PNotEq; break;		// != 28
		case T_Comma: index = PComma; break; 		// ,	29
		case T_LeftParenthesis: index = PLeftP; break;	// ( 31
		case T_RightParenthesis: index = PRightP; break; // ) 32
		case T_Semicolon: index = PDollar; break;
		default: print_error(ESYN, token.line); break;
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
	printf("getPrecSymbol([%d][%d], tj. %d.)\n", ter1, ter2, preceden_tab[ter1][ter2]);
	#endif
	return preceden_tab[ter1][ter2];
}

/**
 * Funkce pro hledani pravidla pro vyrazy.
 * @param  stack 	Zasobnik termu.
 * @return error	Navratova hodnota chyby.
 */
TError findRule(ruleType rule)
{
	TError error = ENOTFOUND;
	TstackElemPtr tempPtr = NULL;

	switch(rule)
	{
		case ADD_RULE:	// E -> E + E
			if ((stack.top->termType != PNonTerm) || 
				(stack.top->Lptr->Lptr->termType != PNonTerm))
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla pro scitani.\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			/**
			 * kontrola typove kompatibility
			 */
			// string + cokoli jineho
			if ((stack.top->idType == Tstring && stack.top->Lptr->Lptr->idType != Tstring) || 
				(stack.top->idType != Tstring && stack.top->Lptr->Lptr->idType == Tstring))
			{
				print_error(ESEM_TYP, token.line);
			}
			// int + double
			else if (stack.top->idType == Tint && stack.top->Lptr->Lptr->idType == Tdouble)
			{
				#ifdef DEBUG
				printf("--Op1 == int, op2 == double.--\n");
				#endif
			}
			// double + int
			else if (stack.top->idType == Tdouble && stack.top->Lptr->Lptr->idType == Tint)
			{
				#ifdef DEBUG
				printf("--Op1 == double, op2 == int.--\n");
				#endif
			}
			else 
			{
			//	printf("###############Chyba Nonterminalu - kompatibilita?.\n"); @TODO vyresit pro ID
			//	print_error(ESEM_TYP, token.line);
			}
			
			// nejdrive se zbavim: < E + E (4x pop)
			StackPop();			
			StackPop();			
			StackPop();			
			StackPop();
			

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo ADD po pop a push\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case SUB_RULE:	// E -> E - E
			if ((stack.top->termType != PNonTerm) || 
				(stack.top->Lptr->Lptr->termType != PNonTerm))
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla pro odcitani.\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */	
			
			/**
			 * kontrola typove kompatibility
			 */
			// string - cokoli jineho
			if ((stack.top->idType == Tstring && stack.top->Lptr->Lptr->idType != Tstring) || 
				(stack.top->idType != Tstring && stack.top->Lptr->Lptr->idType == Tstring))
			{
				print_error(ESEM_TYP, token.line);
			}
			// int - double
			else if (stack.top->idType == Tint && stack.top->Lptr->Lptr->idType == Tdouble)
			{
				#ifdef DEBUG
				printf("--Op1 == int, op2 == double.--\n");
				#endif
			}
			// double - int
			else if (stack.top->idType == Tdouble && stack.top->Lptr->Lptr->idType == Tint)
			{
				#ifdef DEBUG
				printf("--Op1 == double, op2 == int.--\n");
				#endif
			}
			else 
			{
			//	printf("###############Chyba Nonterminalu - kompatibilita?.\n"); @TODO vyresit pro ID
			//	print_error(ESEM_TYP, token.line);
			}		
			
			// nejdrive se zbavim: < E - E (4x pop)
			StackPop();
			StackPop();
			StackPop();
			StackPop();

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo SUB_RULE po pop a push\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case MUL_RULE:	// E -> E * E
			if ((stack.top->termType != PNonTerm) || 
				(stack.top->Lptr->Lptr->termType != PNonTerm))
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla pro nasobeni.\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */		
			
			/**
			 * kontrola typove kompatibility
			 */
			// string * cokoli jineho
			if ((stack.top->idType == Tstring && stack.top->Lptr->Lptr->idType != Tstring) || 
				(stack.top->idType != Tstring && stack.top->Lptr->Lptr->idType == Tstring))
			{
				print_error(ESEM_TYP, token.line);
			}
			// int * double
			else if (stack.top->idType == Tint && stack.top->Lptr->Lptr->idType == Tdouble)
			{
				#ifdef DEBUG
				printf("--Op1 == int, op2 == double.--\n");
				#endif
			}
			// double * int
			else if (stack.top->idType == Tdouble && stack.top->Lptr->Lptr->idType == Tint)
			{
				#ifdef DEBUG
				printf("--Op1 == double, op2 == int.--\n");
				#endif
			}
			else 
			{
			//	printf("###############Chyba Nonterminalu - kompatibilita?.\n"); @TODO vyresit pro ID
			//	print_error(ESEM_TYP, token.line);
			}	
			
			// nejdrive se zbavim: < E * E (4x pop)
			StackPop();
			StackPop();
			StackPop();
			StackPop();

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo MUL_RULE po pop a push\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case DIV_RULE:	// E -> E / E
			if ((stack.top->termType != PNonTerm) || 
				(stack.top->Lptr->Lptr->termType != PNonTerm))
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla pro deleni.\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// kontrola deleni 0
			int nula = 1;
			unsigned int i = 0;

			for (i = 0; i < strlen(stack.top->data); i++)
			{
				if (stack.top->data[i] != '0' && stack.top->data[i] != '.')
				{
					nula = 0;
				}
			}

			if (nula == 1)
			{
				print_error(ERUN_DIV, token.line);
			}

			
			/**
			 * kontrola typove kompatibility
			 */
			// string / cokoli jineho
			if ((stack.top->idType == Tstring && stack.top->Lptr->Lptr->idType != Tstring) || 
				(stack.top->idType != Tstring && stack.top->Lptr->Lptr->idType == Tstring))
			{
				print_error(ESEM_TYP, token.line);
			}
			// int / double
			else if (stack.top->idType == Tint && stack.top->Lptr->Lptr->idType == Tdouble)
			{
				#ifdef DEBUG
				printf("--Op1 == int, op2 == double.--\n");
				#endif
			}
			// double / int
			else if (stack.top->idType == Tdouble && stack.top->Lptr->Lptr->idType == Tint)
			{
				#ifdef DEBUG
				printf("--Op1 == double, op2 == int.--\n");
				#endif
			}
			else 
			{
			//	printf("###############Chyba Nonterminalu - kompatibilita?.\n"); @TODO vyresit pro ID
			//	print_error(ESEM_TYP, token.line);
			}
			
			// nejdrive se zbavim: < E / E (4x pop)
			StackPop();
			StackPop();
			StackPop();
			StackPop();

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo DIV_RULE po pop a push\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case LESSGREAT_RULE: // E -> E > E, E -> E >= E, ...
			if ((stack.top->termType != PNonTerm) || 
				(stack.top->Lptr->Lptr->termType != PNonTerm))
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla pro porovnavani ( <, > ).\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			/**
			 * kontrola typove kompatibility @TODO - pro porovnavani toho bude vic !!!!!!!!!
			 */
			// // string * cokoli jineho
			// if ((stack.top->idType == Tstring && stack.top->Lptr->Lptr->idType != Tstring) || 
			// 	(stack.top->idType != Tstring && stack.top->Lptr->Lptr->idType == Tstring))
			// {
			// 	print_error(ESEM_TYP, token.line);
			// }
			// // int * double
			// else if (stack.top->idType == Tint && stack.top->Lptr->Lptr->idType == Tdouble)
			// {
			// 	#ifdef DEBUG
			// 	printf("--Op1 == int, op2 == double.--\n");
			// 	#endif
			// }
			// // double * int
			// else if (stack.top->idType == Tdouble && stack.top->Lptr->Lptr->idType == Tint)
			// {
			// 	#ifdef DEBUG
			// 	printf("--Op1 == double, op2 == int.--\n");
			// 	#endif
			// }
			// else 
			// {
			// 	printf("###############Chyba Nonterminalu - kompatibilita?.\n"); @TODO vyresit pro ID
			//	print_error(ESEM_TYP, token.line);
			// }	
			
			// nejdrive se zbavim: < E (<, >, <=, >=) E (4x pop)
			StackPop();
			StackPop();
			StackPop();
			StackPop();

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo LESSGREAT_RULE po pop a push\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case EQ_RULE:		// E -> E == E, E -> E != E
			if ((stack.top->termType != PNonTerm) || 
				(stack.top->Lptr->Lptr->termType != PNonTerm))
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla pro porovnavani ( ==, != ).\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			/**
			 * kontrola typove kompatibility @TODO - pro porovnavani toho bude vic !!!!!!!!!
			 */
			// // string * cokoli jineho
			// if ((stack.top->idType == Tstring && stack.top->Lptr->Lptr->idType != Tstring) || 
			// 	(stack.top->idType != Tstring && stack.top->Lptr->Lptr->idType == Tstring))
			// {
			// 	print_error(ESEM_TYP, token.line);
			// }
			// // int * double
			// else if (stack.top->idType == Tint && stack.top->Lptr->Lptr->idType == Tdouble)
			// {
			// 	#ifdef DEBUG
			// 	printf("--Op1 == int, op2 == double.--\n");
			// 	#endif
			// }
			// // double * int
			// else if (stack.top->idType == Tdouble && stack.top->Lptr->Lptr->idType == Tint)
			// {
			// 	#ifdef DEBUG
			// 	printf("--Op1 == double, op2 == int.--\n");
			// 	#endif
			// }
			// else 
			// {
			// 	printf("###############Chyba Nonterminalu - kompatibilita?.\n"); @TODO vyresit pro ID
			//	print_error(ESEM_TYP, token.line);
			// }
			
			// nejdrive se zbavim: < E (==, !=) E (4x pop)
			StackPop();
			StackPop();
			StackPop();
			StackPop();

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo EQ_RULE po pop a push\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case PAR_RULE:	// E -> (E)
			if (stack.top->Lptr->termType != PNonTerm ||
				stack.top->Lptr->Lptr->termType != PLeftP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla PAR_RULE.\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			// nejdrive se zbavim: ), E, (, < (4x pop)
			StackPop();
			StackPop();
			StackPop();
			StackPop();

			// pushnu neterminal na zasobnik
			if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pri StackPush.\n");
				#endif
				
				return error;
			}
			#ifdef DEBUG
			 	printf("Pravidlo PAR_RULE po pop a push\n");
				whatInStacks();
			#endif
		break;

		case ID_E_RULE:	// E -> i ... i pro string, int, double
			if ((tempPtr = malloc(sizeof(struct TstackElem))) == NULL)
			{
				
				print_error(EINT, token.line);
			}
			
			tempPtr->termType = PNonTerm;
			tempPtr->idType = stack.top->idType;
			tempPtr->data = stack.top->data;
			
			//tu musim vygenerovat prvu instrukciu a zaroven vlozit do TS tu pomocnu premennu
			tData data;
			string newVar;
			strInit(&newVar);
			if(tempPtr->idType == 2)
			{
				generateVariable(&newVar,counteerVar);
				data.varType = T_Integ;
				data.value.i = atoi(tempPtr->data);
				htInsert(*locTable,tempPtr->data,data);		// vlozim do TS kluc napr 5 a hodnota 5
				htInsert(*locTable, newVar.str,data);		// pomocna premenna na vysledok
				tHTItem *op1 = htSearch(*locTable,tempPtr->data);
				tHTItem *res = htSearch(*locTable,newVar.str);
				printf("\n \tCODE:%d|OPE1 %s %d ||Vysl %s",C_Assign,op1->key,op1->data.value.i,res->key);
		
				generateInst(C_Assign,op1,NULL,res);


			}
			else if(tempPtr->idType == 1)
			{
				generateVariable(&newVar,counteerVar);
				data.varType = T_Doub;
				data.value.d = atof(tempPtr->data);
				htInsert(*locTable, newVar.str,data);
			}
			else if(tempPtr->idType == 0)
			{
				generateVariable(&newVar,counteerVar);
				data.varType = T_Str;
				strcpy(data.value.str,tempPtr->data);
				htInsert(*locTable, newVar.str,data);
			}
			*expRes = htSearch(*locTable,newVar.str);
			// TODO vygenerovat odpovedajucu instrukciu

			// nejdrive se zbavim: < i (2x pop)
			StackPop();			
			StackPop();			

			// nastavim vrchol zasobniku
			tempPtr->Rptr = NULL;
			tempPtr->Lptr = stack.top;
			stack.top->Rptr = tempPtr;
			stack.top = tempPtr;
			#ifdef DEBUG
			 	printf("Pravidlo ID_E_RULE po pop.\n");
				whatInStacks();
			#endif

			error = ENOP;
		break;

		case FUNC_RULE:
			//
			// @ TODO - mozna bude chybet podminka, az budu zpracovavat parametry
			// 
			 
			// jedná se o: <f() - 4x pop
			if (stack.top->Lptr->termType == PLeftP &&	stack.top->Lptr->Lptr->termType == PIdFun)
			{
				// nejdrive se zbavim: ), (, f, < (4x pop)
				StackPop();
				StackPop();
				StackPop();
				StackPop();

				// pushnu neterminal na zasobnik
				if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
				{
					#ifdef DEBUG
					fprintf(stderr, "Chyba pri StackPush.\n");
					#endif
					
					return error;
				}
			}
			// jedna se o <f(E) - 5x pop
			else if(stack.top->Lptr->termType == PNonTerm && stack.top->Lptr->Lptr->termType == PLeftP 
				&&	stack.top->Lptr->Lptr->Lptr->termType == PIdFun)
			{
				// nejdrive se zbavim: ), E, (, f, < (5x pop)
				StackPop();
				StackPop();
				StackPop();
				StackPop();
				StackPop();

				// pushnu neterminal na zasobnik
				if ((error = StackPush(PNonTerm, "PNonTerm")) != ENOP)
				{
					#ifdef DEBUG
					fprintf(stderr, "Chyba pri StackPush.\n");
					#endif
					
					return error;
				}
			}
			else
			{
				#ifdef DEBUG
				fprintf(stderr, "Chyba pravidla PAR_RULE.\n");
				#endif
				error = ESYN;
				return error;
			}

			/**
			 * @todo 3AC, Ilist
			 */
			
			#ifdef DEBUG
			 	printf("Pravidlo FUNC_RULE po pop a push\n");
				whatInStacks();
			#endif
		break;
	}

	return error;
}



/**
 * Hlavni funkce vyrazu.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @param  count Pocitadlo potrebne pri tvorbe pomocnych premennych.
 * @return       Index do enumerace chyb.
 */
TError expr(FILE *input, string *attr, int semi_or_par, int *count, tHTable **localTable, tHTItem **exprRes)
{
	locTable = localTable;
	expRes = exprRes;
	counteerVar = count;
	TError error = ENOTFOUND;
	TstackElemPtr tempStack = NULL;
	tData *tempData; // pro ukladani 
	int prevTok; // predposledni nacteny token
	int counter = 0; // pro pocitani zavorek
	int tokterm = 0; // pro ukladani terminalu z tokenu
	tHTItem *tempItem = NULL;
	char *myAttr = "*UNDEF*";
	
	#ifdef DEBUG
		printf("expr\n");
	#endif

	if ((tempStack = malloc(sizeof(struct TstackElem))) == NULL)
	{
		StackDispose();
		print_error(EINT, token.line);
	}

	// inicializace zasobniku 
	if ((error = StackInit()) != ENOP)
	{
		#ifdef DEBUG
		fprintf(stderr, "Inicializace se nezdarila\n");
		#endif
		StackDispose();

		error = EINT;
		return error;
	}

	if ((myAttr = malloc(sizeof(char)*strGetLength(attr))) == NULL)
	{
		StackDispose();
		print_error(EINT, token.line);
	}

	/**
	 * Zpracovavani vyrazu ukonceneho strednikem.
	 */
	if(semi_or_par == 0)
	{
		#ifdef DEBUG
			printf("################## Resim vyraz ukonceny strednikem. ###################\n");
		#endif

		//outputSymbolTable(*localTable);
		
		int index = 0;
		do {
			tempStack = StackTop(); // nejvrchnejsi terminal na zasobniku
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose();
				return error;
			}

			strcpy(myAttr, strGetStr(attr));

			// prevedu si token na term (PSymbols)
			tokterm = tokToTerm(token.type);

			// zjistim, jestli id je funkce
			if (tokterm == PIden)
			{
				#ifdef DEBUG
					printf("TOKTERM JE ID!!...attr: %s.\n", strGetStr(attr));
				#endif
				if((tempItem = htSearch(*localTable, strGetStr(attr))) != NULL)
				{
					tempData = htRead(*localTable, strGetStr(attr));
					if (tempData->type == FUNC)
					{
						tokterm = PIdFun;
						#ifdef DEBUG
							printf("!!!!ID je FUNKCE!!!!!.\n");
						#endif
					}
					else
					{
						tokterm = PIden;
						#ifdef DEBUG
							printf("!!!!!ID je ID!!!!!!!.\n");
						#endif

						if((tempItem = htSearch(*localTable, strGetStr(attr))) == NULL)
						{
							#ifdef DEBUG
								fprintf(stderr, "Nenasel jsem IDENTIFIKATHOOOR!!!\n");
							#endif

							StackDispose();
							print_error(ESEM_DEF, token.line);
							exit(ESEM_DEF);
						}
					}
				}
				else
				{
					#ifdef DEBUG
						fprintf(stderr, "Nenasel jsem IDENTIFIKATHOOOR!!!\n");
					#endif

					StackDispose();
					print_error(ESEM_DEF, token.line);
					exit(ESEM_DEF);
				}
			}
			

			// kontrola zavorek
			if(tokterm == PRightP)
			{
				counter--;
				#ifdef DEBUG
				printf("dekteremntuju COUNTER, NYNI: %d\n", counter);
				#endif
			}
			else if(tokterm == PLeftP)
			{
				counter++;
				#ifdef DEBUG
				printf("inkrementuju COUNTER, NYNI: %d\n", counter);
				#endif
			}

			#ifdef DEBUG
				printf("----- ---- --- --%d.-- Cyklus while do-- --- ---- -----\n", index);
				whatInStacks();
				printf("-------------------------------------------------------\n");
			#endif



			++index;
			switch (getPrecSymbol(tempStack->termType, tokterm))
			{				
				case equal:
					#ifdef DEBUG
						printf("Case EQUAL.\n");
					#endif
					if ((error = StackPush(tokterm, myAttr)) != ENOP)
					{
						#ifdef DEBUG
						fprintf(stderr, "Chyba pri StackPush.\n");
						#endif
						StackDispose();
						return error;
					}
					getNextToken(input, attr);
				break;
				case less:
					#ifdef DEBUG
						printf("Case LESS.\n");
					#endif
					if ((error = StackShift(tokterm, myAttr)) != ENOP)
					{
						#ifdef DEBUG
						fprintf(stderr, "Chyba pri StackShift.\n");
						#endif
						StackDispose();
						return error;
					}
					getNextToken(input, attr);
				break;
				case great:
					#ifdef DEBUG
						printf("Case GREAT.\n");
					#endif

					switch(tempStack->termType)
					{
						case PPlus:
							if ((error = findRule(ADD_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PMinus:
							if ((error = findRule(SUB_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PMul:
							if ((error = findRule(MUL_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PDiv:
							if ((error = findRule(DIV_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PLess:
						case PGreat:
						case PLessEq:
						case PGreatEq:
							if ((error = findRule(LESSGREAT_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PEqual:
						case PNotEq:
							if ((error = findRule(EQ_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PRightP: // muze se jednat o funkci nebo o pravidlo zavorek
							// funkce ////////////////////////////////////// @TODO PARAMETRY
							if (tempStack->Lptr->Lptr->termType == PIdFun ||
								tempStack->Lptr->Lptr->Lptr->termType == PIdFun || 
								tempStack->Lptr->Lptr->termType == PComma)
							{
								if ((error = findRule(FUNC_RULE)) != ENOP)
								{
									StackDispose();
									return error;
								}
							}
							// zavorky
							else
							{
								if ((error = findRule(PAR_RULE)) != ENOP)
								{
									StackDispose();
									return error;
								}
							}						
						break;
						case PIden:
						case PInt:
						case PDouble:
						case PString:
							if ((error = findRule(ID_E_RULE)) != ENOP)
							{
								StackDispose();
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
						default: break;
					}
				break;
				case empty:
					// kontrola, zda byla funkce deklarovana
					if (prevTok == PIden && tokterm == PLeftP)
					{
						#ifdef DEBUG
							printf("---FUNKCEEEEE!!!!! NEDEKLAROVANAAAAA----:\n");
						#endif	
						StackDispose();
						print_error(ESEM_DEF, token.line);				
					}
					// string + - * / musi hodit ESEM_TYP
					if (prevTok == PString && 
						(tokterm == PPlus || tokterm == PMinus || tokterm == PMul || tokterm == PDiv))
					{
						#ifdef DEBUG
							printf("---String + - * /----:\n");
						#endif	
						StackDispose();
						print_error(ESEM_TYP, token.line);				
					}
					else if (tokterm == PString && 
						(prevTok == PPlus || prevTok == PMinus || prevTok == PMul || prevTok == PDiv))
					{
						#ifdef DEBUG
							printf("---String + - * /----:\n");
						#endif	
						StackDispose();
						print_error(ESEM_TYP, token.line);				
					}

					#ifdef DEBUG
					fprintf(stderr, "Chyba vyrazu!\n");
					#endif
					StackDispose();
					return ESYN;
				break;
				default:
					#ifdef DEBUG
					fprintf(stderr, "Chyba vyrazu.\n");
					#endif
					error = ESYN;
					StackDispose();
					return error;
				break;			
			}
			tempStack = StackTop();
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose();
				return error;
			}

			#ifdef DEBUG
				printf("<<Counter == %d <<<.\n", counter);
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

			prevTok = tokterm;

		} while(!((tempStack->termType == PDollar) && (tokToTerm(token.type) == PDollar)));
	}
	/**
	 * Zpracovavani vyrazu ukonceneho pravou zavorkou.
	 */
	else if(semi_or_par == 1)
	{
		#ifdef DEBUG
			printf("############## Resim vyraz ukonceny pravou zavorkou. ####################\n");
		#endif		
		
		counter = 1;
		int index = 0;

		// prevedu si token na term (PSymbols)
		tokterm = tokToTerm(token.type);	

		do {
			tempStack = StackTop(); // nejvrchnejsi terminal na zasobniku
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose();
				return error;
			}

			strcpy(myAttr, strGetStr(attr));

			if (tokterm != PDollar)
			{
				tokterm = tokToTerm(token.type);

				// zjistim, jestli id je funkce
				if (tokterm == PIden)
				{
					// #ifdef DEBUG
					// 	printf("TOKTERM JE ID!!...attr: %s.\n", strGetStr(attr));
					// #endif
					if((tempItem = htSearch(*localTable, strGetStr(attr))) != NULL)
					{
						tempData = htRead(*localTable, strGetStr(attr));
						if (tempData->type == FUNC)
						{
							tokterm = PIdFun;
							// #ifdef DEBUG
							// 	printf("!!!!ID je FUNKCE!!!!!.\n");
							// #endif
						}
						else
						{
							tokterm = PIden;
							// #ifdef DEBUG
							// 	printf("!!!!!ID je ID!!!!!!!.\n");
							// #endif

							if((tempItem = htSearch(*localTable, strGetStr(attr))) == NULL)
							{
								#ifdef DEBUG
									fprintf(stderr, "Nenasel jsem IDENTIFIKATHOOOR!!!\n");
								#endif

								print_error(ESEM_DEF, token.line);
								exit(ESEM_DEF);
							}
						}
					}
					else
					{
						#ifdef DEBUG
							fprintf(stderr, "Nenasel jsem IDENTIFIKATHOOOR!!!\n");
						#endif

						StackDispose();
						print_error(ESEM_DEF, token.line);
						exit(ESEM_DEF);
					}
				}
			}
			
			// kontrola zavorek
			if(tokterm == PRightP)
			{
				counter--;
				#ifdef DEBUG
				printf("### DEKREMENTUJU COUNTER, NYNI: %d, tokterm: %d!!!!\n", counter, tokterm);
				#endif
			}
			else if(tokterm == PLeftP)
			{
				counter++;
				#ifdef DEBUG
				printf("### INKREMENTUJU COUNTER, NYNI: %d, tokterm: %d!!!\n", counter, tokterm);
				#endif
			}

			#ifdef DEBUG
				printf("----- ---- --- --%d.-- Cyklus while do-- --- ---- -----\n", index);
				whatInStacks();
				printf("-------------------------------------------------------\n");
			#endif

			++index;

			switch (getPrecSymbol(tempStack->termType, tokterm))
			{				
				case equal:
					#ifdef DEBUG
						printf("Case EQUAL.\n");
						printf("Attr: %s\n", strGetStr(attr));
					#endif
					if ((error = StackPush(tokterm, myAttr)) != ENOP)
					{
						#ifdef DEBUG
						fprintf(stderr, "Chyba pri StackPush.\n");
						#endif
						StackDispose();
						return error;
					}
					getNextToken(input, attr);
				break;
				case less:
					#ifdef DEBUG
						printf("Case LESS.\n");
						printf("Attr: %s\n", strGetStr(attr));
					#endif
					if ((error = StackShift(tokterm, myAttr)) != ENOP)
					{
						#ifdef DEBUG
						fprintf(stderr, "Chyba pri StackShift.\n");
						#endif
						StackDispose();
						return error;
					}
					getNextToken(input, attr);
				break;
				case great:
					#ifdef DEBUG
						printf("Case GREAT.\n");
						printf("Attr: %s\n", strGetStr(attr));
					#endif

					switch(tempStack->termType)
					{
						case PPlus:
							if ((error = findRule(ADD_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PMinus:
							if ((error = findRule(SUB_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PMul:
							if ((error = findRule(MUL_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PDiv:
							if ((error = findRule(DIV_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PLess:
						case PGreat:
						case PLessEq:
						case PGreatEq:
							if ((error = findRule(LESSGREAT_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PEqual:
						case PNotEq:
							if ((error = findRule(EQ_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						case PRightP: // muze se jednat o funkci nebo o pravidlo zavorek
							// funkce ////////////////////////////////////// @TODO PARAMETRY
							if (tempStack->Lptr->Lptr->termType == PIdFun ||
								tempStack->Lptr->Lptr->Lptr->termType == PIdFun || 
								tempStack->Lptr->Lptr->termType == PComma)
							{
								if ((error = findRule(FUNC_RULE)) != ENOP)
								{
									StackDispose();
									return error;
								}
							}
							// zavorky
							else
							{
								if ((error = findRule(PAR_RULE)) != ENOP)
								{
									StackDispose();
									return error;
								}
							}						
						break;
						case PIden:
						case PInt:
						case PDouble:
						case PString:
							if ((error = findRule(ID_E_RULE)) != ENOP)
							{
								StackDispose();
								return error;
							}
						break;
						default: break;
					}
				break;
				case empty:
					// kontrola, zda byla funkce deklarovana
					if (prevTok == PIden && tokterm == PLeftP)
					{
						#ifdef DEBUG
							printf("---FUNKCEEEEE!!!!! NEDEKLAROVANAAAAA----:\n");
						#endif	
						StackDispose();
						print_error(ESEM_DEF, token.line);				
					}
					// string + - * / musi hodit ESEM_TYP
					if (prevTok == PString && 
						(tokterm == PPlus || tokterm == PMinus || tokterm == PMul || tokterm == PDiv))
					{
						#ifdef DEBUG
							printf("---String + - * /----:\n");
						#endif	
						StackDispose();
						print_error(ESEM_TYP, token.line);				
					}
					else if (tokterm == PString && 
						(prevTok == PPlus || prevTok == PMinus || prevTok == PMul || prevTok == PDiv))
					{
						#ifdef DEBUG
							printf("---String + - * /----:\n");
						#endif	
						StackDispose();
						print_error(ESEM_TYP, token.line);				
					}

					#ifdef DEBUG
					fprintf(stderr, "Chyba vyrazu.\n");
					#endif
					StackDispose();
					return ESYN;
				break;
				default:
					#ifdef DEBUG
					fprintf(stderr, "Chyba vyrazu.\n");
					#endif
					error = ESYN;
					StackDispose();
					return error;
				break;			
			}

			tempStack = StackTop();
			if(tempStack == NULL)
			{
				error = ESYN;
				StackDispose();
				return error;
			}

			if (stack.top->termType == PNonTerm && 
				stack.top->Lptr->termType == PDollar && counter <= 0)
 			{
 				error = ENOP;
 				StackDispose();
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
				printf("####### COUTNER STATE: %d #######\n", counter);
			#endif

			prevTok = tokterm;

			// #ifdef DEBUG
			// 	printf("---PREV TOK: %d......\n", prevTok);
			// #endif

		} while(!((tempStack->termType == PDollar) && (tokterm == PDollar)));
	}

	StackDispose();

	return error;
}

/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.c  -  Syntakticky analyzator (Parser)
 * 
 * Autori:
 * 			xvitas02 – Jakub Vitasek
 * 			xvalec00 – Dusan Valecky
 */

//#define DEBUG 1
#define DEBUG_SEM 1
//#define DEBUG_INST 1


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "parser.h"
#include "expression.h"
#include "istack.h"

//JARDA
/*#ifdef JARIS
#include "ilist.h"
#include "stack.h"
#endif*/

string attr; // vytvorime si string
int counterVar = 1;	// globalna premenna, ktora sluzi pri tvorbe pomocnych premennych na medzivypocty

tHTItem *idAssign = NULL;
tHTItem *exprRes = NULL;
stack tableStack;
tInstList List;	// zoznam instrukcii
char *tempFunc;

//JARDA
/*#ifdef JARIS
int currScope;
tListOfInstr *list;
union dat_typ_obsah unie;
union dat_typ_obsah unie2;
extern void *lastI;
extern void *afterIf;
extern void *behindFor;
tStackP stackI;
tStackP forstack;
extern void *beforeFor;
extern void *startLab = NULL;
extern void *endOfMain = NULL;

// n - jeste ne
// a - prave se tam nachazim
// y - uz jsem tam byl
char *I_am_in= "n";
#endif*/

/**
 * seznam vestavenych funkci
 */
const char* builtin[BUILTIN] = {
	"length", "concat", "substr",
	"find", "sort"
};

/**
 * @todo deklarace instruction listu
 */
int line = 1;

/**
 * Vrati dalsi token ze vstupu.
 * @param input Soubor obsahujici vstupni kod.
 * @param attr  String lexemu.
 */
void get_next_token(FILE *input, string *attr)
{
	token = getToken(input, attr, &line);
	if(token.type == T_Error)
	{
		print_error(ELEX, token.line);
	}
}

#ifdef JARIS
/**
 * Vlozi novou instrukci do seznamu instrukci
 * @param instType [description]
 * @param typ1     [description]
 * @param op1      [description]
 * @param typ2     [description]
 * @param op2      [description]
 * @param typ3     [description]
 * @param op3      [description]
 */
void generateInstruction(tInstCode instType, TypeI typ1, union dat_typ_obsah *op1, TypeI typ2, union dat_typ_obsah *op2, TypeI typ3, union dat_typ_obsah *op3)
{
   tInst* I = malloc(sizeof(struct tInst));
   I = malloc(sizeof(tInst));
   I->op1 = malloc(sizeof(iOperand));
   I->op2 = malloc(sizeof(iOperand));
   I->res = malloc(sizeof(iOperand));
   
   I->instType = instType;
  
   I->op1->type = typ1;
   I->op1->obsah = *op1;
   I->op2->type = typ2;
   I->op2->obsah = *op2;
   I->res->type = typ3;

   listInsertLast(list, *I);
}
#endif

/**
 * Zkontroluje, zda je vlozeny retezec klicove slovo.
 * @param  attr String lexemu.
 * @return      0 pokud neni, 1 pokud je.
 */
int check_builtin(char *test)
{
	int idx = 0;
	while(idx < BUILTIN)
	{
		if(strcmp(builtin[idx], test) == 0)
		{
			return 1;
		}
		++idx;
	}
	return 0;
}

/**
 * [check_builtin_params description]
 * @param func  [description]
 * @param order [description]
 */
void check_builtin_params(char *func, int order)
{
	if(strcmp(func, "length") == 0)
	{
		if(order != 1)
		{
			print_error(ESEM_TYP, token.line);
		}
	}
	else if(strcmp(func, "substr") == 0)
	{
		if(order != 3)
		{
			print_error(ESEM_TYP, token.line);
		}
	}
	else if(strcmp(func, "concat") == 0)
	{
		if(order != 2)
		{
			print_error(ESEM_TYP, token.line);
		}
	}
	else if(strcmp(func, "find") == 0)
	{
		if(order != 2)
		{
			print_error(ESEM_TYP, token.line);
		}
	}
	else if(strcmp(func, "sort") == 0)
	{
		if(order != 1)
		{
			print_error(ESEM_TYP, token.line);
		}
	}
}

/**
 * [fill_builtin_params description]
 */
void fill_builtin_params()
{
	// LENGTH
	tData data;
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 1;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "length", data); // vkladani do tabulky parametru
	// /LENGTH
	
	// SUBSTR
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 1;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "substr", data); // vkladani do tabulky parametru
	data.type = VAR;
	data.varType = T_Integ; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 2;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "substr", data); // vkladani do tabulky parametru
	data.type = VAR;
	data.varType = T_Integ; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 3;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "substr", data); // vkladani do tabulky parametru
	// /SUBSTR
	
	// CONCAT
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 1;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "concat", data); // vkladani do tabulky parametru
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 2;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "concat", data); // vkladani do tabulky parametru
	// /CONCAT
	
	// FIND
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 1;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "find", data); // vkladani do tabulky parametru
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 2;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "find", data); // vkladani do tabulky parametru
	// /FIND
	
	// SORT
	data.type = VAR;
	data.varType = T_Str; // ulozeni typu parametru
	data.timesUsed = 1;
	data.orderParams = 1;
	data.scope = 1; // nejnizsi scope nasledujiciho bloku
	data.value.ptrTS = NULL;
	data.retType = 0;
	htInsert(paraTable, "sort", data); // vkladani do tabulky parametru
	// /SORT
}

int return_param_count(char *func)
{
	tData *tempData;
	int index = 1;

	while((tempData = htReadOrder(paraTable, func, index)) != NULL)
	{
		index++;
	}
	return index-1;
}

/**
 * Hlavni funkce parseru. Simuluje pravidlo 1.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError parse(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "parse\n");
	#endif
	TError error = ESYN;
	currScope = 0;
	currOrder = 0;
	currOrderTerm = 0;
	isReturn = 0;
	//JARDA
	/*#ifdef JARIS
	SInitP(&stackI);
	SInitP(&forstack);
	#endif*/

	/**
	 * inicializace stringu s nazvem tokenu
	 */
	strInit(&attr);

	/**
	 * inicializace hlavni tabulky symbolu
	 */
	init_table(&commTable);
	init_table(&funcTable);
	init_table(&commTable);
	init_table(&funcTable);
	init_table(&paraTable);

	/**
	 * naplneni parametru pro builtin funkce
	 */
	fill_builtin_params();
	outputSymbolTable(paraTable);

	/**
	 * inicializace zasobniku tabulek symbolu
	 */
	gStackInit(&tableStack);

	get_next_token(input, &attr);
	// prazdny soubor
	if(token.type == T_EOF)
	{
		return ESYN;
	}
	// 1: <PROGRAM> -> <FUNC_N>
	error = func_n(input);
	// SEMANTICKA KONTROLA
	tData *tempData;
	// byla funkce main?
	if(((tempData = htRead(funcTable, "main")) == NULL) && (error == ENOP))
	{
		#ifdef DEBUG_SEM
		fprintf(stderr, "Chyba: v kodu neni main()\n");
		#endif
		print_error(ESEM_DEF, 0);
	}
	// /SEMANTICKA KONTROLA
	#ifdef DEBUG
	fprintf(stderr, "parse: func_n vratilo: %d\n", error);
	#endif

	/**
	 * smazani tabulky symbolu
	 */
	htClearAll(commTable);
	htClearAll(funcTable);
	htClearAll(paraTable);

	/**
	 * smazani zasobniku tabulek symbolu
	 */
	gStackDispose(&tableStack);

	if(error == ENOP || error == EEMPTY)
	{
		error = ENOP;
	}
	else if(error == ENOTFOUND)
	{
		return ESYN;
	}
	//JARDA
	/*#ifdef JARIS
	unie.obsah=4;
	unie2.obsah=5;
	generateInstruction(I_STOP, INT, &unie, INT, &unie2,  DOUBLE, NULL);
	#endif*/
	return error;
}

/**
 * Simuluje pravidla 2 a 3.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError func_n(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "func_n\n");
	#endif
	TError error = ENOTFOUND;
	// 2: <FUNC_N> -> <FUNC> <FUNC_N>
	error = func(input);
	#ifdef DEBUG
	fprintf(stderr, "func_n: func vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		// <FUNC>
		get_next_token(input, &attr);
		error = func_n(input);
		#ifdef DEBUG
		fprintf(stderr, "func_n: func_n vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			// <FUNC> <FUNC_N>
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	else if(error == ESYN)
	{
		return error;
	}
	// 3: <FUNC_N> -> E 
	else if(token.type == T_EOF)
	{
		#ifdef DEBUG
		fprintf(stderr, "konec souboru\n");
		#endif
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidlo 11.
 * @return Index do enumerace chyb.
 */
TError func(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "func\n");
	#endif
	TError error = ENOTFOUND;
	// 11: <FUNC> -> <TYPE> id <PAR_DEF_LIST> <DEC_OR_DEF>
	error = type();
	#ifdef DEBUG
	fprintf(stderr, "func: type vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		get_next_token(input, &attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			// kontrola, jestli nazev neni stejny jako nejaka
			// builtin funkce
			if(check_builtin(strGetStr(&attr)) == 1)
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: funkce %s ma stejny nazev jako vestavena funkce\n", strGetStr(&attr));
				#endif
				print_error(ESEM_DEF, token.line);
			}
			tData *tempData;
			// funkce jiz v tabulce je
			//JARDA
			/*#ifdef JARIS
			fprintf(stderr, "VYPISSSSSSS ? : %s\n",strGetStr(&attr) );
			#endif*/
			if((tempData = htRead(funcTable, strGetStr(&attr))) != NULL)
			{
				if(tempData->retType != currType)
				{
					#ifdef DEBUG_SEM
					fprintf(stderr, "Chyba: navratovy typ funkce nesouhlasi\n");
					#endif
					print_error(ESEM_DEF, token.line);
				}
			}
			// funkce jeste v tabulce neni
			else
			{
				if(strGetStr(&attr) != NULL)
				{
					tData data;
					data.type = FUNC;
					data.retType = currType;
					data.timesUsed = 0;
					data.scope = -1;
					data.isDefined = 0;
					data.isDeclared = 0;
					data.value.ptrTS = NULL;
					htInsert(funcTable, strGetStr(&attr), data);
					#ifdef DEBUG_SEM
					fprintf(stderr, "VKLADAM %s\n", strGetStr(&attr));
					#endif
				}
				//JARDA
				/*#ifdef JARIS
				if(strcmp((strGetStr(&attr)),"main") == 0)
				{
					generateInstruction(I_START, INT, &unie, STRING, &unie2,  DOUBLE, NULL);
					startLab = listGetPointerLast(list);
					printf("startLab: %d\n",startLab );
					I_am_in = "a";
				}
				else if ((I_am_in == "a") && (strcmp((strGetStr(&attr)),"main") != 0))
				{
					I_am_in = "y";
					generateInstruction(I_MAIN_END, INT, &unie, STRING, &unie2,  DOUBLE, NULL);
					endOfMain = listGetPointerLast(list);
					printf("end of main: %d\n", endOfMain);
				}
				printf("VYPISSSSSSS neni : %s\n",strGetStr(&attr) );
				#endif*/
			}
			currFunc = malloc(sizeof(char)*strlen(strGetStr(&attr)));
			strcpy(currFunc, strGetStr(&attr));
			// KONEC SEMANTICKE ANALYZY


			get_next_token(input, &attr);
			error = par_def_list(input);
			#ifdef DEBUG
			fprintf(stderr, "func: par_def_list vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				get_next_token(input, &attr);
				error = dec_or_def(input);

				/**
				 * pokud byla deklarovana, zapsat
				 */
				//outputSymbolTable(funcTable);
				if((tempData = htRead(funcTable, currFunc)) != NULL)
				{
					tData data;
					data.type = tempData->type;
					data.retType = tempData->retType;
					data.timesUsed = tempData->timesUsed;
					data.scope = tempData->scope;
					data.isDefined = tempData->isDefined;
					data.value.ptrTS = tempData->value.ptrTS;
					data.isDeclared = 1;
					htInsertData(funcTable, currFunc, data);
					#ifdef DEBUG_SEM
					fprintf(stderr, "Upravuji %s, isDeclared: %d\n", currFunc, data.isDeclared);
					#endif
					//printf("400 %s\n",strGetStr(&attr));
				}

				#ifdef DEBUG
				fprintf(stderr, "func: dec_or_def vratilo: %d\n", error);
				#endif
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}
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
	return error;
}

/**
 * Simuluje pravidlo 14.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError par_def_list(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "par_def_list\n");
	#endif
	TError error = ENOTFOUND;
	// 14: <PAR_DEF_LIST> -> ( <PARAMS> )
	if(token.type == T_LeftParenthesis)
	{
		get_next_token(input, &attr);
		error = params(input);
		/**
		 * vlozime pocet parametru funkce
		 */
		tData *tempData;
		// funkce jiz v tabulce je
		if((tempData = htRead(funcTable, currFunc)) != NULL)
		{
			tData data;
			data.type = tempData->type;
			data.timesUsed = tempData->timesUsed;
			data.scope = tempData->scope;
			data.orderParams = currOrder;
			data.value.ptrTS = NULL;
			//htInsert(funcTable, strGetStr(&attr), data);
			//printf("462 %s\n",strGetStr(&attr));
			#ifdef DEBUG_SEM
			//fprintf(stderr, "UPRAVUJI %s, POCET PARAMS: %d\n", currFunc, currOrder);
			#endif
		}
		// SEMANTICKA ANALYZA
		// pokud ma main parametry, chyba
		if((error == ENOP) && (strcmp(currFunc, "main") == 0))
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: main nesmi mit parametry\n");
			#endif
			print_error(ESEM_DEF, token.line);
		}
		// /SEMANTICKA ANALYZA
		#ifdef DEBUG
		fprintf(stderr, "par_def_list: params vratilo: %d\n", error);
		#endif
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
		else if(token.type == T_RightParenthesis)
		{
			error = ENOP;
		}
		else
		{
			return ESYN;
		}
	}
	return error;
}

/**
 * Simuluje pravidla 12 a 13.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError dec_or_def(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "dec_or_def\n");
	#endif
	TError error = ENOTFOUND;
	// 12: <DEC_OR_DEF> -> <COMM_SEQ>
	error = comm_seq(input);
	#ifdef DEBUG
	fprintf(stderr, "dec_or_def: comm_seq vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		return error;
	}
	else if(error == ESYN)
	{
		return error;
	}
	// 13: <DEC_OR_DEF> -> ;
	else if(token.type == T_Semicolon)
	{
		return ENOP;
	}
	return error;
}

/**
 * Simuluje pravidlo 19.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError comm_seq(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "comm_seq\n");
	#endif
	TError error = ENOTFOUND;
	// 19: <COMM_SEQ> -> { <STMT_LIST> }
	if(token.type == T_LeftBrace)
	{
		/**
		 * kontrola, zda funkce nebyla podruhe definovana
		 */
		tData *tempData;
		if((tempData = htRead(funcTable, currFunc)) != NULL)
		{
			/**
			 * zjistime, jestli nejde o redefinici
			 */
			if((tempData->isDefined == 1) && (strcmp(currFunc, "main") != 0) && (currScope == 0))
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: funkce %s byla vicekrat definovana\n", currFunc);
				#endif
				print_error(ESEM_DEF, token.line);
			}
			/**
			 * zapiseme, ze funkce byla definovana
			 */
			if(tempData->type == FUNC)
			{
				tData data;
				data.type = tempData->type;
				data.timesUsed = tempData->timesUsed;
				data.scope = tempData->scope;
				data.orderParams = tempData->orderParams;
				data.retType = tempData->retType;
				data.isDefined = 1;
				data.value.ptrTS = NULL;
				htInsertData(funcTable, currFunc, data);
				//printf("576 %s\n",strGetStr(&attr));
				#ifdef DEBUG_SEM
				fprintf(stderr, "UPRAVUJI %s, DEFINOVANA: %d\n", currFunc, data.isDefined);
				#endif
			}
		}
		// SEMANTICKA ANALYZA
		if(tableStack.top->table != NULL)
		{
			commTable = tableStack.top->table;
		}
		else
		{
			commTable = funcTable;
		}
		++currScope; // menime scope, inkrementace
		gStackPush(&tableStack, commTable);
		#ifdef DEBUG
		fprintf(stderr, "PUSHUJU na %d\n", token.line);
		#endif
		commTable = tableStack.top->table;
		// /SEMANTICKA ANALYZA


		// SYNTAKTICKA ANALYZA
		get_next_token(input, &attr);
		error = stmt_list(input);
		#ifdef DEBUG
		fprintf(stderr, "comm_seq: stmt_list vratilo: %d\n", error);
		#endif
		// /SYNTAKTICKA ANALYZA
		

		// SEMANTICKA ANALYZA
		--currScope; // menime scope, dekrementace
		gStackPop(&tableStack);
		#ifdef DEBUG
		fprintf(stderr, "POPPUJU na %d\n", token.line);
		#endif
		commTable = tableStack.top->table;
		// /SEMANTICKA ANALYZA
		if(error == ENOP || error == EEMPTY)
		{
			if(token.type == T_RightBrace)
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
	}
	return error;
}

/**
 * Simuluje pravidla 20 a 21.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError stmt_list(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "stmt_list\n");
	#endif
	TError error = ENOTFOUND;
	// 20: <STMT_LIST> -> <STMT> <STMT_LIST>
	error = stmt(input);
	#ifdef DEBUG
	fprintf(stderr, "stmt_list: stmt vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		get_next_token(input, &attr);
		error = stmt_list(input);
		#ifdef DEBUG
		fprintf(stderr, "stmt_list: stmt_list vratilo: %d\n", error);
		#endif
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	else if(error == ESYN)
	{
		return error;
	}
	// 21: <STMT_LIST> -> E
	else
	{
		return EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidla 22, 23, 24, 25, 26, 27, 28 a 29
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError stmt(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "stmt\n");
	#endif
	TError error = ENOTFOUND;

	// SEMANTICKA ANALYZA

	// 22: <STMT> -> if ( <EXPR> ) <COMM_SEQ> <IF_N>
	if(token.type == T_If)
	{
		get_next_token(input, &attr);
		if(token.type == T_LeftParenthesis)
		{
			get_next_token(input, &attr);
			error = expr(input, &attr, 1, &counterVar, &commTable, &exprRes);

			//JARDA
			/*#ifdef JARIS
			lastI = listGetPointerLast(list);
			unie.obsah = afterIf;
			//je-li podminka pravdiva, skacu za if (afterIf)
			generateInstruction(I_IFGOTO, INT, &unie, STRING, &unie2,  DOUBLE, NULL);
			#endif*/

			#ifdef DEBUG
			//outputSymbolTable(commTable);
			fprintf(stderr, "stmt: expr vratilo: %d\n", error);
			fprintf(stderr, "### token po expr: %d\n", token.type);
			#endif
			if(error == ENOP)
			{
				get_next_token(input, &attr);
				error = comm_seq(input);
				
				//JARDA
				/*#ifdef JARIS
				SPushP(&stackI, lastI);
				afterIf = listGetPointerLast(list);
				SPushP(&stackI, afterIf);
				unie.obsah=afterIf;
				//generateInstruction(I_IFGOTO, INT, &unie, STRING, &unie2,  DOUBLE, NULL);
				#ifdef*/
				
				
				#ifdef DEBUG
				fprintf(stderr, "stmt: comm_seq vratilo: %d\n", error);
				#endif
				if(error == ENOP)
				{
					get_next_token(input, &attr);
					error = if_n(input);
					#ifdef DEBUG
					fprintf(stderr, "stmt: if_n: %d\n", error);
					fprintf(stderr, "po if_n mam token: %d\n", token.type);
					#endif
					if(error == ENOP)
					{
						return ENOP;
					}
					else if(error == ESYN)
					{
						return error;
					}
				}
				else if(error == ESYN)
				{
					return error;
				}
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		else
		{
			return ESYN;
		}
	}
	// 23: <STMT> -> for( <VAR_DEF> <EXPR> <ASSIGN> ) <COMM_SEQ>
	if(token.type == T_For)
	{
		get_next_token(input, &attr);
		if(token.type == T_LeftParenthesis)
		{
			get_next_token(input, &attr);
			error = var_def(input);
			#ifdef DEBUG
			fprintf(stderr, "stmt: var_def vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				get_next_token(input, &attr);
				error = expr(input, &attr, 0, &counterVar, &commTable, &exprRes);
				
				//JARDA
				/*#ifdef JARIS
				unie.obsah=5;
				unie2.obsah=10;
				generateInstruction(I_SET_FOR, INT, &unie, STRING, &unie2,  DOUBLE, NULL);
				#endif*/
				
				
				
				#ifdef DEBUG
				fprintf(stderr, "stmt: expr vratilo: %d\n", error);
				#endif
				if(error == ENOP)
				{
					get_next_token(input, &attr);
					error = assign(input);
					#ifdef DEBUG
					fprintf(stderr, "stmt: assign vratilo: %d\n", error);
					#endif
					if(error == ENOP)
					{
						get_next_token(input, &attr);
						//JARDA
						/*#ifdef JARIS
						beforeFor = listGetPointerLast(list);
						#endif*/
						/*#ifdef JARIS
						fprintf(stderr, "ADRESA ADRESA ADRESA: --------> %d\n", beforeFor);
						#endif*/
						error = comm_seq(input);
						//JARDA
						/*#ifdef JARIS
						generateInstruction(I_FOR_GOTO, INT, &unie, STRING, &unie2,  DOUBLE, NULL);
						SPushP(&forstack, beforeFor);
						behindFor = listGetPointerLast(list);
						SPushP(&forstack, behindFor);
						#endif*/
						#ifdef DEBUG
						fprintf(stderr, "stmt: comm_seq vratilo: %d\n", error);
						#endif
						if(error == ENOP)
						{
							return ENOP;
						}
						else if(error == ESYN)
						{
							return error;
						}
					}
					else if(error == ESYN)
					{
						return error;
					}
				}
				else if(error == ESYN)
				{
					return error;
				}
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		else
		{
			return ESYN;
		}
	}
	// 24: <STMT> -> <COMM_SEQ>
	if((error = comm_seq(input)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		fprintf(stderr, "stmt: comm_seq vratilo: %d\n", error);
		#endif
		return error;
	}
	// 25: <STMT> -> <VAR_DEF>
	if((error = var_def(input)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		fprintf(stderr, "stmt: var_def vratilo: %d\n", error);
		#endif
		return error;
	}
	// 26: <STMT> -> cin >> id <CIN_ID_N>;
	if(token.type == T_Cin)
	{
		get_next_token(input, &attr);
		if(token.type == T_RightShift)
		{
			get_next_token(input, &attr);
			if(token.type == T_Id)
			{
				// SEMANTICKA ANALYZA
				tData *tempData;
				if((tempData = htRead(commTable, strGetStr(&attr))) == NULL)
				{
					#ifdef DEBUG_SEM
					fprintf(stderr, "Chyba: promenna %s vypisovana na cout nebyla nalezena\n", strGetStr(&attr));
					#endif
					print_error(ESEM_DEF, token.line);
				}
				// /SEMANTICKA ANALYZA

				get_next_token(input, &attr);
				error = cin_id_n(input);
				#ifdef DEBUG
				fprintf(stderr, "stmt: cin_id_n vratilo: %d\n", error);
				#endif
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					if(token.type == T_Semicolon)
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
			}
			// konstanta
			else if((error = realtype()) == ENOP)
			{
				get_next_token(input, &attr);
				error = cin_id_n(input);
				#ifdef DEBUG
				fprintf(stderr, "stmt: cin_id_n vratilo: %d\n", error);
				#endif
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					if(token.type == T_Semicolon)
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
			}
			else
			{
				return ESYN;
			}
		}
		else
		{
			return ESYN;
		}
	}
	// 27: <STMT> -> cout << <COUT_TERM>;
	if(token.type == T_Cout)
	{
		get_next_token(input, &attr);
		if(token.type == T_LeftShift)
		{
			get_next_token(input, &attr);
			error = cout_term(input);
			#ifdef DEBUG
			fprintf(stderr, "stmt: cout_term vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				if(token.type == T_Semicolon)
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
		}
		else
		{
			return ESYN;
		}
	}
	// 28: <STMT> -> <RETURN>
	if((error = ret(input)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		fprintf(stderr, "stmt: ret vratilo: %d\n", error);
		#endif
		return error;
	}
	// 29) <STMT> -> id <CALL_ASSIGN>
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		tData *tempData;
		if(((tempData = htRead(commTable, strGetStr(&attr))) == NULL) && (check_builtin(strGetStr(&attr)) != 1))
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: promenna %s v leve casti prirazeni nebyla nalezena\n", strGetStr(&attr));
			#endif
			print_error(ESEM_DEF, token.line);
		}
		idAssign = htSearch(commTable,strGetStr(&attr));
		tempFunc = malloc(sizeof(char)*strlen(strGetStr(&attr)));
		strcpy(tempFunc, strGetStr(&attr));
		// /SEMANTICKA ANALYZA

		get_next_token(input, &attr);
		error = call_assign(input);
		#ifdef DEBUG
		fprintf(stderr, "stmt: call_assign vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			return ENOP;
		}
		else
		{
			return ESYN;
		}	
	}
	return error;
}

/**
 * Simuluje pravidla xx a xx.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError call_assign(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "params\n");
	#endif
	TError error = ENOTFOUND;
	// xx: <CALL_ASSIGN> -> = <EXPR>
	if(token.type == T_Assig)
	{
		get_next_token(input, &attr);
		error = expr(input, &attr, 0, &counterVar, &commTable, &exprRes);
		idAssign->data.value.ptrTS = exprRes;
		if(exprRes != NULL)
		{
			#ifdef DEBUG_INST
			if(exprRes->data.varType == T_Integ)
			{
				tHTItem *pom = idAssign->data.value.ptrTS;
				fprintf(stderr, "\tParserInt: CODE:%d|OPE1 %s %d ||Vysl %s\n",C_Assign,pom->key,pom->data.value.i,idAssign->key);
			}
			else if(exprRes->data.varType == T_Doub)
			{
				fprintf(stderr, "\tParserDoub: CODE:%d|OPE1 %s %f ||Vysl %s\n",C_Assign,exprRes->key,exprRes->data.value.d,idAssign->key);
			}
			else if(exprRes->data.varType == T_Str)
			{
				fprintf(stderr, "\tParserStr: CODE:%d|OPE1 %s %s ||Vysl %s\n",C_Assign,exprRes->key,exprRes->data.value.str,idAssign->key);
			}
			#endif
			generate_inst(C_Assign,exprRes,NULL,idAssign);
		}
		#ifdef DEBUG
		fprintf(stderr, "call_assign: expr vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			return ENOP;
		}
		else
		{
			return ESYN;
		}
	}
	// xx: <CALL_ASSIGN> -> (<terms>);
	else if(token.type == T_LeftParenthesis)
	{
		currFunc = malloc(sizeof(char)*strlen(tempFunc));
		strcpy(currFunc, tempFunc);

		// SEMANTICKA ANALYZA
		/**
		 * kontrola, zda volana funkce byla definovana
		 */
		tData *tempData;
		#ifdef DEBUG_SEM
		fprintf(stderr, "currFunc: %s\n", currFunc);
		#endif
		if((tempData = htRead(funcTable, currFunc)) != NULL)
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "je %s definovana: %d\n", currFunc, tempData->isDefined);
			#endif
			if(tempData->isDefined != 1)
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: volana funkce %s nebyla definovana\n", currFunc);
				#endif
				print_error(ESEM_DEF, token.line);
			}
		}
		// /SEMANTICKA ANALYZA
		get_next_token(input, &attr);
		error = terms(input);
		#ifdef DEBUG
		fprintf(stderr, "call_assign: terms vratilo: %d\n", error);
		#endif
		if(error == ENOP || error == EEMPTY)
		{
			get_next_token(input, &attr);
			if(token.type == T_Semicolon)
			{
				return ENOP;
			}
			else
			{
				return ESYN;
			}
		}
		else
		{
			return ESYN;
		}
	}

	return error;
}

/**
 * Simuluje pravidla 15 a 16.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError params(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "params\n");
	#endif
	TError error = ENOTFOUND;
	// 15: <PARAMS> -> <TYPE> id <PARAMS_N>
	error = type();
	#ifdef DEBUG
	fprintf(stderr, "params: type vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		// /SEMANTICKA ANALYZA
		get_next_token(input, &attr);
		if(token.type == T_Id)
		{
			tData *tempData;
			if((tempData = htRead(funcTable, currFunc)) != NULL)
			{
				// pokud funkce jeste neni deklarovana
				if(tempData->isDeclared != 1)
				{
					tData data;
					data.type = VAR;
					data.varType = currType; // ulozeni typu parametru
					data.timesUsed = 1;
					data.orderParams = ++currOrder;
					data.scope = 1; // nejnizsi scope nasledujiciho bloku
					data.value.ptrTS = NULL;
					data.retType = tempData->retType;
					htInsert(funcTable, strGetStr(&attr), data);
					htInsert(paraTable, currFunc, data); // vkladani do tabulky parametru

					#ifdef DEBUG_SEM
					fprintf(stderr, "VKLADAM %s, SCOPE: %d, TYPE: %d, CURRENT SCOPE: %d\n",
						strGetStr(&attr), data.scope, data.varType, currScope);
					fprintf(stderr, "VKLADAM PARAMETR %s s maskou %s, SCOPE: %d, TYPE: %d, ORDER: %d\n",
						strGetStr(&attr), currFunc, data.scope, data.varType, data.orderParams);
					#endif
				}

			}
			
			
			// KONEC SEMANTICKE ANALYZY
			get_next_token(input, &attr);
			error = params_n(input);
			//currOrder = 0;
			#ifdef DEBUG
			fprintf(stderr, "params: params_n vratilo: %d\n", error);
			#endif
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}
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
	// 16: <PARAMS> -> E
	else if(token.type == T_RightParenthesis)
	{
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidla 17 a 18.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError params_n(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "params_n\n");
	#endif
	TError error = ENOTFOUND;
	// 17: <PARAMS_N> -> , <TYPE> id <PARAMS_N>
	if(token.type == T_Comma)
	{
		get_next_token(input, &attr);
		error = type();
		#ifdef DEBUG
		fprintf(stderr, "params_n: type vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			// /SEMANTICKA ANALYZA
			get_next_token(input, &attr);
			if(token.type == T_Id)
			{
				tData *tempData;
				if((tempData = htRead(funcTable, currFunc)) != NULL)
				{
					if(tempData->isDeclared != 1)
					{
						tData data;
						data.type = VAR;
						data.varType = currType; // ulozeni typu parametru
						data.timesUsed = 1;
						data.orderParams = ++currOrder;
						data.scope = 1; // nejnizsi scope nasledujiciho bloku
						data.value.ptrTS = NULL;
						data.retType = tempData->retType;
						htInsert(funcTable, strGetStr(&attr), data);
						htInsert(paraTable, currFunc, data); // vkladani do tabulky parametru

						#ifdef DEBUG_SEM
						fprintf(stderr, "VKLADAM %s, SCOPE: %d, TYPE: %d, CURRENT SCOPE: %d\n",
							strGetStr(&attr), data.scope, data.varType, currScope);
						fprintf(stderr, "VKLADAM PARAMETR %s s maskou %s, SCOPE: %d, TYPE: %d, ORDER: %d\n",
							strGetStr(&attr), currFunc, data.scope, data.varType, data.orderParams);
						#endif
					}

				}
				
				// /SEMANTICKE ANALYZY
				get_next_token(input, &attr);
				error = params_n(input);
				#ifdef DEBUG
				fprintf(stderr, "params_n: params_n vratilo: %d\n", error);
				#endif
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					error = ENOP;
					return error;
				}
				else if(error == ESYN)
				{
					return error;
				}
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
	}
	// 18: <PARAMS_N> -> E
	else if(token.type == T_RightParenthesis)
	{
		return EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidlo 42.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError ret(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "ret\n");
	#endif
	TError error = ENOTFOUND;
	// 42: <RETURN> -> return <EXPR>;
	if(token.type == T_Return)
	{
		isReturn = 1;
		get_next_token(input, &attr);
		error = expr(input, &attr, 0, &counterVar, &commTable, &exprRes);
		isReturn = 0;
		#ifdef DEBUG
		fprintf(stderr, "ret: expr vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	return error;
}

/**
 * Simuluje pravidla 39 a nedefinovane pravidlo.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError cout_term(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "cout_term\n");
	#endif
	TError error = ENOTFOUND;
	// 39: <COUT_TERM> -> id <COUT_TERM_N>
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		tData *tempData;
		if((tempData = htRead(commTable, strGetStr(&attr))) == NULL)
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: promenna %s vypisovana na cout nebyla nalezena\n", strGetStr(&attr));
			#endif
			print_error(ESEM_DEF, token.line);
		}
		// KONEC SEMANTICKE ANALYZY
		get_next_token(input, &attr);
		error = cout_term_n(input);
		#ifdef DEBUG
		fprintf(stderr, "cout_term: cout_term_n vratilo: %d\n", error);
		#endif
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	// konstanta
	else if((error = realtype()) == ENOP)
	{
		get_next_token(input, &attr);
		error = cout_term_n(input);
		#ifdef DEBUG
		fprintf(stderr, "cout_term: cout_term_n vratilo: %d\n", error);
		#endif
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}

	return error;
}

/**
 * Simuluje pravidla 40 a 41.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError cout_term_n(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "cout_term_n\n");
	#endif
	TError error = ENOTFOUND;
	// 40: <COUT_TERM_N> -> << <COUT_TERM>
	if(token.type == T_LeftShift)
	{
		get_next_token(input, &attr);
		error = cout_term(input);
		#ifdef DEBUG
		fprintf(stderr, "cout_term_n: cout_term vratilo: %d\n", error);
		#endif
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	// 41: <COUT_TERM_N> -> E
	else
	{
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidlo 37.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError cin_id_n(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "cin_id_n\n");
	#endif
	TError error = ENOTFOUND;
	// 37) <CIN_ID_N> -> >> id <CIN_ID_N>
	if(token.type == T_RightShift)
	{
		get_next_token(input, &attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			tData *tempData;
			if((tempData = htRead(commTable, strGetStr(&attr))) == NULL)
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: promenna %s z cin nebyla nalezena\n", strGetStr(&attr));
				#endif
				print_error(ESEM_DEF, token.line);
			}
			// KONEC SEMANTICKE ANALYZY
			get_next_token(input, &attr);
			error = cin_id_n(input);
			#ifdef DEBUG
			fprintf(stderr, "cin_id_n: cin_id_n vratilo: %d\n", error);
			#endif
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		// konstanta
		else if((error = realtype()) == ENOP)
		{
			get_next_token(input, &attr);
			error = cin_id_n(input);
			#ifdef DEBUG
			fprintf(stderr, "cin_id_n: cin_id_n vratilo: %d\n", error);
			#endif
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		else
		{
			return ESYN;
		}		
	}
	// 38) <CIN_ID_N> -> E
	else
	{
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidlo 36.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError assign(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "assign\n");
	#endif
	TError error = ENOTFOUND;
	// 36: <ASSIGN> -> id = <EXPR> 
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		tData *tempData;
		if((tempData = htRead(commTable, strGetStr(&attr))) == NULL)
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: promenna %s z leve casti prirazeni nebyla nalezena\n", strGetStr(&attr));
			#endif
			print_error(ESEM_DEF, token.line);
		}
		// KONEC SEMANTICKE ANALYZY

		get_next_token(input, &attr);
		if(token.type == T_Assig)
		{
			get_next_token(input, &attr);
			error = expr(input, &attr, 1, &counterVar, &commTable, &exprRes);
			#ifdef DEBUG
			fprintf(stderr, "assign: expr vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		else
		{
			return ESYN;
		}
	}
			
	return error;
}

/**
 * Simuluje pravidlo 4 a 5.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError var_def(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "var_def\n");
	#endif
	TError error = ENOTFOUND;
	// 4: <VAR_DEF> -> <TYPE> id <INIT>;
	if((error = type()) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		fprintf(stderr, "var_def: type vratilo: %d:\n", error);
		#endif
		if(error == ESYN)
		{
			return error;
		}

		get_next_token(input, &attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			tData *tempData;
			tempData = htReadScope(commTable, strGetStr(&attr), currScope);
			// redefinice
			if(tempData && tempData->scope == currScope)
			{
				#ifdef DEBUG
				fprintf(stderr, "KONCIM – TOKEN: %s, SCOPE: %d, CURRENT: %d\n", strGetStr(&attr), tempData->scope, currScope);
				#endif
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: promenna %s byla redefinovana v ramci cislo %d\n", strGetStr(&attr), currScope);
				#endif
				print_error(ESEM_DEF, token.line);
			}
			// probehne zastineni
			else
			{
				tData data;
				data.type = VAR;
				data.scope = currScope;
				data.timesUsed = 0;
				data.varType = currType;
				data.value.ptrTS = exprRes;
				htInsert(commTable, strGetStr(&attr), data);
				idAssign = htSearch(commTable,strGetStr(&attr));
				#ifdef DEBUG_SEM
				fprintf(stderr, "VKLADAM %s, SCOPE: %d, TYPE: %d, CURRENT: %d\n", strGetStr(&attr), data.scope, data.varType, currScope);
				#endif
			}
			// KONEC SEMANTICKE ANALYZY

			get_next_token(input, &attr);
			error = init(input);
			#ifdef DEBUG
			fprintf(stderr, "var_def: init vratilo: %d\n", error);
			#endif
			if(error == ENOP || error == EEMPTY)
			{
				return ENOP;
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		else
		{
			return ESYN;
		}	
	}
	// 5: <VAR_DEF> -> auto id <INIT>;
	else if(token.type == T_Auto)
	{
		currType = T_Auto;
		get_next_token(input, &attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			tData *tempData;
			if((tempData = htRead(commTable, strGetStr(&attr))) != NULL)
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: redefinice promenne %s\n", strGetStr(&attr));
				#endif
				print_error(ESEM_DEF, token.line);
			}
			// promenna jeste neni v tabulce
			else
			{
				if(strGetStr(&attr) != NULL)
				{
					tData data;
					data.type = VAR;
					data.timesUsed = 0;
					data.scope = currScope;
					data.value.ptrTS = NULL;
					htInsert(commTable, strGetStr(&attr), data);
					idAssign = htSearch(commTable,strGetStr(&attr));
					#ifdef DEBUG_SEM
					fprintf(stderr, "VKLADAM %s, SCOPE: %d\n", strGetStr(&attr), data.scope);
					#endif
				}
			}
			// KONEC SEMANTICKE ANALYZY

			get_next_token(input, &attr);
			error = init(input);
			#ifdef DEBUG
			fprintf(stderr, "var_def: init vratilo: %d\n", error);
			#endif
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				return ENOP;
			}
			else if(error == ESYN)
			{
				return error;
			}
		}
		else
		{
			return ESYN;
		}	
	}
	return error;
}

/**
 * Simuluje pravidlo 6.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError init(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "init\n");
	#endif
	TError error = ENOTFOUND;
	// 6: <INIT> -> = <EXPR>
	if(token.type == T_Assig)
	{
		get_next_token(input, &attr);
		error = expr(input, &attr, 0, &counterVar, &commTable, &exprRes);
		idAssign->data.value.ptrTS = exprRes;
		if(exprRes != NULL)
		{
			#ifdef DEBUG_INST
			if(exprRes->data.varType == T_Integ)
			{
				tHTItem *pom = idAssign->data.value.ptrTS;
				fprintf(stderr, "\tParserInt: CODE:%d|OPE1 %s %d ||Vysl %s\n",C_Assign,pom->key,pom->data.value.i,idAssign->key);
			}
			else if(exprRes->data.varType == T_Doub)
			{
				fprintf(stderr, "\tParserDoub: CODE:%d|OPE1 %s %f ||Vysl %s\n",C_Assign,exprRes->key,exprRes->data.value.d,idAssign->key);
			}
			else if(exprRes->data.varType == T_Str)
			{
				fprintf(stderr, "\tParserStr: CODE:%d|OPE1 %s %s ||Vysl %s\n",C_Assign,exprRes->key,exprRes->data.value.str,idAssign->key);
			}
			#endif
			generate_inst(C_Assign,exprRes,NULL,idAssign);
		}
		#ifdef DEBUG
		//outputSymbolTable(commTable);
		fprintf(stderr, "init: expr vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	// 7: <INIT> ->	E
	else
	{
		/**
		 * pokud je typ auto a neni tam definice
		 */
		if(currType == T_Auto)
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: promenna typu auto musi byt rovnou definovana\n");
			#endif
			print_error(ESEM_DEF, token.line);
		}
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidla 43 a 44.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError if_n(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "if_n\n");
	#endif
	TError error = ENOTFOUND;
	// 43) <IF_N> -> else <COMM_SEQ>
	if(token.type == T_Else)
	{
		get_next_token(input, &attr);
		error = comm_seq(input);
		#ifdef DEBUG
		fprintf(stderr, "if_n: comm_seq vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	// 44) <IF_N> -> E
	else
	{
		error = EEMPTY;
	}
	return error;
}



/**
 * Simuluje pravidla 32 a 33.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError terms(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "terms\n");
	#endif
	TError error = ENOTFOUND;
	// 32) <TERMS> -> id <TERMS_N>
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		tData *tempData;
		// najdeme vlozenou promennou v tabulce symbolu
		if((tempData = htRead(commTable, strGetStr(&attr))) != NULL)
		{
			// kontrola typu parametru, ID
			tData *parData;
			if(((parData = htReadOrder(paraTable, currFunc, ++currOrderTerm)) != NULL))
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "parData->varType: %d\n", parData->varType);
				fprintf(stderr, "tempData->varType: %d\n", tempData->varType);
				#endif
				if(parData->varType != tempData->varType)
				{
					#ifdef DEBUG_SEM
					fprintf(stderr, "Chyba: promenna %s ma typ %d, pozadovany typ: %d\n", strGetStr(&attr), tempData->varType, parData->varType);
					#endif
					print_error(ESEM_TYP, token.line);
				}
			}
		}
		// nenasli jsme
		else
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: promenna %s vlozena jako parametr funkce %s nebyla nalezena\n", strGetStr(&attr), currFunc);
			#endif

			print_error(ESEM_DEF, token.line);
		}
		
		// KONEC SEMANTICKE ANALYZY
		get_next_token(input, &attr);
		error = terms_n(input);
		/**
		 * kontrola poctu parametru
		 */
		#ifdef DEBUG_SEM
		fprintf(stderr, "volam funkci: %s, vlozeno parametru: %d, deklarovany pocet: %d\n", currFunc, currOrderTerm, currOrder);
		#endif
		if((currOrderTerm != currOrder) && (check_builtin(currFunc) != 1))
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: pocet parametru zadany pro volani funkce %s neodpovida\n", currFunc);
			#endif
			print_error(ESEM_TYP, token.line);
		}
		else if(check_builtin(currFunc) == 1)
		{
			check_builtin_params(currFunc, currOrderTerm);
		}
		currOrderTerm = 0;

		#ifdef DEBUG
		fprintf(stderr, "terms: terms_n vratilo: %d\n", error);
		#endif
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	else if(((error = realtype(input)) == ENOP) || error == ESYN)
	{
		#ifdef DEBUG
		fprintf(stderr, "terms: realtype vratilo: %d\n", error);
		#endif

		tData *tempData;
		// kontrola typu parametru, KONSTANTA
		if(((tempData = htReadOrder(paraTable, currFunc, ++currOrderTerm)) != NULL))
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "tempData->varType: %d\n", tempData->varType);
			fprintf(stderr, "currType: %d\n", currType);
			#endif
			if(tempData->varType != currType)
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: pri volani funkce %s vlozena konstanta typu %d, pozadovany typ: %d\n", currFunc, currType, tempData->varType);
				#endif
				print_error(ESEM_TYP, token.line);
			}
		}
		get_next_token(input, &attr);
		error = terms_n(input);
		/**
		 * kontrola poctu parametru
		 */
		#ifdef DEBUG_SEM
		fprintf(stderr, "volam funkci: %s, vlozeno parametru: %d, deklarovany pocet: %d\n", currFunc, currOrderTerm, currOrder);
		#endif
		if((currOrderTerm != currOrder) && (check_builtin(currFunc) != 1))
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: pocet parametru zadany pro volani funkce %s neodpovida\n", currFunc);
			#endif
			print_error(ESEM_TYP, token.line);
		}
		else if(check_builtin(currFunc) == 1)
		{
			check_builtin_params(currFunc, currOrderTerm);
		}
		currOrderTerm = 0;

		#ifdef DEBUG
		fprintf(stderr, "terms: terms_n vratilo: %d\n", error);
		#endif
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(error == ESYN)
		{
			return error;
		}
	}
	// 33) <TERMS> -> E
	else
	{
		#ifdef DEBUG_SEM
		fprintf(stderr, "volam funkci: %s, vlozeno parametru: %d, deklarovany pocet: %d\n", currFunc, currOrderTerm, currOrder);
		#endif
		if((currOrderTerm != currOrder) && (check_builtin(currFunc) != 1))
		{
			#ifdef DEBUG_SEM
			fprintf(stderr, "Chyba: pocet parametru zadany pro volani funkce %s neodpovida\n", currFunc);
			#endif
			print_error(ESEM_TYP, token.line);
		}
		else if(check_builtin(currFunc) == 1)
		{
			check_builtin_params(currFunc, currOrderTerm);
		}
		currOrderTerm = 0;
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidla 34 a 35.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError terms_n(FILE *input)
{
	#ifdef DEBUG
	fprintf(stderr, "terms_n\n");
	#endif
	TError error = ENOTFOUND;
	// 34: <TERMS_N> -> , id <TERMS_N>
	if(token.type == T_Comma)
	{
		get_next_token(input, &attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			tData *tempData;
			// najdeme vlozenou promennou v tabulce symbolu
			if((tempData = htRead(commTable, strGetStr(&attr))) != NULL)
			{
				// kontrola typu parametru, ID
				tData *parData;
				if(((parData = htReadOrder(paraTable, currFunc, ++currOrderTerm)) != NULL))
				{
					#ifdef DEBUG_SEM
					fprintf(stderr, "parData->varType: %d\n", parData->varType);
					fprintf(stderr, "tempData->varType: %d\n", tempData->varType);
					#endif
					if(parData->varType != tempData->varType)
					{
						#ifdef DEBUG_SEM
						fprintf(stderr, "Chyba: promenna %s ma typ %d, pozadovany typ: %d\n", strGetStr(&attr), tempData->varType, parData->varType);
						#endif
						print_error(ESEM_TYP, token.line);
					}
				}
			}
			// nenasli jsme
			else
			{
				#ifdef DEBUG_SEM
				fprintf(stderr, "Chyba: promenna %s vlozena jako parametr funkce %s nebyla nalezena\n", strGetStr(&attr), currFunc);
				#endif
				print_error(ESEM_DEF, token.line);
			}
			// KONEC SEMANTICKE ANALYZY
			get_next_token(input, &attr);
			error = terms_n(input);
			#ifdef DEBUG
			fprintf(stderr, "terms: terms_n vratilo: %d\n", error);
			#endif
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}	
		}
		else if((error = realtype(input)) == ENOP || error == ESYN)
		{
			// SEMANTICKA ANALYZA
			tData *tempData;
			// kontrola typu parametru
			if((tempData = htReadOrder(paraTable, currFunc, ++currOrderTerm)) != NULL)
			{
				if(tempData->varType != currType)
				{
					#ifdef DEBUG_SEM
					fprintf(stderr, "Chyba: pri volani funkce %s vlozena konstanta typu %d, pozadovany typ: %d\n", currFunc, currType, tempData->varType);
					#endif
					print_error(ESEM_TYP, token.line);
				}
			}
			// KONEC SEMANTICKE ANALYZY
			get_next_token(input, &attr);
			error = terms_n(input);
			#ifdef DEBUG
			fprintf(stderr, "terms: terms_n vratilo: %d\n", error);
			#endif
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
			else if(error == ESYN)
			{
				return error;
			}	
		}
		else
		{
			return ESYN;
		}
	}
	// 35: <TERMS_N> -> E
	else
	{
		error = EEMPTY;
	}
	return error;
}


/**
 * Simuluje pravidla 8, 9, 10 a nedefinovane pravidlo.
 * @return Index do enumerace chyb.
 */
TError type()
{
	#ifdef DEBUG
	fprintf(stderr, "type\n");
	#endif
	TError error = ENOTFOUND;
	// 8: <TYPE> ->	int
	// 9: <TYPE> ->	double
	// 10: <TYPE> -> string 
	if(token.type == T_Double || token.type == T_String || token.type == T_Int)
	{
		if(token.type == T_Double)
		{
			currType = T_Doub;
		}
		else if(token.type == T_String)
		{
			currType = T_Str;
		}
		else if(token.type == T_Int)
		{
			currType = T_Integ;
		}
		return ENOP;
	}
	return error;
}

/**
 * Simuluje pravidla, ktera nemame v gramatice.
 * @return Index do enumerace chyb.
 */
TError realtype()
{
	#ifdef DEBUG
	fprintf(stderr, "realtype\n");
	#endif
	TError error = ENOTFOUND;
	// P: UNDEF
	if(token.type == T_Doub || token.type == T_Str || token.type == T_Integ)
	{
		#ifdef JARIS
		//unie.obsah_s = malloc(sizeof(char)*strlen(strGetStr(&attr)));
		strcpy(unie.obsah_s, strGetStr(&attr));
		generateInstruction(I_PRINT, STRING, &unie, STRING, &unie2,  DOUBLE, NULL);
		#endif
		currType = token.type;
		return ENOP;
	}
	return error;
}

/**
 * Incializuje tabulku symbolu
 * @return Index do enumerace chyb.
 */
TError init_table(tHTable **table)
{
	TError error = ENOP;
	tHTItem *item;
	item = (tHTItem *) malloc(sizeof(tHTItem));

	if(item != NULL)
	{
		item->key = "*UNDEF*";
		item->data.type = 0;
		item->data.retType = T_EOF;
		item->data.timesUsed = 0;
		item->data.orderParams = 0;
		item->data.varType = T_EOF;
		item->data.scope = 0;
		item->data.isDefined = 0;
		item->data.isDeclared = 0;
		item->ptrnext = NULL;
	}
	else
	{
		print_error(EINT, token.line);
	}

	*table = NULL;
	*table = (tHTable*) malloc(sizeof(tHTable));
	if(*table != NULL)
	{
		for (int i=0; i<MAX_HTSIZE; (**table)[i++] = item);
		htInit(*table);
	}
	else
	{
		print_error(EINT, token.line);
	}
	return error;
}

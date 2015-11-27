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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "parser.h"
#include "ial.h"
#include "expression.h"

int counterVar = 1;		// globalna premenna, ktora sluzi pri tvorbe pomocnych premennych na medzivypocty

tHTable* localTable;
tHTItem* item;
/**
 * @todo deklarace instruction listu
 */
int line = 1;

/**
 * Vrati dalsi token ze vstupu
 * @param input Soubor obsahujici vstupni kod.
 * @param attr  String lexemu.
 */
void getNextToken(FILE *input, string *attr)
{
	token = getToken(input, attr, &line);
	if(token.type == T_Error)
	{
		print_error(ELEX, token.line);
		exit(1);
	}
}

/**
 * Hlavni funkce parseru. Simuluje pravidlo 1.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError parse(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("parse\n");
	#endif
	TError error = ESYN;

	// inicializace TS
	if(initSTable() != ENOP)
	{
		return error;
	}

	getNextToken(input, attr);
	// prazdny soubor
	if(token.type == T_EOF)
	{
		return ESYN;
	}
	// 1: <PROGRAM> -> <FUNC_N>
	error = func_n(input, attr);
	#ifdef DEBUG
	printf("parse: func_n vratilo: %d\n", error);
	#endif

	outputSymbolTable(localTable);

	if(error == ENOP || error == EEMPTY)
	{
		error = ENOP;
	}
	else if(error == ENOTFOUND)
	{
		return ESYN;
	}
	return error;
}

/**
 * Simuluje pravidla 2 a 3.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError func_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("func_n\n");
	#endif
	TError error = ENOTFOUND;
	// 2: <FUNC_N> -> <FUNC> <FUNC_N>
	error = func(input, attr);
	#ifdef DEBUG
	printf("func_n: func vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		// <FUNC>
		getNextToken(input, attr);
		error = func_n(input, attr);
		#ifdef DEBUG
		printf("func_n: func_n vratilo: %d\n", error);
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
		printf("konec souboru\n");
		#endif
		error = EEMPTY;
	}
	return error;
}

/**
 * Simuluje pravidlo 11.
 * @return Index do enumerace chyb.
 */
TError func(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("func\n");
	#endif
	TError error = ENOTFOUND;
	// 11: <FUNC> -> <TYPE> id <PAR_DEF_LIST> <DEC_OR_DEF>
	error = type();
	#ifdef DEBUG
	printf("func: type vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			if(attr->str != NULL)
			{
				tData data;
				data.type = FUNC;
				data.timesUsed = 0;
				htInsert(localTable, attr->str, data);
			}
			// KONEC SEMANTICKE ANALYZY

			getNextToken(input, attr);
			error = par_def_list(input, attr);
			#ifdef DEBUG
			printf("func: par_def_list vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				getNextToken(input, attr);
				error = dec_or_def(input, attr);
				#ifdef DEBUG
				printf("func: dec_or_def vratilo: %d\n", error);
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
TError par_def_list(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("par_def_list\n");
	#endif
	TError error = ENOTFOUND;
	// 14: <PAR_DEF_LIST> -> ( <PARAMS> )
	if(token.type == T_LeftParenthesis)
	{
		getNextToken(input, attr);
		error = params(input, attr);
		#ifdef DEBUG
		printf("par_def_list: params vratilo: %d\n", error);
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
		// ???????????????????????????????????????????????????????????
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
TError dec_or_def(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("dec_or_def\n");
	#endif
	TError error = ENOTFOUND;
	// 12: <DEC_OR_DEF> -> <COMM_SEQ>
	error = comm_seq(input, attr);
	#ifdef DEBUG
	printf("dec_or_def: comm_seq vratilo: %d\n", error);
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
TError comm_seq(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("comm_seq\n");
	#endif
	TError error = ENOTFOUND;
	// 19: <COMM_SEQ> -> { <STMT_LIST> }
	if(token.type == T_LeftBrace)
	{
		getNextToken(input, attr);
		error = stmt_list(input, attr);
		#ifdef DEBUG
		printf("comm_seq: stmt_list vratilo: %d\n", error);
		#endif
		if(error == ENOP || error == EEMPTY)
		{
			// pokud pravidlo proslo na epsilonu
			// negetujeme dalsi token
			if(error == ENOP)
			{
				// ?????????????????????????????????? @todo mozny bug
				//getNextToken(input, attr);
			}
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
TError stmt_list(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("stmt_list\n");
	#endif
	TError error = ENOTFOUND;
	// 20: <STMT_LIST> -> <STMT> <STMT_LIST>
	error = stmt(input, attr);
	#ifdef DEBUG
	printf("stmt_list: stmt vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		getNextToken(input, attr);
		error = stmt_list(input, attr);
		#ifdef DEBUG
		printf("stmt_list: stmt_list vratilo: %d\n", error);
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
TError stmt(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("stmt\n");
	#endif
	TError error = ENOTFOUND;
	// 22: <STMT> -> if ( <EXPR> ) <COMM_SEQ> <IF_N>
	if(token.type == T_If)
	{
		getNextToken(input, attr);
		if(token.type == T_LeftParenthesis)
		{
			getNextToken(input, attr);
			error = expr(input, attr, 1, &counterVar);
			#ifdef DEBUG
			printf("stmt: expr vratilo: %d\n", error);
			printf("### token po expr: %d\n", token.type);
			#endif
			if(error == ENOP)
			{
				getNextToken(input, attr);
				error = comm_seq(input, attr);
				#ifdef DEBUG
				printf("stmt: comm_seq vratilo: %d\n", error);
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
		else
		{
			return ESYN;
		}
	}
	// 23: <STMT> -> for( <VAR_DEF> <EXPR> <ASSIGN> ) <COMM_SEQ>
	else if(token.type == T_For)
	{
		getNextToken(input, attr);
		if(token.type == T_LeftParenthesis)
		{
			getNextToken(input, attr);
			error = var_def(input, attr);
			#ifdef DEBUG
			printf("stmt: var_def vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				getNextToken(input, attr);
				error = expr(input, attr, 0, &counterVar);
				#ifdef DEBUG
				printf("stmt: expr vratilo: %d\n", error);
				#endif
				if(error == ENOP)
				{
					getNextToken(input, attr);
					error = assign(input, attr); ///// za attr 1, myslim, ze staci v expr
					#ifdef DEBUG
					printf("stmt: assign vratilo: %d\n", error);
					#endif
					if(error == ENOP)
					{
						getNextToken(input, attr);
						error = comm_seq(input, attr);
						#ifdef DEBUG
						printf("stmt: comm_seq vratilo: %d\n", error);
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
	else if((error = comm_seq(input, attr)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		printf("stmt: comm_seq vratilo: %d\n", error);
		#endif
		return error;
	}
	// 25: <STMT> -> <VAR_DEF>
	else if((error = var_def(input, attr)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		printf("stmt: var_def vratilo: %d\n", error);
		#endif
		return error;
	}
	// 26: <STMT> -> cin >> id <CIN_ID_N>;
	else if(token.type == T_Cin)
	{
		getNextToken(input, attr);
		if(token.type == T_RightShift)
		{
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				// SEMANTICKA ANALYZA
				/**
				 * todo kontrola, zda id existuje v tabulce symbolu
				 */
				// KONEC SEMANTICKE ANALYZY
				getNextToken(input, attr);
				error = cin_id_n(input, attr);
				#ifdef DEBUG
				printf("stmt: cin_id_n vratilo: %d\n", error);
				#endif
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					if(error == ENOP)
					{
						// ?????????????????????????????????? @todo mozny bug
						//getNextToken(input, attr);
					}

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
	else if(token.type == T_Cout)
	{
		getNextToken(input, attr);
		if(token.type == T_LeftShift)
		{
			getNextToken(input, attr);
			error = cout_term(input, attr);
			#ifdef DEBUG
			printf("stmt: cout_term vratilo: %d\n", error);
			#endif
			if(error == ENOP)
			{
				// ?????????????????????????????????? @todo mozny bug
				//getNextToken(input, attr);
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
	else if((error = ret(input, attr)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		printf("stmt: ret vratilo: %d\n", error);
		#endif
		return error;
	}
	// 29) <STMT> -> id = <FCALL_OR_ASSIGN>
	else if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		/**
		 * todo kontrola, zda id existuje v tabulce symbolu
		 */
		// KONEC SEMANTICKE ANALYZY
		getNextToken(input, attr);
		if(token.type == T_Equal)
		{
			getNextToken(input, attr);
			error = fcall_or_assign(input, attr);
			#ifdef DEBUG
			printf("stmt: fcall_or_assign vratilo: %d\n", error);
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
 * Simuluje pravidla 15 a 16.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError params(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("params\n");
	#endif
	TError error = ENOTFOUND;
	// 15: <PARAMS> -> <TYPE> id <PARAMS_N>
	error = type();
	#ifdef DEBUG
	printf("params: type vratilo: %d\n", error);
	#endif
	if(error == ENOP)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			/**
			 * todo vlozeni do tabulky symbolu funkce
			 */
			// KONEC SEMANTICKE ANALYZY
			getNextToken(input, attr);
			error = params_n(input, attr);
			#ifdef DEBUG
			printf("params: params_n vratilo: %d\n", error);
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
TError params_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("params_n\n");
	#endif
	TError error = ENOTFOUND;
	// 17: <PARAMS_N> -> , <TYPE> id <PARAMS_N>
	if(token.type == T_Comma)
	{
		getNextToken(input, attr);
		error = type();
		#ifdef DEBUG
		printf("params_n: type vratilo: %d\n", error);
		#endif
		if(error == ENOP)
		{
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				// SEMANTICKA ANALYZA
				/**
				 * todo vkladani do tabulky symbolu funkce
				 */
				// KONEC SEMANTICKE ANALYZY
				getNextToken(input, attr);
				error = params_n(input, attr);
				#ifdef DEBUG
				printf("params_n: params_n vratilo: %d\n", error);
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
TError ret(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("ret\n");
	#endif
	TError error = ENOTFOUND;
	// 42: <RETURN> -> return <EXPR>;
	if(token.type == T_Return)
	{
		getNextToken(input, attr);
		error = expr(input, attr, 0, &counterVar);
		#ifdef DEBUG
		printf("ret: expr vratilo: %d\n", error);
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
TError cout_term(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cout_term\n");
	#endif
	TError error = ENOTFOUND;
	// 39: <COUT_TERM> -> id <COUT_TERM_N>
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		/**
		 * todo kontrola, zda id existuje v tabulce symbolu
		 */
		// KONEC SEMANTICKE ANALYZY
		getNextToken(input, attr);
		error = cout_term_n(input, attr);
		#ifdef DEBUG
		printf("cout_term: cout_term_n vratilo: %d\n", error);
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
	/**
	 * @todo pravidlo na cout konstant (doub, str, integ)
	 */
	return error;
}

/**
 * Simuluje pravidla 40 a 41.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError cout_term_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cout_term_n\n");
	#endif
	TError error = ENOTFOUND;
	// 40: <COUT_TERM_N> -> << <COUT_TERM>
	if(token.type == T_LeftShift)
	{
		getNextToken(input, attr);
		error = cout_term(input, attr);
		#ifdef DEBUG
		printf("cout_term_n: cout_term vratilo: %d\n", error);
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
TError cin_id_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cin_id_n\n");
	#endif
	TError error = ENOTFOUND;
	// 37) <CIN_ID_N> -> >> id <CIN_ID_N>
	if(token.type == T_RightShift)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			/**
			 * todo kontrola, zda id existuje v tabulce symbolu
			 */
			// KONEC SEMANTICKE ANALYZY
			getNextToken(input, attr);
			error = cin_id_n(input, attr);
			#ifdef DEBUG
			printf("cin_id_n: cin_id_n vratilo: %d\n", error);
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
TError assign(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("assign\n");
	#endif
	TError error = ENOTFOUND;
	// 36: <ASSIGN> -> id = <EXPR> 
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		/**
		 * todo kontrola, zda id existuje v tabulce symbolu
		 */
		// KONEC SEMANTICKE ANALYZY
		getNextToken(input, attr);
		if(token.type == T_Equal)
		{
			getNextToken(input, attr);
			error = expr(input, attr, 1, &counterVar);
			#ifdef DEBUG
			printf("assign: expr vratilo: %d\n", error);
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
TError var_def(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("var_def\n");
	#endif
	TError error = ENOTFOUND;
	// 4: <VAR_DEF> -> <TYPE> id <INIT>;
	if((error = type()) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		printf("var_def: type vratilo: %d\n", error);
		#endif
		if(error == ESYN)
		{
			return error;
		}

		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			if(attr->str != NULL)
			{
				tData data;
				data.type = VAR;
				data.timesUsed = 0;
				htInsert(localTable, attr->str, data);
			}
			// KONEC SEMANTICKE ANALYZY
			getNextToken(input, attr);
			error = init(input, attr);
			#ifdef DEBUG
			printf("var_def: init vratilo: %d\n", error);
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
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			if(attr->str != NULL)
			{
				tData data;
				data.type = VAR;
				data.timesUsed = 0;
				htInsert(localTable, attr->str, data);
			}
			// KONEC SEMANTICKE ANALYZY	
			getNextToken(input, attr);
			error = init(input, attr);
			#ifdef DEBUG
			printf("var_def: init vratilo: %d\n", error);
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
TError init(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("init\n");
	#endif
	TError error = ENOTFOUND;
	// 6: <INIT> -> = <EXPR>
	if(token.type == T_Equal)
	{
		getNextToken(input, attr);
		error = expr(input, attr, 0, &counterVar);
		#ifdef DEBUG
		printf("init: expr vratilo: %d\n", error);
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
TError if_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("if_n\n");
	#endif
	TError error = ENOTFOUND;
	// 43) <IF_N> -> else <COMM_SEQ>
	if(token.type == T_Else)
	{
		getNextToken(input, attr);
		error = comm_seq(input, attr);
		#ifdef DEBUG
		printf("if_n: comm_seq vratilo: %d\n", error);
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
 * Simuluje pravidla 30 a 31.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError fcall_or_assign(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("fcall_or_assign\n");
	#endif
	TError error = ENOTFOUND;
	// 30: <FCALL_OR_ASSIGN> -> <EXPR> ;
	if((error = expr(input, attr, 0, &counterVar)) == ENOP || error == ESYN)
	{
		#ifdef DEBUG
		printf("fcall_or_assign: expr vratilo: %d\n", error);
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
	// 31: <FCALL_OR_ASSIGN> -> id ( <TERMS> );
	else if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		/**
		 * @todo kontrola, zda id existuje v tabulce symbolu
		 */
		// KONEC SEMANTICKE ANALYZY
		getNextToken(input, attr);
		if(token.type == T_LeftParenthesis)
		{
			getNextToken(input, attr);
			error = terms(input, attr);
			#ifdef DEBUG
			printf("fcall_or_assign: terms vratilo: %d\n", error);
			#endif
			if(error == ENOP || error == EEMPTY)
			{
				if(error == ENOP)
				{
					getNextToken(input, attr);
				}

				if(token.type == T_RightParenthesis)
				{
					getNextToken(input, attr);
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
 * Simuluje pravidla 32 a 33.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
TError terms(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("terms\n");
	#endif
	TError error = ENOTFOUND;
	// 32) <TERMS> -> id <TERMS_N>
	if(token.type == T_Id)
	{
		// SEMANTICKA ANALYZA
		if(attr->str != NULL)
		{
			tData data;
			data.type = VAR;
			data.timesUsed = 0;
			htInsert(localTable, attr->str, data);
		}
		// KONEC SEMANTICKE ANALYZY
		getNextToken(input, attr);
		error = terms_n(input, attr);
		#ifdef DEBUG
		printf("terms: terms_n vratilo: %d\n", error);
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
TError terms_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("terms_n\n");
	#endif
	TError error = ENOTFOUND;
	// 34: <TERMS_N> -> , id <TERMS_N>
	if(token.type == T_Comma)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// SEMANTICKA ANALYZA
			if(attr->str != NULL)
			{
				tData data;
				data.type = VAR;
				data.timesUsed = 0;
				htInsert(localTable, attr->str, data);
			}
			// KONEC SEMANTICKE ANALYZY
			getNextToken(input, attr);
			error = terms_n(input, attr);
			#ifdef DEBUG
			printf("terms: terms_n vratilo: %d\n", error);
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
	printf("type\n");
	#endif
	TError error = ENOTFOUND;
	// 8: <TYPE> ->	int
	// 9: <TYPE> ->	double
	// 10: <TYPE> -> string 
	if(token.type == T_Double || token.type == T_String || token.type == T_Int)
	{
		return ENOP;
	}
	return error;
}

/**
 * Incializuje tabulku symbolu
 * @return Index do enumerace chyb.
 */
TError initSTable()
{
	TError error = ENOP;
	item = NULL;
	item = (tHTItem*) malloc(sizeof(tHTable));
	if(item != NULL)
	{
		item->key = "*UNDEF*";
		item->data.type = 0;
		item->data.timesUsed = 0;
		item->ptrnext = NULL;
	}
	else
	{
		error = EINT;
		return error;
	}

	localTable = NULL;
	localTable = (tHTable*) malloc(sizeof(tHTable));
	if(localTable != NULL)
	{
		for (int i=0; i<MAX_HTSIZE; (*localTable)[i++] = item);
		htInit(localTable);
	}
	else
	{
		error = EINT;
		return error;
	}
	return error;
}

/**
 * [outputSymbolTable description]
 * @param ptrht [description]
 */
void outputSymbolTable(tHTable* ptrht)
{
	printf ("------------HASH TABLE--------------\n");
	for ( int i=0; i<HTSIZE; i++ ) {
		printf ("%i:",i);
		tHTItem* ptr = (*ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s,%d,%d)", ptr->key, ptr->data.type, ptr->data.timesUsed);
			ptr = ptr->ptrnext;
		}
		printf ("\n");
	}
	printf ("------------------------------------\n");
}

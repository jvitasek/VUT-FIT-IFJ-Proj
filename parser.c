/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.c  -  Syntakticky analyzator (Parser)
 * 
 * Autori:
 * 			xvitas02 – Jakub Vitasek
 * 			xvalec00 – Dusan Valecky
 */

#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "parser.h"
#include "ial.h"
#include "expression.h"

tHTable* globalTS;
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
	#ifdef DEBUG
	//printf("TOKEN: %d, LINE: %d\n", token.type, token.line);
	// if(strcmp(attr->str, "") != 0)
	// {
	// 	printf("---\n%s\n", strGetStr(attr));
	// 	printf("%d\n---\n", token.type);
	// }
	#endif
	if(token.type == T_Error) exit(1);
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
	// 1: <PROGRAM> -> <FUNC_N>
	error = func_n(input, attr);
	if(error == EEMPTY || error == ENOP)
	{
		error = ENOP;
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
	TError error = ESYN;
	// 2: <FUNC_N> -> <FUNC> <FUNC_N>
	error = func(input, attr);
	if(error == ENOP)
	{
		// <FUNC>
		getNextToken(input, attr);
		error = func_n(input, attr);
		if(error == ENOP)
		{
			// <FUNC> <FUNC_N>
			return error;
		}
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
	TError error = ESYN;
	// 11: <FUNC> -> <TYPE> id <PAR_DEF_LIST> <DEC_OR_DEF>
	error = type();
	if(error == ENOP)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			getNextToken(input, attr);
			error = par_def_list(input, attr);
			if(error == ENOP)
			{
				getNextToken(input, attr);
				error = dec_or_def(input, attr);
				return error;
			}
			else
			{
				return error;
			}
		}
		else
		{
			error = ESYN;
		}
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
	TError error = ESYN;
	// 14: <PAR_DEF_LIST> -> ( <PARAMS> )
	if(token.type == T_LeftParenthesis)
	{
		getNextToken(input, attr);
		error = params(input, attr);
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
		else if(token.type == T_RightParenthesis)
		{
			error = ENOP;
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
	TError error = ESYN;
	// 12: <DEC_OR_DEF> -> <COMM_SEQ>
	error = comm_seq(input, attr);
	if(error == ENOP)
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
	TError error = ESYN;
	// 19: <COMM_SEQ> -> { <STMT_LIST> }
	if(token.type == T_LeftBrace)
	{
		getNextToken(input, attr);
		error = stmt_list(input, attr);
		if(error == ENOP || error == EEMPTY)
		{
			// pokud pravidlo proslo na epsilonu
			// negetujeme dalsi token
			if(error == ENOP)
			{
				getNextToken(input, attr);
			}

			if(token.type == T_RightBrace)
			{
				return ENOP;
			}
			else
			{
				error = ESYN;
				//fprintf(stderr, "COMM_SEQ: ocekavano }\n");
			}
		}
		else
		{
			error = ESYN;
			//fprintf(stderr, "COMM_SEQ: stmt_list nevratil ENOP\n");
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
	TError error = ESYN;
	// 20: <STMT_LIST> -> <STMT> <STMT_LIST>
	error = stmt(input, attr);
	if(error == ENOP)
	{
		getNextToken(input, attr);
		error = stmt_list(input, attr);
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
			return error;
		}
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
	TError error = ESYN;
	// 22: <STMT> -> if <EXPR> <COMM_SEQ> <IF_N>
	if(token.type == T_If)
	{
		getNextToken(input, attr);
		error = expr(input, attr);
		if(error == ENOP)
		{
			getNextToken(input, attr);
			error = comm_seq(input, attr);
			if(error == ENOP)
			{
				getNextToken(input, attr);
				error = if_n(input, attr);
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					error = ENOP;
					return error;
				}
			}
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
			if(error == ENOP)
			{
				getNextToken(input, attr);
				error = expr(input, attr);
				if(error == ENOP)
				{
					getNextToken(input, attr);
					error = assign(input, attr);
					if(error == ENOP)
					{
						getNextToken(input, attr);
						if(token.type == T_RightParenthesis)
						{
							getNextToken(input, attr);
							error = comm_seq(input, attr);
							if(error == ENOP)
							{
								return ENOP;
							}
						}
						else
						{
							error = ESYN;
						}
					}
				}
			}
		}
		else
		{
			error = ESYN;
		}
	}
	// 24: <STMT> -> <COMM_SEQ>
	else if((error = comm_seq(input, attr)) == ENOP)
	{
		return error;
	}
	// 25: <STMT> -> <VAR_DEF>
	else if((error = var_def(input, attr)) == ENOP)
	{
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
				getNextToken(input, attr);
				error = cin_id_n(input, attr);
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					if(error == ENOP)
					{
						getNextToken(input, attr);
					}

					if(token.type == T_Semicolon)
					{
						return ENOP;
					}
					else
					{
						error = ESYN;
					}
				}
			}
			else
			{
				error = ESYN;
			}
		}
		else
		{
			error = ESYN;
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
			if(error == ENOP)
			{
				getNextToken(input, attr);
				if(token.type == T_Semicolon)
				{
					return ENOP;
				}
				else
				{
					error = ESYN;
				}
			}
		}
		else
		{
			error = ESYN;
		}
	}
	// 28: <STMT> -> <RETURN>
	else if((error = ret(input, attr)) == ENOP)
	{
		return error;
	}
	// 29) <STMT> -> id = <FCALL_OR_ASSIGN>
	else if(token.type == T_Id)
	{
		getNextToken(input, attr);
		if(token.type == T_Equal)
		{
			getNextToken(input, attr);
			error = fcall_or_assign(input, attr);
			if(error == ENOP)
			{
				return error;
			}
		}
		else
		{
			error = ESYN;
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
	TError error = ESYN;
	// 15: <PARAMS> -> <TYPE> id <PARAMS_N>
	error = type();
	if(error == ENOP)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			getNextToken(input, attr);
			error = params_n(input, attr);
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
		}
		else
		{
			error = ESYN;
		}
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
	TError error = ESYN;
	// 17: <PARAMS_N> -> , <TYPE> id <PARAMS_N>
	if(token.type == T_Comma)
	{
		getNextToken(input, attr);
		error = type();
		if(error == ENOP)
		{
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				getNextToken(input, attr);
				error = params_n(input, attr);
				// neni potreba kontrolovat, zda pravidlo
				// proslo na epsilon, negetujeme dalsi token
				if(error == ENOP || error == EEMPTY)
				{
					error = ENOP;
					return error;
				}
			}
			else
			{
				error = ESYN;
			}
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
	TError error = ESYN;
	// 42: <RETURN> -> return <EXPR>;
	if(token.type == T_Return)
	{
		getNextToken(input, attr);
		error = expr(input, attr);
		if(error == ENOP)
		{
			getNextToken(input, attr);
			if(token.type == T_Semicolon)
			{
				return ENOP;
			}
			else
			{
				error = ESYN;
			}
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
	TError error = ESYN;
	// 39)	<COUT_TERM>		-> 	id <COUT_TERM_N>
	if(token.type == T_Id)
	{
		getNextToken(input, attr);
		error = cout_term_n(input, attr);
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
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
	TError error = ESYN;
	// 40: <COUT_TERM_N> -> << <COUT_TERM>
	if(token.type == T_LeftShift)
	{
		getNextToken(input, attr);
		error = cout_term(input, attr);
		// neni potreba kontrolovat, zda pravidlo
		// proslo na epsilon, negetujeme dalsi token
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
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
	TError error = ESYN;
	// 37) <CIN_ID_N> -> >> id <CIN_ID_N>
	if(token.type == T_LeftShift)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			getNextToken(input, attr);
			error = cin_id_n(input, attr);
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
		}
		else
		{
			error = ESYN;
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
	TError error = ESYN;
	// 36: <ASSIGN> -> id = <EXPR> 
	if(token.type == T_Id)
	{
		getNextToken(input, attr);
		if(token.type == T_Equal)
		{
			getNextToken(input, attr);
			error = expr(input, attr);
			if(error == ENOP)
			{
				return error;
			}
		}
		else
		{
			error = ESYN;
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
	TError error = ESYN;
	// 4: <VAR_DEF> -> <TYPE> id <INIT>;
	if((error = type()) == ENOP)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			getNextToken(input, attr);
			error = init(input, attr);
			if(error == ENOP || error == EEMPTY)
			{
				printf("token pred: %d\n", token.type);
				// pokud pravidlo proslo na epsilonu
				// negetujeme dalsi token
				if(error == ENOP)
				{
					getNextToken(input, attr);
				}
				printf("token po: %d\n", token.type);
				if(token.type == T_Semicolon)
				{
					return ENOP;
				}
				else
				{
					return ESYN;
				}
			}
		}
		else
		{
			error = ESYN;
		}	
	}
	// 5: <VAR_DEF> -> auto id <INIT>;
	else if(token.type == T_Auto)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			getNextToken(input, attr);
			error = init(input, attr);
			// neni potreba kontrolovat, zda pravidlo
			// proslo na epsilon, negetujeme dalsi token
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
		}
		else
		{
			error = ESYN;
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
	TError error = ESYN;
	// 6: <INIT> -> = <EXPR>
	if(token.type == T_Equal)
	{
		getNextToken(input, attr);
		error = expr(input, attr);
		if(error == ENOP)
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
	TError error = ESYN;
	// 43) <IF_N> -> else <COMM_SEQ>
	if(token.type == T_Else)
	{
		getNextToken(input, attr);
		error = comm_seq(input, attr);
		if(error == ENOP)
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
	TError error = ESYN;
	// 30: <FCALL_OR_ASSIGN> -> <EXPR> ;
	if((error = expr(input, attr)) == ENOP)
	{
		getNextToken(input, attr);
		if(token.type == T_Semicolon)
		{
			return ENOP;
		}
		else
		{
			error = ESYN;
		}
	}
	// 31: <FCALL_OR_ASSIGN> -> id ( <TERMS> );
	else if(token.type == T_Id)
	{
		getNextToken(input, attr);
		if(token.type == T_LeftParenthesis)
		{
			getNextToken(input, attr);
			error = terms(input, attr);

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
						error = ESYN;
					}
				}
				else
				{
					error = ESYN;
				}
			}
		}
		else
		{
			error = ESYN;
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
	TError error = ESYN;
	// 32) <TERMS> -> id <TERMS_N>
	if(token.type == T_Id)
	{
		getNextToken(input, attr);
		error = terms_n(input, attr);
		if(error == ENOP || error == EEMPTY)
		{
			error = ENOP;
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
	TError error = ESYN;
	// 34: <TERMS_N> -> , id <TERMS_N>
	if(token.type == T_Comma)
	{
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			getNextToken(input, attr);
			error = terms_n(input, attr);
			if(error == ENOP || error == EEMPTY)
			{
				error = ENOP;
				return error;
			}
		}
		else
		{
			error = ESYN;
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
	TError error = ESYN;
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
		item->data = -1;
		item->ptrnext = NULL;
	}
	else
	{
		error = EINT;
		return error;
	}

	globalTS = NULL;
	globalTS = (tHTable*) malloc(sizeof(tHTable));
	if(globalTS != NULL)
	{
		for (int i=0; i<MAX_HTSIZE; (*globalTS)[i++] = item);
		htInit(globalTS);
	}
	else
	{
		error = EINT;
		return error;
	}
	return error;
}

/**
 * [vypisCelouTabulku description]
 * @param ptrht [description]
 */
void vypisCelouTabulku( tHTable* ptrht ) {
	
	printf ("------------HASH TABLE--------------\n");
	for ( int i=0; i<HTSIZE; i++ ) {
		printf ("%i:",i);
		tHTItem* ptr = (*ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s,%.2f)",ptr->key,ptr->data);
			ptr = ptr->ptrnext;
		}
		printf ("\n");
	}
	
	printf ("------------------------------------\n");
}

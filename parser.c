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
TError error;

/**
 * Vrati dalsi token ze vstupu
 * @param input Soubor obsahujici vstupni kod.
 * @param attr  String lexemu.
 */
void getNextToken(FILE *input, string *attr)
{
	token = getToken(input, attr, &line);
	#ifdef DEBUG
	if(strcmp(attr->str, "") != 0)
	{
		printf("---\n%s\n", strGetStr(attr));
		printf("%d\n---\n", token.type);
	}
	#endif
	if(token.type == T_Error) exit(1);
}

/**
 * Hlavni funkce parseru. Simuluje pravidlo 1.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int parse(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("parse\n");
	#endif
	error = ESYN;

	// inicializace TS
	if(initSTable() != ENOP)
	{
		return error;
	}

	getNextToken(input, attr);
	// 1: <PROGRAM> -> <FUNC_N>
	func_n(input, attr);
	return error;
}

/**
 * Simuluje pravidla 2 a 3.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int func_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("func_n\n");
	#endif
	error = ESYN;
	
	// 2: <FUNC_N> -> <FUNC> <FUNC_N>
	if(func(input, attr) == ENOP)
	{
		// <func>
		getNextToken(input, attr);
		if(func_n(input, attr) == ENOP)
		{
			// <func> <func_n>
			error = ENOP;
			return error;
		}
	}
	// 3: <FUNC_N> -> E 
	else
	{
		error = ENOP;
		return error;
	}

	return error;
}

/**
 * Simuluje pravidlo 11.
 * @return Index do enumerace chyb.
 */
int func(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("func\n");
	#endif
	error = ESYN;
	// 11: <FUNC> -> <TYPE> id <PAR_DEF_LIST> <DEC_OR_DEF>
	if(type() == ENOP)
	{
		// <type>
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// <type> ID
			getNextToken(input, attr);
			if(par_def_list(input, attr) == ENOP)
			{
				// <type> ID <par_def_list>
				getNextToken(input, attr);
				if(dec_or_def(input, attr) == ENOP)
				{
					// <type> ID <par_def_list> <dec_or_def>
					error = ENOP;
					return error;
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			
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
int par_def_list(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("par_def_list\n");
	#endif
	error = ESYN;
	// 14: <PAR_DEF_LIST> -> ( <PARAMS> )
	if(token.type == T_LeftParenthesis)
	{
		// (
		getNextToken(input, attr);
		if(token.type == T_RightParenthesis)
		{
			// (E)
			error = ENOP;
			return error;
		}
		else if(params(input, attr) == ENOP)
		{
			// (<params>
			//getNextToken(input, attr);
			printf("%d\n", token.type);
			if(token.type == T_RightParenthesis)
			{
				// (<params>)
				error = ENOP;
				return error;
			}
			else
			{
				fprintf(stderr, "Ocekavana prava zavorka.\n");
				
			}
		}
	}
	else
	{
		fprintf(stderr, "Ocekavana leva zavorka.\n");
		
	}
	return error;
}

/**
 * Simuluje pravidla 12 a 13.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int dec_or_def(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("dec_or_def\n");
	#endif
	error = ESYN;
	// 12: <DEC_OR_DEF> -> <COMM_SEQ>
	if(comm_seq(input, attr) == ENOP)
	{
		error = ENOP;
		return error;
	}
	// 13: <DEC_OR_DEF> -> ;
	else if(token.type == T_Semicolon)
	{
		error = ENOP;
		return error;
	}
	else
	{
		fprintf(stderr, "Ocekavana sekvence prikazu nebo strednik.\n");
		
	}
	return error;
}

/**
 * Simuluje pravidlo 19.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int comm_seq(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("comm_seq\n");
	#endif
	error = ESYN;
	// 19: <COMM_SEQ> -> { <STMT_LIST> }
	if(token.type == T_LeftBrace)
	{
		// {
		getNextToken(input, attr);
		if(stmt_list(input, attr) == ENOP)
		{
			// { STMT_LIST
			getNextToken(input, attr);
			if(token.type == T_RightBrace)
			{
				// { STMT_LIST }
				error = ENOP;
				return error;
			}
			else
			{
				fprintf(stderr, "Ocekavana prava slozena zavorka.\n");
				
			}
		}
	}
	else
	{
		//fprintf(stderr, "Ocekavana leva slozena zavorka.\n");
		
	}
	return error;
}

/**
 * Simuluje pravidla 20 a 21.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int stmt_list(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("stmt_list\n");
	#endif
	error = ESYN;
	// 20: <STMT_LIST> -> <STMT> <STMT_LIST>
	if(stmt(input, attr) == ENOP)
	{
		// <stmt>
		getNextToken(input, attr);
		if(stmt_list(input, attr) == ENOP)
		{
			// <stmt> <stmt_list>
			error = ENOP;
			return error;
		}
	}
	else
	{
		// }
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidla 22, 23, 24, 25, 26, 27, 28 a 29
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int stmt(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("stmt\n");
	#endif
	error = ESYN;
	// 29: <STMT> -> id = <FCALL_OR_ASSIGN>
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(token.type == T_Assig)
		{
			// ID =
			getNextToken(input, attr);
			if(fcall_or_assing(input, attr) == ENOP)
			{
				// ID = <fcall_or_assign>
				error = ENOP;
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavan znak prirazeni.\n");
			
		}
	}
	// 22: <STMT> -> if <EXPR> <COMM_SEQ> <IF_N>
	else if(token.type == T_If)
	{
		// if 
		getNextToken(input, attr);
		if(expr(input, attr) == ENOP)
		{
			// if <expr>	
			getNextToken(input, attr);
			if(comm_seq(input, attr) == ENOP)
			{
				// if <expr> <comm_seq> 
				getNextToken(input, attr);
				if(if_n(input, attr) == ENOP)
				{
					// if <expr> <comm_seq> else
					error = ENOP;
					return error;
				}
			}
		}
	}
	// 23: <STMT> -> for( <VAR_DEF> <EXPR> <ASSIGN> ) <COMM_SEQ>
	else if(token.type == T_For)
	{
		// for
		getNextToken(input, attr);
		if(token.type == T_LeftParenthesis)
		{
			// for (
			getNextToken(input, attr);
			if(var_def(input, attr) == ENOP)
			{
				// for ( <var_def>
				getNextToken(input, attr);
				if(expr(input, attr) == ENOP)
				{
					// for ( <var_def> <expr>
					getNextToken(input, attr);
					if(assign(input, attr) == ENOP)
					{
						// for ( <var_def> <expr> <assign>
						getNextToken(input, attr);
						if(token.type == T_RightParenthesis)
						{
							// for ( <var_def> <expr> <assign> )
							getNextToken(input, attr);
							if(comm_seq(input, attr) == ENOP)
							{
								// for ( <var_def> <expr> <assign> ) <comm_seq>
								error = ENOP;
								return error;
							}
						}
						else
						{
							fprintf(stderr, "Ocekavana prava zavorka.\n");
							
						}
					}
				}
			}
		}	
		else
		{
			fprintf(stderr, "Ocekavana leva zavorka.\n");
			
		}
	}
	// 24: <STMT> -> <COMM_SEQ>
	else if(comm_seq(input, attr) == ENOP)
	{
		// <comm_seq>
		error = ENOP;
		return error;
	}
	// 25: <STMT> -> <VAR_DEF>
	else if(var_def(input, attr) == ENOP)
	{
		// <var_def>
		error = ENOP;
		return error;
	}
	// 26: <STMT> -> cin >> id <CIN_ID_N>;
	else if(token.type == T_Cin)
	{
		// cin
		getNextToken(input, attr);
		if(token.type == T_RightShift)
		{
			// cin >>
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				// cin >> ID
				getNextToken(input, attr);
				if(cin_id_n(input, attr) == ENOP)
				{
					// cin >> ID <cin_id_n>
					getNextToken(input, attr);
					if(token.type == T_Semicolon)
					{
						// cin >> ID <cin_id_n>;
						error = ENOP;
						return error;
					}
					else
					{
						fprintf(stderr, "Ocekavan strednik.\n");
						
					}
				}
			}
			else
			{
				fprintf(stderr, "Ocekavano ID.\n");
				
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano >>.\n");
			
		}
	}
	// 27: <STMT> -> cout << <COUT_TERM>;
	else if(token.type == T_Cout)
	{
		// cout
		getNextToken(input, attr);
		if(token.type == T_LeftShift)
		{
			// cout <<
			getNextToken(input, attr);
			if(cout_term(input, attr) == ENOP)
			{
				// cout << <cout_term>
				getNextToken(input, attr);
				if(token.type == T_Semicolon)
				{
					// cout << <cout_term>;
					error = ENOP;
					return error;
				}
				else
				{
					fprintf(stderr, "Ocekavan strednik.\n");
					
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano <<.\n");
			
		}
	}
	// 28: <STMT> -> <RETURN>
	else if(ret(input, attr) == ENOP)
	{
		// <return>
		error = ENOP;
		return error;
	}

	return error;
}

/**
 * Simuluje pravidlo 42.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int ret(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("ret\n");
	#endif
	error = ESYN;
	// 42: <RETURN> -> return <EXPR>;
	if(token.type == T_Return)
	{
		// return
		getNextToken(input, attr);
		if(expr(input, attr) == ENOP)
		{
			// return <ęxpr>
			getNextToken(input, attr);
			if(token.type == T_Semicolon)
			{
				// return <expr>;
				error = ENOP;
				return error;
			}
			else
			{
				fprintf(stderr, "Ocekavan strednik.\n");
				
			}
		}
	}
	else
	{
		fprintf(stderr, "Ocekavano klicove slovo return.\n");
		
	}
	return error;
}

/**
 * Simuluje pravidla 39 a nedefinovane pravidlo.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int cout_term(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cout_term\n");
	#endif
	error = ESYN;
	// 39: <COUT_TERM> -> id <COUT_TERM_N>
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(cout_term_n(input, attr) == ENOP)
		{
			// ID <cout_term_n>
			error = ENOP;
			return error;
		}
	}
	// UNDEF!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	else if(realtype() == ENOP)
	{
		// <realtype>
		getNextToken(input, attr);
		if(cout_term_n(input, attr) == ENOP)
		{
			// <realtype> <cout_term_n>
			error = ENOP;
			return error;
		}
	}
	else
	{
		fprintf(stderr, "Ocekavano ID.\n");
		
	}
	return error;
}

/**
 * Simuluje pravidla 40 a 41.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int cout_term_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cout_term_n\n");
	#endif
	error = ESYN;
	// 40: <COUT_TERM_N> -> << <COUT_TERM>
	if(token.type == T_LeftShift)
	{
		// << 
		getNextToken(input, attr);
		if(cout_term(input, attr) == ENOP)
		{
			// << <cout_term>
			error = ENOP;
			return error;
		}
	}
	// 41: <COUT_TERM_N> -> E 
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidlo 37.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int cin_id_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cin_id_n\n");
	#endif
	error = ESYN;
	// 37: <CIN_ID_N> -> >> id <CIN_ID_N>
	if(token.type == T_RightShift)
	{
		// >>
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// >> ID
			getNextToken(input, attr);
			if(cin_id_n(input, attr) == ENOP)
			{
				// >> ID <cin_id_n>
				error = ENOP;
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			
		}
	}
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidlo 36.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int assign(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("assign\n");
	#endif
	error = ESYN;
	// 36: <ASSIGN> -> id = <EXPR> 
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(token.type == T_Equal)
		{
			// ID = 
			getNextToken(input, attr);
			if(expr(input, attr) == ENOP)
			{
				// ID = <expr>
				error = ENOP;
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavan znak prirazeni.\n");
			
		}
	}
	else
	{
		fprintf(stderr, "Ocekavano ID.\n");
		
	}
	return error;
}

/**
 * Simuluje pravidlo 4 a 5.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int var_def(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("var_def\n");
	#endif
	error = ESYN;
	// 5: <VAR_DEF> -> auto id <INIT>;
	if(token.type == T_Auto)
	{
		// auto
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// auto ID
			getNextToken(input, attr);
			if(init(input, attr) == ENOP)
			{
				// auto ID <init>
				getNextToken(input, attr);
				if(token.type == T_Semicolon)
				{
					// auto ID <init>;
					error = ENOP;
					return error;
				}
				else
				{
					fprintf(stderr, "Ocekavan strednik.\n");
					
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			
		}
	}
	// 4: <VAR_DEF> -> <TYPE> id <INIT>;
	if(type() == ENOP)
	{
		// <type>
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// <type> ID
			getNextToken(input, attr);
			if(init(input, attr) == ENOP)
			{
				// <type> ID <init>
				getNextToken(input, attr);
				if(token.type == T_Semicolon)
				{
					// <type> ID <init>;
					error = ENOP;
					return error;
				}
				else
				{
					fprintf(stderr, "Ocekavan strednik.\n");
					
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			
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
int init(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("init\n");
	#endif
	error = ESYN;
	// 6: <INIT> -> = <EXPR>
	if(token.type == T_Equal)
	{
		// = 
		getNextToken(input, attr);
		if(expr(input, attr) == ENOP)
		{
			// = <expr>
			error = ENOP;
			return error;
		}
	}
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidla 43 a 44.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int if_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("if_n\n");
	#endif
	error = ESYN;
	// 43: <IF_N> -> else <COMM_SEQ>
	if(token.type == T_Else)
	{
		// else 
		getNextToken(input, attr);
		if(comm_seq(input, attr) == ENOP)
		{
			// else <comm_seq>
			error = ENOP;
			return error;
		}
	}
	// 44: <IF_N> -> E
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidla 30 a 31.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int fcall_or_assing(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("fcall_or_assing\n");
	#endif
	error = ESYN;
	// 31: <FCALL_OR_ASSIGN> -> id ( <TERMS> );
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(token.type == T_LeftParenthesis)
		{
			// ID (
			getNextToken(input, attr);
			if(terms(input, attr) == ENOP)
			{
				// ID ( <terms>
				getNextToken(input, attr);
				if(token.type == T_RightParenthesis)
				{
					// ID ( <terms> )
					getNextToken(input, attr);
					if(token.type == T_Semicolon)
					{
						// ID ( <terms> ) ;
						error = ENOP;
						return error;
					}
					else
					{
						fprintf(stderr, "Ocekavan strednik.\n");
						
					}
				}
				else
				{
					fprintf(stderr, "Ocekavana prava zavorka.\n");
					
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavana leva zavorka.\n");
			
		}
	}
	// 30: <FCALL_OR_ASSIGN> -> <EXPR> ;
	else if(expr(input, attr) == ENOP)
	{
		// <expr>
		getNextToken(input, attr);
		if(token.type == T_Semicolon)
		{
			// <ęxpr> ;
			error = ENOP;
			return error;
		}
		else
		{
			fprintf(stderr, "Ocekavan strednik.\n");
			
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
int terms(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("terms\n");
	#endif
	error = ESYN;
	// 32: <TERMS> -> id <TERMS_N>
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(terms_n(input, attr) == ENOP)
		{
			// ID <terms_n>
			error = ENOP;
			return error;
		}
	}
	else
	{
		error = ENOP;
		return error;
	}
	// // 33: <TERMS> -> E ?????????????????????????????????????????????????????
	// else if(token.type == T_RightParenthesis)
	// {
	// 	// )
	// 	error = ENOP;
	// 	return error;
	// }
	return error;
}

/**
 * Simuluje pravidla 34 a 35.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int terms_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("terms_n\n");
	#endif
	error = ESYN;
	// 34: <TERMS_N> -> , id <TERMS_N>
	if(token.type == T_Comma)
	{
		// ,
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// , ID
			getNextToken(input, attr);
			if(terms_n(input, attr) == ENOP)
			{
				// , ID <terms_n>
				error = ENOP;
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			
		}	
	}
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}


/**
 * Simuluje pravidla 8, 9, 10 a nedefinovane pravidlo.
 * @return Index do enumerace chyb.
 */
int type()
{
	#ifdef DEBUG
	printf("type\n");
	#endif
	error = ESYN;
	// 8: <TYPE> -> int
	if(token.type == T_Int)
	{
		// int
		error = ENOP;
		return error;
	}
	// 9: <TYPE> -> double
	else if(token.type == T_Double)
	{
		// double
		error = ENOP;
		return error;
	}
	// 10: <TYPE> -> string 
	else if(token.type == T_String)
	{
		// string
		error = ENOP;
		return error;
	}
	// UNDEF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	else if(token.type == T_Auto)
	{
		// auto
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje nedefinovana pravidla.
 * @return Index do enumerace chyb.
 */
int realtype()
{
	#ifdef DEBUG
	printf("realtype\n");
	#endif
	error = ESYN;
	// UNDEF
	if(token.type == T_Integ)
	{
		error = ENOP;
		return error;
	}
	// UNDEF
	else if(token.type == T_Doub)
	{
		error = ENOP;
		return error;
	}
	// UNDEF
	else if(token.type == T_Str)
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidla 15 a 16.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int params(FILE *input, string *attr)
{
	error = ESYN;
	// 15: <PARAMS> -> <TYPE> id <PARAMS_N>
	if(type() == ENOP)
	{
		// <type>
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// <type> ID
			getNextToken(input, attr);
			if(params_n(input, attr) == ENOP)
			{
				// <type> ID <params_n>
				error = ENOP;
				return error;
			}
		}
	}
	// 16: <PARAMS> -> E
	else
	{
		// E
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Simuluje pravidla 17 a 18.
 * @param  input Soubor obsahujici vstupni kod.
 * @param  attr  String lexemu.
 * @return       Index do enumerace chyb.
 */
int params_n(FILE *input, string *attr)
{
	error = ESYN;
	// 17: <PARAMS_N> -> , <TYPE> id <PARAMS_N>
	if(token.type == T_Comma)
	{
		// ,
		getNextToken(input, attr);
		if(type() == ENOP)
		{
			// , <type>
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				// , <type> ID
				getNextToken(input, attr);
				if(params_n(input, attr) == ENOP)
				{
					// , <type> <params_n>
					error = ENOP;
					return error;
				}
			}
			else
			{
				fprintf(stderr, "Ocekavano ID.\n");
				
			}	
		}
	}
	// 18: <PARAMS_N> -> E
	else
	{
		// E
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * Incializuje tabulku symbolu
 * @return Index do enumerace chyb.
 */
int initSTable()
{
	error = ENOP;
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

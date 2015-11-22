/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.c  -  Syntakticky analyzator (Parser)
 * 
 * Autori:
 * 			xvitas02 – Jakub Vitasek
 * 			xvalec00 – Dusan Valecky
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "error.h"

int line = 0;
TError error;

/**
 * [getNextToken description]
 * @param input [description]
 * @param attr  [description]
 */
void getNextToken(FILE *input, string *attr)
{
	token = getToken(input, attr, &line);
	if(token.type == T_Error) exit(1);
}

/**
 * [parse description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int parse(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("parse\n");
	#endif
	error = ENOP;
	/**
	 * @todo inicializace tabulky symbolu
	 */
	getNextToken(input, attr);
	if(func_n(input, attr) == ENOP)
	{
		/**
		 * @todo generovani instrukci
		 */
	}
	return error;
}

/**
 * [func_n description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int func_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("func_n\n");
	#endif
	error = ESYN;
	
	// P: 2
	if(func(input, attr) == ENOP)
	{
		// <func>
		getNextToken(input, attr);
		if(func_n(input, attr) == ENOP)
		{
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
 * [func description]
 * @return [description]
 */
int func(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("func\n");
	#endif
	error = ESYN;
	// P: 11
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
					error = ENOP;
					return error;
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			return error;
		}
	}
	return error;
}

/**
 * [par_def_list description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int par_def_list(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("par_def_list\n");
	#endif
	error = ESYN;
	if(token.type == T_LeftParenthesis)
	{
		// (
		getNextToken(input, attr);
		if(params(input, attr) == ENOP)
		{
			// (<params
			if(token.type == T_RightParenthesis)
			{
				// (<params>)
				error = ENOP;
				return error;
			}
			else
			{
				fprintf(stderr, "Ocekavana prava zavorka.\n");
				return error;
			}
		}
	}
	else
	{
		fprintf(stderr, "Ocekavana leva zavorka.\n");
		return error;
	}
	return error;
}

/**
 * [dec_or_def description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int dec_or_def(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("dec_or_def\n");
	#endif
	error = ESYN;
	if(comm_seq(input, attr) == ENOP)
	{
		error = ENOP;
		return error;
	}
	else if(token.type == T_Semicolon)
	{
		error = ENOP;
		return error;
	}
	else
	{
		fprintf(stderr, "Ocekavana sekvence prikazu nebo strednik.\n");
		return error;
	}
	return error;
}

/**
 * [comm_seq description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int comm_seq(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("comm_seq\n");
	#endif
	error = ESYN;
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
				return error;
			}
		}
	}
	else
	{
		fprintf(stderr, "Ocekavana leva slozena zavorka.\n");
		return error;
	}
	return error;
}

/**
 * [stmt_list description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int stmt_list(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("stmt_list\n");
	#endif
	error = ESYN;
	// P: 20
	if(stmt(input, attr) == ENOP)
	{
		getNextToken(input, attr);
		if(stmt_list(input, attr) == ENOP)
		{
			error = ENOP;
			return error;
		}
	}
	// P: 21
	else if(token.type == T_RightBrace)
	{
		error = ENOP;
		return error;
	}
	else
	{
		fprintf(stderr, "Ocekavan statement nebo prava slozena zavorka.\n");
		return error;
	}
	return error;
}

/**
 * [stmt description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int stmt(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("stmt\n");
	#endif
	error = ESYN;
	// P: 29
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
				error = ENOP;
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavan znak prirazeni.\n");
			return error;
		}
	}
	// P: 22
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
	// P: 23
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
								error = ENOP;
								return error;
							}
						}
						else
						{
							fprintf(stderr, "Ocekavana prava zavorka.\n");
							return error;
						}
					}
				}
			}
		}	
		else
		{
			fprintf(stderr, "Ocekavana leva zavorka.\n");
			return error;
		}
	}
	// P: 24
	else if(comm_seq(input, attr) == ENOP)
	{
		error = ENOP;
		return error;
	}
	// P: 25
	else if(var_def(input, attr) == ENOP)
	{
		error = ENOP;
		return error;
	}
	// P: 26
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
						return error;
					}
				}
			}
			else
			{
				fprintf(stderr, "Ocekavano ID.\n");
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano >>.\n");
			return error;
		}
	}
	// P: 27
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
					error = ENOP;
					return error;
				}
				else
				{
					fprintf(stderr, "Ocekavan strednik.\n");
					return error;
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano <<.\n");
			return error;
		}
	}
	// P: 28
	else if(ret(input, attr) == ENOP)
	{
		error = ENOP;
		return error;
	}

	return error;
}

/**
 * [ret description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int ret(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("ret\n");
	#endif
	error = ESYN;
	// P: 42
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
				return error;
			}
		}
	}
	else
	{
		fprintf(stderr, "Ocekavano klicove slovo return.\n");
		return error;
	}
	return error;
}

/**
 * [cout_term description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int cout_term(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cout_term\n");
	#endif
	error = ESYN;
	// P: 39
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
	// P: undef
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
		return error;
	}
	return error;
}

/**
 * [cout_term_n description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int cout_term_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cout_term_n\n");
	#endif
	error = ESYN;
	// P: 40
	if(token.type == T_LeftShift)
	{
		// << 
		getNextToken(input, attr);
		if(cout_term(input, attr) == ENOP)
		{
			error = ENOP;
			return error;
		}
	}
	// P: 41
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * [cin_id_n description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int cin_id_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("cin_id_n\n");
	#endif
	error = ESYN;
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
 * [assign description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int assign(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("assign\n");
	#endif
	error = ESYN;
	// P: 36
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
			return error;
		}
	}
	else
	{
		fprintf(stderr, "Ocekavano ID.\n");
		return error;
	}
	return error;
}

/**
 * [var_def description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int var_def(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("var_def\n");
	#endif
	error = ESYN;
	// P: 5
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
					return error;
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			return error;
		}
	}
	// P: 4
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
					return error;
				}
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
			return error;
		}
	}
	return error;
}

/**
 * [init description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int init(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("init\n");
	#endif
	error = ESYN;
	// P: 6
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
 * [if_n description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int if_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("if_n\n");
	#endif
	error = ESYN;
	// P: 43
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
	// P: 44
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

int expr(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("expr\n");
	#endif
	error = ESYN;
	return error;
}

/**
 * [fcall_or_assing description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int fcall_or_assing(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("fcall_or_assing\n");
	#endif
	error = ESYN;
	// P: 31
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
				error = ENOP;
				return error;
			}
		}
		else
		{
			fprintf(stderr, "Ocekavana leva zavorka.\n");
			return error;
		}
	}
	// VYRAZ @todo HARDCODED
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * [terms description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int terms(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("terms\n");
	#endif
	error = ESYN;
	// P: 32
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(terms_n(input, attr) == ENOP)
		{
			error = ENOP;
			return error;
		}
	}
	// P: 33
	else if(token.type == T_RightParenthesis)
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * [terms_n description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int terms_n(FILE *input, string *attr)
{
	#ifdef DEBUG
	printf("terms_n\n");
	#endif
	error = ESYN;
	// P: 35
	if(token.type == T_RightParenthesis)
	{
		error = ENOP;
		return error;
	}
	// 34: <TERMS_N> -> , id <TERMS_N>
	else if(token.type == T_Comma)
	{
		// ,
		getNextToken(input, attr);
		if(terms_n(input, attr) == ENOP)
		{
			error = ENOP;
			return error;
		}
	}
	return error;
}


/**
 * [type description]
 * @return [description]
 */
int type()
{
	#ifdef DEBUG
	printf("type\n");
	#endif
	error = ESYN;
	// P: 8
	if(token.type == T_Int)
	{
		error = ENOP;
		return error;
	}
	// P: 9
	else if(token.type == T_Double)
	{
		error = ENOP;
		return error;
	}
	// P: 10
	else if(token.type == T_String)
	{
		error = ENOP;
		return error;
	}
	// P: undef
	else if(token.type == T_Auto)
	{
		error = ENOP;
		return error;
	}
	else
	{
		fprintf(stderr, "Typ neni int/double/string.\n");
		return error;
	}
	return error;
}

/**
 * [realtype description]
 * @return [description]
 */
int realtype()
{
	#ifdef DEBUG
	printf("realtype\n");
	#endif
	error = ESYN;
	if(token.type == T_Integ)
	{
		error = ENOP;
		return error;
	}
	else if(token.type == T_Doub)
	{
		error = ENOP;
		return error;
	}
	else if(token.type == T_Str)
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * [params description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int params(FILE *input, string *attr)
{
	error = ESYN;
	// P: 15
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
	// P: 16
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

/**
 * [params_n description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int params_n(FILE *input, string *attr)
{
	error = ESYN;
	// P: 17
	if(token.type == T_Comma)
	{
		// ,
		getNextToken(input, attr);
		if(type() == ENOP)
		{
			// , <type>
			getNextToken(input, attr);
			if(params_n(input, attr) == ENOP)
			{
				// , <type> <params_n>
				error = ENOP;
				return error;
			}
		}
	}
	else
	{
		error = ENOP;
		return error;
	}
	return error;
}

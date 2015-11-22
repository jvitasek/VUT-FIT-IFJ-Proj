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

// globalna premenna pre token
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
	error = ENOP;
	/**
	 * @todo inicializace tabulky symbolu
	 */
	getNextToken(input, attr);
	if((error = func_n(input, attr)) == ENOP)
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
	error = ESYN;
	if(type() == ENOP)
	{
		// <type>
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// <type> ID
			getNextToken(input, attr);
			if(token.type == T_LeftParenthesis)
			{
				// <type> ID(
				getNextToken(input, attr);
				if(params(input, attr) == ENOP)
				{
					// <type> ID(<params>
					getNextToken(input, attr);
					// <type> ID(<params>)
					if(comm_seq(input, attr) == ENOP)
					{
						// <type> ID(<params>) { STMT_LIST };
						error = ENOP;
					}
					else if(token.type == T_RightParenthesis)
					{
						// <type> ID(<params>)
						getNextToken(input, attr);
						if(token.type == T_Semicolon)
						{
							// <type> ID(<params>);
							error = ENOP;
						}
						else
						{
							fprintf(stderr, "Ocekavan strednik.\n");
						}
					}
					else
					{
						fprintf(stderr, "Ocekavan prava zavorka nebo telo funkce.\n");
					}
				}
				else
				{
					fprintf(stderr, "Ocekavany parametry nebo prava zavorka.\n");
				}
			}
			else
			{
				fprintf(stderr, "Ocekavana leva zavorka.\n");
			}
		}
		else
		{
			fprintf(stderr, "Ocekavano ID.\n");
		}
	}
	else
	{
		fprintf(stderr, "Ocekavan typ.\n");
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
			}
			else
			{
				fprintf(stderr, "Ocekavana prava slozena zavorka.\n");
			}
		}
	}
	else
	{
		fprintf(stderr, "Ocekavana leva slozena zavorka.\n");
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
	error = ESYN;
	// P: 20
	if(stmt(input, attr) == ENOP)
	{
		getNextToken(input, attr);
		if(stmt_list(input, attr) == ENOP)
		{
			error = ENOP;
		}
	}
	// P: 21
	else if(token.type == T_RightBrace)
	{
		error = ENOP;
	}
	else
	{
		fprintf(stderr, "Ocekavan statement nebo prava slozena zavorka.\n");
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
			}
		}
		else
		{
			fprintf(stderr, "Ocekavan znak prirazeni.\n");
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
	// P: 24
	else if(comm_seq(input, attr) == ENOP)
	{
		error = ENOP;
	}
	// P: 25
	else if(var_def(input, attr) == ENOP)
	{
		error = ENOP;
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
	// P: 28
	else if(ret(input, attr) == ENOP)
	{
		error = ENOP;
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
 * [cout_term description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int cout_term(FILE *input, string *attr)
{
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
		}
	}
	else
	{
		fprintf(stderr, "Ocekavano ID.\n");
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
	error = ESYN;
	// P: 40
	if(token.type == T_LeftShift)
	{
		// << 
		getNextToken(input, attr);
		if(cout_term(input, attr) == ENOP)
		{
			error = ENOP;
		}
	}
	// P: 41
	else
	{
		error = ENOP;
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
	error = ESYN;
	if(token.type == T_RightShift)
	{
		// >>
		getNextToken(input, attr);
		if(token.type == T_Id)
		{
			// >> ID
			getNextToken(input, attr);
			if(cin_id_n(input, attr))
			{
				// >> ID <cin_id_n>
				error = ENOP;
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
 * [var_def description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int var_def(FILE *input, string *attr)
{
	error = ESYN;
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
	// P: 5
	else if(token.type == T_Auto)
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
 * [init description]
 * @param  input [description]
 * @param  attr  [description]
 * @return       [description]
 */
int init(FILE *input, string *attr)
{
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
		}
	}
	else
	{
		error = ENOP;
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
		}
	}
	// P: 44
	else
	{
		error = ENOP;
	}
	return error;
}

int expr(FILE *input, string *attr)
{
	error = ENOP;

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
			}
		}
		else
		{
			fprintf(stderr, "Ocekavana leva zavorka.\n");
		}
	}
	// VYRAZ @todo HARDCODED
	else
	{
		error = ENOP;
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
	error = ESYN;
	// P: 32
	if(token.type == T_Id)
	{
		// ID
		getNextToken(input, attr);
		if(terms_n(input, attr) == ENOP)
		{
			error = ENOP;
		}
	}
	// P: 33
	else if(token.type == T_RightParenthesis)
	{
		error = ENOP;
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
	error = ESYN;
	// P: 35
	if(token.type == T_RightParenthesis)
	{
		error = ENOP;
	}
	// 34: <TERMS_N> -> , id <TERMS_N>
	else if(token.type == T_Comma)
	{
		// ,
		getNextToken(input, attr);
		if(terms_n(input, attr) == ENOP)
		{
			error = ENOP;
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
	error = ESYN;
	// P: 8
	if(token.type == T_Int)
	{
		error = ENOP;
	}
	// P: 9
	else if(token.type == T_Double)	// pravidlo 3
	{
		error = ENOP;
	}
	// P: 10
	else if(token.type == T_String)
	{
		error = ENOP;
	}
	else
	{
		fprintf(stderr, "Typ neni int/double/string.\n");
	}
	return error;
}

/**
 * [realtype description]
 * @return [description]
 */
int realtype()
{
	error = ESYN;
	if(token.type == T_Integ)
	{
		error = ENOP;
	}
	else if(token.type == T_Doub)
	{
		error = ENOP;
	}
	else if(token.type == T_Str)
	{
		error = ENOP;
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
	if(token.type == T_Comma)	// pravislo 6
	{
		// Uz mame ,
		getNextToken(input, attr);
		// Simulace pravidla 2 a 3 (TYPE)
		if(type() == ENOP)
		{
			// Uz mame , TYPE
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				// Uz mame , TYPE id
				getNextToken(input, attr);
				// Simulace pravidla 6 a 7 (PARAMS_N)
				if(params_n(input,attr))
				{
					// Uz mame , TYPE id PARAMS_N
					error = ENOP;
				}
				else
				{
					fprintf(stderr, "Spatne zadane parametry funkce.\n");
				}
			}
			else
			{
				fprintf(stderr, "Ocekavano ID funkce.\n");
			}
		}
		else
		{
			fprintf(stderr, "Ocekavan int/double/string za carkou.\n");
		}		 
	}
	else if(token.type == T_RightParenthesis)	//pravidlo 7
	{
		error = ENOP;
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
	if((token.type == T_Int) || (token.type == T_Double) || (token.type == T_String))
	{
		// Simulace pravidla 2 a 3 (TYPE)
		if(type() == ENOP)
		{
			// Uz mame TYPE
			getNextToken(input, attr);
			if(token.type == T_Id)
			{
				// Uz mame TYPE id
				getNextToken(input, attr);
				// Simulace pravidla 6 a 7 (PARAMS_N)
				if(params_n(input,attr) == ENOP)
				{
					// Uz mame TYPE id PARAMS_N
					error = ENOP;
				}
				else
				{
					fprintf(stderr, "Spatne zadane parametry funkce.\n");
				}
			}
			else
			{
				fprintf(stderr, "Ocekavano ID funkce.\n");
			}
		}		
	}
	else if(token.type == T_RightParenthesis)	//pravidlo 5
	{
		error = ENOP;
	}
	return error;
}

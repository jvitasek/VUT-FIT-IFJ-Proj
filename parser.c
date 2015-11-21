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
	token = getToken(input,attr,&line);
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
		if((error = stmt_list(input, attr)) == ENOP)
		{
			// { STMT_LIST
			getNextToken(input, attr);
			if(token.type == T_RightBrace)
			{
				// { STMT_LIST }
				error = ENOP;
			}
		}
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
	if((error = stmt(input, attr)) == ENOP)
	{
		getNextToken(input, attr);
		if((error = stmt_list(input, attr)) == ENOP)
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
	}

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
	if(token.type == T_Int)	// pravidlo 2
	{
		error = ENOP;
	}
	else if(token.type == T_Double)	// pravidlo 3
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
		else
		{
			fprintf(stderr, "Typ neni int/double/string.\n");
		}		
	}
	else if(token.type == T_RightParenthesis)	//pravidlo 5
	{
		error = ENOP;
	}
	return error;
}

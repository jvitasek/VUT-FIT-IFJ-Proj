/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.c  -  Syntakticky analyzator(Parser)
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>		// boolean
#include "scanner.h"


T_Token token;	// globalna premenna pre token
int line = 1;

/*
 * Funkcia, ktora ulozi nasledujuci token do globalnej premennej "token"
 */
void getNextToken(FILE *input, string *attr)
{
	token = getToken(input,attr,&line);
	if(token.type == T_Error) exit(1);
}

/*
 * Funkcia, ktora simuluje pravidla 2: TYPE -> int a 3: TYPE -> double
 */
bool type()
{
	bool ret = false;
	if(token.type == T_Int)	// pravidlo 2
	{
		ret = true;
	}else if(token.type == T_Double)	// pravidlo 3
	{
		ret = true;
	}
	return ret;
}

/*
 * Funkcia, ktora simuluje pravidla 6: PARAMS ->, TYPE id PARAMS_N a 7: PARAMS_N -> E 
 */
bool params_n(FILE *input, string *attr)
{
	bool ret = false;
	if(token.type == T_Comma)	// pravislo 6
	{
		// Uz mame ,
		getNextToken(input,attr);
		// Simulace pravidla 2 a 3 (TYPE)
		if(type())
		{
			// Uz mame , TYPE
			getNextToken(input,attr);
			if(token.type == T_Id)
			{
				// Uz mame , TYPE id
				getNextToken(input,attr);
				// Simulace pravidla 6 a 7 (PARAMS_N)
				if(params_n(input,attr))
				{
					// Uz mame , TYPE id PARAMS_N
					ret = true;	// cize nastavime navratovy hodnotu na true, pretoze syntax je v poriadku
				}else
				{
					fprintf(stderr,"Zle zadane parametre funkcie.\nLine: %d\n",token.line);
					return false;
				}
			}else
			{
				fprintf(stderr,"Nebolo zadane id funkcie.\nLine: %d\n",token.line);
				return false;
			}
		}else
		{
			fprintf(stderr,"Nebol zadany int/double za \",\".\nLine: %d\n",token.line);
			return false;
		}		 
	}else if(token.type == T_RightParenthesis)	//pravidlo 7
	{
		ret = true;
	}
	return ret;
}

/*
 * Funkcia, ktora simuluje pravidla 4: PARAMS -> TYPE id PARAMS_N a 5: PARAMS -> E 
 */
bool params(FILE *input, string *attr)
{
	bool ret = false;
	if((token.type == T_Int) || (token.type == T_Double))	// pravidlo 4
	{
		// Simulace pravidla 2 a 3 (TYPE)
		if(type())
		{
			// Uz mame TYPE
			getNextToken(input,attr);
			if(token.type == T_Id)
			{
				// Uz mame TYPE id
				getNextToken(input,attr);
				// Simulace pravidla 6 a 7 (PARAMS_N)
				if(params_n(input,attr))
				{
					// Uz mame TYPE id PARAMS_N
					ret = true;	// cize nastavime navratovy hodnotu na true, pretoze syntax je v poriadku
				}else
				{
					fprintf(stderr,"Zle zadane parametre funkcie.\nLine: %d\n",token.line);
					return false;
				}
			}else
			{
				fprintf(stderr,"Nebolo zadane id funkcie.\nLine: %d\n",token.line);
				return false;
			}
		}else
		{
			fprintf(stderr,"Typ nebol int/double.\nLine: %d\n",token.line);
			return false;
		}		
	}else if(token.type == T_RightParenthesis)	//pravidlo 5
	{
		ret = true;
	}
	return ret;
}

/*
 * Funkcia, ktora simuluje pravidlo 8: BODY -> ;
 */
bool body()
{
	bool ret = false;
	if(token.type == T_Semicolon)
	{
		ret = true;
	}
	return ret;
}

/*
 * Funkcia, ktora simuluje pravidlo 1: FUNC -> TYPE id ( PARAMS ) { BODY }
 */
bool func(FILE *input, string *attr)
{
	getNextToken(input,attr);
	bool ret = false;
	if((token.type == T_Int) || (token.type == T_Double))
	{
		// Simulace pravidla 2 a 3 (TYPE)
		if(type())
		{
			getNextToken(input,attr);
			if(token.type == T_Id)
			{
				// Uz mame TYPE id
				getNextToken(input,attr);
				if(token.type == T_LeftParenthesis)
				{
					// Uz mame TYPE id (
					getNextToken(input,attr);
					// Simulace pravidla 4 a 5 (PARAMS)
					if(params(input,attr))
					{
						// Uz mame TYPE id ( PARAMS
						//getNextToken(input,attr);
						if(token.type == T_RightParenthesis)
						{
							// Uz mame TYPE id ( PARAMS )
							getNextToken(input,attr);
							if(token.type == T_LeftBrace)
							{
								// Uz mame TYPE id ( PARAMS ) {
								getNextToken(input,attr);
								// Simulace pravidla 8 (BODY)
								if(body())
								{
									// Uz mame TYPE id ( PARAMS ) { BODY
									getNextToken(input,attr);
									if(token.type == T_RightBrace)
									{
										// Uz mame vsetko TYPE id ( PARAMS ) { BODY }
										ret = true;	// cize nastavime navratovy hodnotu na true, pretoze syntax je v poriadku
									}else
									{
										fprintf(stderr,"Nebola zadana \"}\" po tele.\nLine: %d\n",token.line);
										return false;
									}
								}else
								{
									fprintf(stderr,"Nebolo zadane telo po \"{\".\nLine: %d\n",token.line);
									return false;
								}
							}else
							{
								fprintf(stderr,"Nebola zadana \"{\" po definicii funkcie.\nLine: %d\n",token.line);
								return false;
							}
						}else
						{
							fprintf(stderr,"Nebola zadana \")\" po parametroch funkcie.\nLine: %d\n",token.line);
							return false;
						}
					}else
					{
						fprintf(stderr,"Nespravne zadane parametre funkcie.\nLine: %d\n",token.line);
						return false;
					}
				}else
				{
					fprintf(stderr,"Nebola zadana \"(\" po nazve funkcie.\nLine: %d\n",token.line);
					return false;
				}
			}else
			{
				fprintf(stderr,"Nebolo zadane id funkcie.\nLine: %d\n",token.line);
				return false;
			}
		}else
		{
			fprintf(stderr,"Typ nebol int/double.\nLine: %d\n",token.line);
			return false;
		}
	}else fprintf(stderr,"Nebol zadany int/double na zaciatku funkcie.\nLine: %d\n",token.line);
	return ret;
}


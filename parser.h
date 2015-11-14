/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.h  -  Hlavickovy subor pre parser
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */
 
#include "str.h"
#include <stdbool.h>
 
void getNextToken(FILE *input, string *attr);
bool func(FILE *input, string *attr);

/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.h  -  Hlavickovy soubor parseru
 * 
 * Autori:
 * 			xvitas02 – Jakub Vitasek
 * 			xvalec00 – Dusan Valecky
 */
 
#include "scanner.h"
#include <stdbool.h>
 
void getNextToken(FILE *input, string *attr);
int parse(FILE *input, string *attr);

// PRAVIDLA
int type();
int params(FILE *input, string *attr);
int params_n(FILE *input, string *attr);
int func_n(FILE *input, string *attr);
int comm_seq(FILE *input, string *attr);
int stmt_list(FILE *input, string *attr);
int stmt(FILE *input, string *attr);
int fcall_or_assing(FILE *input, string *attr);
int terms(FILE *input, string *attr);
int terms_n(FILE *input, string *attr);

T_Token token;

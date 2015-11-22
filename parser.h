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

// POM. FUNKCE
int type();
int realtype();

// PRAVIDLA
int params(FILE *input, string *attr);
int params_n(FILE *input, string *attr);
int func_n(FILE *input, string *attr);
int comm_seq(FILE *input, string *attr);
int stmt_list(FILE *input, string *attr);
int stmt(FILE *input, string *attr);
int fcall_or_assing(FILE *input, string *attr);
int terms(FILE *input, string *attr);
int terms_n(FILE *input, string *attr);
int if_n(FILE *input, string *attr);
int expr(FILE *input, string *attr);
int assign(FILE *input, string *attr);
int var_def(FILE *input, string *attr);
int init(FILE *input, string *attr);
int cout_term(FILE *input, string *attr);
int cout_term_n(FILE *input, string *attr);
int cin_id_n(FILE *input, string *attr);
int ret(FILE *input, string *attr);

T_Token token;

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
 
 // HLAVNI FUNKCE
int parse(FILE *input, string *attr); // P: 1

// POM. FUNKCE
void getNextToken(FILE *input, string *attr);

// PRAVIDLA
int type(); // P: 8, 9, 10
int realtype(); // P: undef
int params(FILE *input, string *attr); // P: 15, 16
int params_n(FILE *input, string *attr); // P: 17, 18
int func(FILE *input, string *attr); // P: 11
int func_n(FILE *input, string *attr); // P: 2, 3
int par_def_list(FILE *input, string *attr); // P: 14
int dec_or_def(FILE *input, string *attr); // P: 12, 13
int comm_seq(FILE *input, string *attr); // P: 19
int stmt_list(FILE *input, string *attr); // P: 20, 21
int stmt(FILE *input, string *attr); // P: 22, 23, 24, 25, 26, 27, 28, 29
int fcall_or_assing(FILE *input, string *attr); // P: 30, 31
int terms(FILE *input, string *attr); // P: 32, 33
int terms_n(FILE *input, string *attr); // P: 34, 35
int if_n(FILE *input, string *attr); // P: 43, 44
int expr(FILE *input, string *attr); // P: @todo
int assign(FILE *input, string *attr); // P: 36
int var_def(FILE *input, string *attr); // P: 4, 5
int init(FILE *input, string *attr); // P: 6
int cout_term(FILE *input, string *attr); // P: 39, undef
int cout_term_n(FILE *input, string *attr); // P: 40, 41
int cin_id_n(FILE *input, string *attr); // P: 37
int ret(FILE *input, string *attr); // P: 42


T_Token token;

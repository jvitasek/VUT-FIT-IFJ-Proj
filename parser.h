/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * parser.h  -  Hlavickovy soubor parseru
 * 
 * Autori:
 * 			xvitas02 – Jakub Vitasek
 * 			xvalec00 – Dusan Valecky
 */
 
#include "ial.h"
#include <stdbool.h>
#define KW 10
 
 // HLAVNI FUNKCE
TError parse(FILE *input); // P: 1

// POM. FUNKCE
void getNextToken(FILE *input, string *attr);
TError initSTable(tHTable **table);

// PRAVIDLA
TError type(); // P: 8, 9, 10
TError realtype(); // UNDEF
TError params(FILE *input); // P: 15, 16
TError params_n(FILE *input); // P: 17, 18
TError func(FILE *input); // P: 11
TError func_n(FILE *input); // P: 2, 3
TError par_def_list(FILE *input); // P: 14
TError dec_or_def(FILE *input); // P: 12, 13
TError comm_seq(FILE *input); // P: 19
TError stmt_list(FILE *input); // P: 20, 21
TError stmt(FILE *input); // P: 22, 23, 24, 25, 26, 27, 28, 29
TError fcall_or_assign(FILE *input); // P: 30, 31
TError terms(FILE *input); // P: 32, 33
TError terms_n(FILE *input); // P: 34, 35
TError if_n(FILE *input); // P: 43, 44
TError assign(FILE *input); // P: 36
TError var_def(FILE *input); // P: 4, 5
TError init(FILE *input); // P: 6
TError cout_term(FILE *input); // P: 39, undef
TError cout_term_n(FILE *input); // P: 40, 41
TError cin_id_n(FILE *input); // P: 37
TError ret(FILE *input); // P: 42

T_Token token;

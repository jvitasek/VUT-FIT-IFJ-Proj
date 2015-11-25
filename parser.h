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
 
 // HLAVNI FUNKCE
TError parse(FILE *input, string *attr); // P: 1

// POM. FUNKCE
void getNextToken(FILE *input, string *attr);
TError initSTable();

// PRAVIDLA
TError type(); // P: 8, 9, 10
TError realtype(); // UNDEF
TError params(FILE *input, string *attr); // P: 15, 16
TError params_n(FILE *input, string *attr); // P: 17, 18
TError func(FILE *input, string *attr); // P: 11
TError func_n(FILE *input, string *attr); // P: 2, 3
TError par_def_list(FILE *input, string *attr); // P: 14
TError dec_or_def(FILE *input, string *attr); // P: 12, 13
TError comm_seq(FILE *input, string *attr); // P: 19
TError stmt_list(FILE *input, string *attr); // P: 20, 21
TError stmt(FILE *input, string *attr); // P: 22, 23, 24, 25, 26, 27, 28, 29
TError fcall_or_assign(FILE *input, string *attr); // P: 30, 31
TError terms(FILE *input, string *attr); // P: 32, 33
TError terms_n(FILE *input, string *attr); // P: 34, 35
TError if_n(FILE *input, string *attr); // P: 43, 44
TError assign(FILE *input, string *attr); // P: 36
TError var_def(FILE *input, string *attr); // P: 4, 5
TError init(FILE *input, string *attr); // P: 6
TError cout_term(FILE *input, string *attr); // P: 39, undef
TError cout_term_n(FILE *input, string *attr); // P: 40, 41
TError cin_id_n(FILE *input, string *attr); // P: 37
TError ret(FILE *input, string *attr); // P: 42

T_Token token;

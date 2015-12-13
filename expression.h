/**
* Implementace interpretu imperativniho jazyka IFJ15 *
* 
* expression.h  -  Zpracování výrazů
* 
* Autori:
* 			xvitas02 – Jakub Vitasek
* 			xvalec00 – Dusan Valecky
*/

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdlib.h>
#include "ial.h"


/**
 * Mozne symboly ve vyrazech.
 */
typedef enum PSymbols {
	PPlus,   // 0 +
	PMinus,  // 1 -
	PMul,    // 2 *
	PDiv,    // 3 /
	PEqual,  // 4 ==
	PNotEq,  // 5 !=
	PLessEq, // 6 <=
	PGreatEq, // 7 >=
	PGreat,  // 8 >
	PLess,	 // 9 <
	PString, // 10 string
	PDouble, // 11 double
	PInt, 	 // 12 integer
	PIden,   // 13 id
	PIdFun,  // 14 f
	PLeftP,  // 15 (
	PRightP, // 16 )
	PComma,  // 17 ,
	PDollar, // 18 $
	PLessReduc,	// 19 < na zasobniku
	PNonTerm, // 20 neterminal
	less,	 // 21 < v preceden. tabulce
	great,	 // 22 > v preceden. tabulce
	equal,	 // 23 = v preceden. tabulce
	empty,	 // 24 'empty' v preceden. tabulce
	none	// jine
} PSymbols;

/**
 * Pravidla pro vyrazy.
 */
typedef enum ruleType {
	ADD_RULE,	// E -> E + E
	SUB_RULE,	// E -> E - E
	MUL_RULE,	// E -> E * E
	DIV_RULE,	// E -> E / E
	LESSGREAT_RULE, // E -> E > E, E -> E >= E, ...
	PAR_RULE,	// E -> (E)
	ID_E_RULE,	// E -> i ... i pro string, int, double
	FUNC_RULE	// E -> f(E) ... @todo f(E,E) ...
} ruleType;

/**
 * Typy termu.
 */
typedef enum hashType {  
	Tstring,	// 0 string
	Tdouble,	// 1 double
	Tint,		// 2 integer
	Tid,		// 3 identifikator
	Tother	// 4 jiny
} hashType;

/**
 * Zasobnik s ukazatelem na prvek pred a za
 */
typedef struct TstackElem {
	struct TstackElem *Lptr;
	struct TstackElem *Rptr;
	PSymbols termType;
	hashType idType;
	char *data;
	tHTItem *tsItem;
} *TstackElemPtr;


typedef struct Tstack {
	TstackElemPtr first;
	TstackElemPtr top;
} Tstack;

TError stack_init(); // inicialzace zasobniku
TError stack_dispose();	
TError stack_pop(tHTItem **item, int ptrAss);
TError stack_push(int tokterm, char *attr);
TstackElemPtr stack_top();
TError stack_shift(int tokterm, char *attr);

int tok_to_term(int tokenType);
TError find_rule(ruleType rule);
void generate_variable(string *var, int *counter);
void generate_inst(tInstCode instType, void *op1, void *op2, void *res);
TError expr(FILE *input, string *attr, int semi_or_par, int *count, tHTable **localTable, tHTItem **exprRes); // P: @todo

#endif

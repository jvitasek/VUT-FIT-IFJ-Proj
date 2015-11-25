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
#include "ilist.h"
//#include ""

typedef enum PSymbols {
	PPlus,    // 0 +
	PMinus,   // 1 -
	PMul,     // 2 *
	PDiv,     // 3 /
	PEqual,    // 4 ==
	PNotEq,   // 5 !=
	PLessEq,  // 6 <=
	PGreatEq, // 7 >=
	PGreat,   // 8 >
	PLess,    // 9 <
	PIden,    // 10 id
	PIdFun,   // 11 f
	PLeftP,   // 12 (
	PRightP,  // 13 )
	PComma,   // 14 ,
	PDollar,  // 15 $
	PLessReduc,	// 16 < na zasobniku
	PNonTerm,		// 17 neterminal
	less,		// 18 < v preceden. tabulce
	great,	// 19 > v preceden. tabulce
	equal,	// 20 = v preceden. tabulce
	empty,	// 21 'nic' v preceden. tabulce
	none
} PSymbols;

typedef enum hashType {  
	Tstring,	// 0 string
	Tdouble,	// 1 double
	Tint,		// 2 integer
	Tid,		// 3 identifikator
	Tother	// 4 jiny
} hashType;

typedef struct TstackElem {
	struct TstackElem *Lptr;
	struct TstackElem *Rptr;
	PSymbols termType;
	hashType idType;
	char *data;
} *TstackElemPtr;

typedef struct Tstack {
	TstackElemPtr first;
	TstackElemPtr top;
} Tstack;

int StackInit(Tstack *stack); // inicialzace zasobniku
void StackDispose(Tstack *stack);	
void StackPop(Tstack *stack);
int StackPush(Tstack *stack, int tokterm);
TstackElemPtr StackTop(Tstack *stack);
int StackShift(Tstack *stack/*, T_Token token*/);
int StackEmpty(Tstack *stack);
void generateVariable(string *var, int *counter);
void generateInst(tInstCode instType, void *op1, void *op2, void *res);
TError expr(FILE *input, string *attr, int semi_or_par, int *count); // P: @todo

#endif

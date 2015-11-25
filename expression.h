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
	PLeftShift, // 10 <<
	PRightShift, // 11 >>
	PAssign,	// 12 =
	PIden,    // 13 id
	PIdFun,   // 14 f
	PLeftP,   // 15 (
	PRightP,  // 16 )
	PComma,   // 17 ,
	PDollar,  // 18 $
	PLessReduc,	// 23 < na zasobniku
	PNonTerm,		// 24 neterminal
	less,		// 19 < v preceden. tabulce
	great,	// 20 > v preceden. tabulce
	equal,	// 21 = v preceden. tabulce
	empty	// 22 'nic' v preceden. tabulce 
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
int StackPush(Tstack *stack, TstackElemPtr item);
TstackElemPtr StackTop(Tstack *stack);
int StackShift(Tstack *stack/*, T_Token token*/);
int StackEmpty(Tstack *stack);
TError expr(FILE *input, string *attr); // P: @todo

#endif

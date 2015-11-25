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

typedef enum Tsymbols {
	Tplus,    // 0 +
	Tminus,   // 1 -
	Tmul,     // 2 *
	Tdiv,     // 3 /
	Tequal,    // 4 ==
	TnotEq,   // 5 !=
	TlessEq,  // 6 <=
	TgreatEq, // 7 >=
	Tgreat,   // 8 >
	Tless,    // 9 <
	TleftShift, // 10 <<
	TrightShift, // 11 >>
	Tassign,	// 12 =
	Tiden,    // 13 id
	Tidfun,   // 14 f
	TleftP,   // 15 (
	TrightP,  // 16 )
	Tcomma,   // 17 ,
	Tdollar,  // 18 $
	less,		// 19 < v preceden. tabulce
	great,	// 20 > v preceden. tabulce
	equal,	// 21 = v preceden. tabulce
	empty,	// 22 'nic' v preceden. tabulce 
	lessReduc	// 23 < na zasobniku
} Tsymbols;

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
	Tsymbols type;
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

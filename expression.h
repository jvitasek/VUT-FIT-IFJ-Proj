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
	Tplus,    // +
	Tminus,   // -
	Tmul,     // *
	Tdiv,     // /
	Tequal,    // ==
	TnotEq,   // !=
	TlessEq,  // <=
	TgreatEq, // >=
	Tgreat,   // >
	Tless,    // <
	TleftShift, // <<
	TrightShift, // >>
	Tassign,	// =
	Tiden,    // id
	Tidfun,   // f
	TleftP,   // (
	TrightP,  // )
	Tcomma,   // ,
	Tdollar,  // $
	less,
	great,
	equal,
	empty,
	lessReduc	// < na zasobniku
} Tsymbols;

typedef enum hashType {  
	Tstring,
	Tdouble,
	Tint,
	Tother
} hashType;

typedef struct TstackElem {
	struct TstackElem *Lptr;
	struct TstackElem *Rptr;
	Tsymbols symbols;
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
int StackPush(Tstack *stack/*, T_Token token*/);
TstackElemPtr StackTop(Tstack *stack);
void StackShift(Tstack *stack, T_Token token);
int StackEmpty(Tstack *stack);
int readExpr(FILE *input, string *attr);
int expr(FILE *input, string *attr); // P: @todo

#endif

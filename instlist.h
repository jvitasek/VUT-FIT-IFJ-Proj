/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * instList.h  -  Hlavickovy subor pre zoznam instrukcii trojadresneho kodu
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#ifndef INSTLIST_H
#define INSTLIST_H

typedef enum
{
  C_Add,  		// + (C_Add,op1,op2,res)
  C_Sub,  		// - (C_Sub,op1,op2,res)
  C_Mul,  		// * (C_Mul,op1,op2,res)
  C_Div,  		// / (C_Div,op1,op2,res)

  C_Less,		// <  (C_Less,op1,op2,res)
  C_Greater,	// >  (C_Greater,op1,op2,res)
  C_LessEq,		// <= (C_LessEq,op1,op2,res)
  C_GreaterEq,	// >= (C_GreaterEq,op1,op2,res)
  C_Equal,		// == (C_Equal,op1,op2,res)
  C_NotEqual,	// != (C_NotEqual,op1,op2,res)
  
  C_Cin,
  C_Cout,
  
  C_Length,
  C_Substr,
  C_Concat,
  C_Find,
  C_Sort,
  
  C_Label,
  C_Not,
  C_IfGoTo,
  C_ElseGoTo,
  C_ForGoTo,
  C_Start,
  C_EndMain,
  C_GoTo,
  
  C_Ret,		// (C_Ret,op1,-,-)

  C_Assign,		// = (C_Assign,op1,-,res)
  C_Stop,		// (C_Stop,-,-,-)
} tInstCode;

typedef struct
{
  tInstCode instCode;	//operator
  void *op1;		//ukazatel na operand1
  void *op2;		//ukazatel na operand2
  void *result;		//ukazatel na vysledok
} tInstruct;

typedef struct listItem
{
  tInstruct instruct;
  struct listItem *nextItem;	//ukazatel na dalsi prvok
} tListItem;

typedef struct
{
  struct listItem *First;	//ukazatel na prvy prvok
  struct listItem *Act;		//ukazatel na aktivny prvok
  struct listItem *Last;	//ukazatel na posledny prvok
} tInstList;



void listInit(tInstList *L);
void listDispose(tInstList *L);
void listInsertLast(tInstList *L, tInstruct I);
void listFirst(tInstList *L);
void listNext(tInstList *L);
void listGoTo(tInstList *L, void *gotoInstr);
void *listGetPointerAct(tInstList *L);
tInstruct *listGetPointerLast(tInstList *L);
tInstruct *listGetData(tInstList *L);
void printElementsOfList(tInstList L);
#endif

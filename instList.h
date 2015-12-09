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
  C_Add,  //+ (C_Add,op1,op2,res)
  C_Sub,  //- (C_Sub,op1,op2,res)
  C_Mul,
  C_Div,

  C_Less,
  C_Greater,
  C_LessEq,		// <=
  C_GreaterEq,	// >=
  C_Equal,
  C_NotEqual,	// <>
  
  C_Read,
  C_Write,
  C_Length,
  C_Sort,
  C_Find,
  C_Copy,
  
  C_Label,
  C_Not,
  C_IfGoTo,
  C_GoTo,
  
  C_Ret,

  C_Assign		// :=
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
tInstruct *listGetPointerLast(tInstList *L);
tInstruct *listGetData(tInstList *L);
void print_elements_of_list(tInstList L);
#endif

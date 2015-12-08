#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include "stack.h"
#include "error.h"
//#include "interpret.h"


tListOfInstr *list;

TError generateInstruction(tInstCode instType, TypeI typ1, int op1, TypeI typ2, int op2, TypeI typ3, iOperand *op3 )
// vlozi novou instrukci do seznamu instrukci
{
  TError error;
  error = ENOP;

  tInst* I;
  I = malloc(sizeof(tInst));
  I->op1 = malloc(sizeof(iOperand));
  I->op2 = malloc(sizeof(iOperand));
  I->res = malloc(sizeof(iOperand));
  if (!I || !(I->op1) || !(I->op2) || !(I->res)) {
    fprintf(stderr, "Chyba pri malloc.\n");
    error = EINT;
    return error;
  }   

  I->instType = instType;
  I->op1->type = typ1;        
  I->op1->obsah = op1;
  I->op2->type = typ2;  
  I->op2->obsah = op2;
  I->res->type = typ3;

  listInsertLast(list, *I);

  return error;
}

TError inter(tListOfInstr *list)
{
  TError error;
  error = ENOP;

  listFirst(list);
  tInst* I;
  I = malloc(sizeof(tInst));

  int value;
  tStackP stack;
  SInitP (&stack); 

  while (1)
  {
    I = listGetData(list);

    /*
    int a = -1;
    if(I->op1->obsah)
    {
      a = (int) I->op1->obsah;
    }
    printf("%d\n", a);
    */

    switch (I->instType)
    {
      // aritmeticke operace
      case I_ADD:
        I->res->obsah = (int) I->op1->obsah + (int) I->op2->obsah;
        SPushP (&stack, I->res->obsah); 
        printf("%d\n",I->res->obsah);
        break;

      case I_SUB:
        value = STopPopP (&stack);        
        I->res->obsah = value - (int) I->op1->obsah;        
        SPushP (&stack, I->res->obsah); 
        printf("%d\n",I->res->obsah);
        break;

      case I_MUL:
        value = STopPopP(&stack);
        I->res->obsah = value * (int) I->op1->obsah;
        SPushP (&stack, I->res->obsah); 
        printf("%d\n",I->res->obsah);
        break;

      case I_DIV:

        break;

      // relacni operace
      case I_EQ:
        break;

      case I_NEQ:
        break;

      case I_LE:
        break;

      case I_LEQ:
        break;

      case I_GE:
        break;

      case I_GEQ:
        break;

      case I_NEG:
        break;

      case I_STOP:  
        error = ENOP;
        return error;
        break;

      default: 
        error = ENOP;
        return error;
    }
    listNext(list);
  }

  return error;
}


int main(int argc, char** argv)
{
  list = malloc(sizeof(tListOfInstr));
  listInit(list);

  int a = 2;
  int *b = &a;

  int c = 20;
  int *d = &c; 

  int e = 4;
  int *f = &e; 

  generateInstruction(I_ADD, INT, *b, INT, *f, INT, NULL);
  generateInstruction(I_SUB, INT, *b, INT, *f, INT, NULL);
  generateInstruction(I_MUL, INT, *b, INT, *d, INT, NULL);
  generateInstruction(I_STOP, INT, *b, INT, *d, INT, NULL);

  inter(list);
  listFree(list);

  return 0;  
}


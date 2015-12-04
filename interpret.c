#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include "stack.h"
//#include "interpret.h"
tListOfInstr *list;
tInst *I;

void generateInstruction(tInstCode instType, TypeI typ1, int op1, TypeI typ2, int op2, TypeI typ3, iOperand *op3 )
// vlozi novou instrukci do seznamu instrukci
{


   tInst* I = malloc(sizeof(struct tInst));
   I = malloc(sizeof(tInst));
   I->op1 = malloc(sizeof(iOperand));
   I->op2 = malloc(sizeof(iOperand));
   I->res = malloc(sizeof(iOperand));

   
   I->instType = instType;


   I->op1->type = typ1;
         

   I->op1->obsah = op1;

   I->op2->type = typ2;
  
   I->op2->obsah = op2;

   I->res->type = typ3;
   //printf("%d\n", I->op1->obsah );
   listInsertLast(list, *I);

}

void inter(tListOfInstr *list)
{

  listFirst(list);
  int vys;
  int sub, mul;
  tStackP stack;
  SInitP (&stack); 
  tInst *I = malloc(sizeof(tInst));

  for(int it=0; it<5; it++)
  {
    
    I = listGetData(list);




    //TADY POTŘEBUJU, ABY SE MI NAHRÁLA 8 DO PROM. a

    //printf("%d\n", a);


    
    switch (I->instType)
    {

      case 0:
        //printf("I_ADD\n");
        I->res->obsah = (int) I->op1->obsah + (int) I->op2->obsah;
        
        SPushP (&stack, I->res->obsah); 
  
        break;
      case 1:
         //printf("I_SUB\n");
        
        sub = STopPopP (&stack);
        
        I->res->obsah = sub - (int) I->op1->obsah;
        
        //SPushP (&stack, I->res->obsah); 
        SPushP (&stack, I->res->obsah); 
        
        break;
      case 2:
        
        mul = STopPopP(&stack);
        I->res->obsah = mul * (int) I->op1->obsah;
        SPushP (&stack, I->res->obsah); 
        break;

      case 17:
        printf("koncim...\n");
        return;
        break;

      case 18:
        vys = STopPopP (&stack);
        printf("%d\n", vys);
        break;
      default:
      
      
      break;

    }
    listNext(list);
    
  }
  
}


int main(int argc, char** argv)
{
   list = malloc(sizeof(tListOfInstr));
   listInit(list);

  //(8+1)-3

  int a;       // definice proměnné i datového typu int
  a=8;
  int *b;      // definice proměnné p, ukazatele na datový typ int
  b = &a; 

  int c;       // definice proměnné i datového typu int
  c=1;
  int *d;      // definice proměnné p, ukazatele na datový typ int
  d = &c; 

  int g;       // definice proměnné i datového typu int
  g=2;
  int *r;      // definice proměnné p, ukazatele na datový typ int
  r = &g; 

  void *addr=NULL;
  generateInstruction(I_ADD, INT, *b, INT, *d, INT, NULL);
  generateInstruction(I_SUB, INT, 3 , INT, 0, INT, NULL);
  generateInstruction(I_MUL, INT, 10 , INT, 0, INT, NULL);
  generateInstruction(I_PRINT, INT, 0 , INT, 0, INT, NULL);
  generateInstruction(I_STOP, INT, 0 , INT, 0, INT, NULL);
  

  inter(list);
  listFree(list);

  return 0;
  
}


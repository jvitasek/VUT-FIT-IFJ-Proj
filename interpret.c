/**
* Implementace interpretu imperativniho jazyka IFJ15 *
* 
* interpret.c  
* 
* Autor:
*       xvysta02 - Jaroslav Vystavel
*/

#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include "ial.h"
#include "instlist.h"
#include <stdbool.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void * afterIf;
void * else_goto;
int viceFci = FALSE;
void *startLab ;
void *endOfMain;

TError interpret(tInstList *L)
{
  #ifdef DEBUG
  printf("Interpret\n");
  printElementsOfList(*L);
  #endif
  listFirst(L);
  tInstruct *ins;
  tHTItem *op1;
  tHTItem *op2;
  tHTItem *res;

  while(1)
{
  //printf("INSTRUKCE: %d\n",I->instType);


  
   ins = listGetData(L);
   if (ins->instCode == C_Start)
   {
    viceFci = TRUE;
    //printf("TRUTRUETRAJN BJKIBHBHBHUHJINHBOUJBHLGOUV BH JGVLHJN L\n");
   }
   if(ins->instCode == C_Stop)
    break;
   listNext(L);

}

if(viceFci == TRUE)
  {
    listGoTo(L,startLab);
    ins = listGetData(L);
  }
  while(1)
  {
    ins = listGetData(L);
    if(ins == NULL) break;

    switch(ins->instCode)
    {
      case C_Assign:
        op1 = (tHTItem*)ins->op1;
        op2 = (tHTItem*)ins->op2;
        res = (tHTItem*)ins->result;
        

        if(op1->data.varType == T_Doub)
        {
         // printf("double\n");
          res->data.value.d = op1->data.value.d;
          
        }
        else if(op1->data.varType == T_Integ)
        {
         // printf("int\n");
          res->data.value.i = op1->data.value.i;
        }
      break;
          
    case C_Add:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {

        res->data.value.d = op1->data.value.d + op2->data.value.d;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        res->data.value.i = op1->data.value.i + op2->data.value.i;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        res->data.value.d = op1->data.value.i + op2->data.value.d;
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        res->data.value.d = op1->data.value.d + op2->data.value.i;
      }
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      
      break;

    case C_Sub:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        res->data.value.d = op1->data.value.d - op2->data.value.d;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        res->data.value.i = op1->data.value.i - op2->data.value.i;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        res->data.value.d = ((double) op1->data.value.i) - op2->data.value.d;
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        //printf("AHOJ\n");
        res->data.value.d = op1->data.value.d - op2->data.value.i;
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      
      break;

    case C_Less:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        if(op1->data.value.d < op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        if(op1->data.value.i < op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        if(op1->data.value.i < op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        
        if(op1->data.value.d < op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }

      break;
      case C_Greater:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        if(op1->data.value.d > op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        if(op1->data.value.i > op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        if(op1->data.value.i > op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        
        if(op1->data.value.d > op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      break;

      case C_LessEq:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        if(op1->data.value.d <= op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        if(op1->data.value.i <= op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        if(op1->data.value.i <= op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        
        if(op1->data.value.d <= op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }

      break;
      case C_GreaterEq:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        if(op1->data.value.d >= op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        if(op1->data.value.i >= op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        if(op1->data.value.i >= op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        
        if(op1->data.value.d >= op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      break;

      case C_Equal:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        if(op1->data.value.d == op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        if(op1->data.value.i == op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        if(op1->data.value.i == op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        
        if(op1->data.value.d == op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }

      break;
      case C_NotEqual:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

     // printf("%u", op1->data.varType );
       //printf("%u", op2->data.varType );
      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {
         //printf("AAAHOJ\n");
        if(op1->data.value.d != op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        if(op1->data.value.i != op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        //printf("AHOJ\n");
        if(op1->data.value.i != op2->data.value.d)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        
        if(op1->data.value.d != op2->data.value.i)
        res->data.value.i = 1;
        else
          {res->data.value.i = 0;}
      }
      
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      break;

    case C_Cout:
      
      op1 = (tHTItem*)ins->op1;
      
      //printf("tady\n");
      if(op1->data.varType == T_Str)
        {
         

          size_t len = strlen(op1->data.value.str);
          memmove(op1->data.value.str, op1->data.value.str+1, len-2);
          op1->data.value.str[len-2] = 0;
          printf("%s\n",op1->data.value.str);

        }
      else if(op1->data.varType == T_Integ)
        {
          printf("%d\n",op1->data.value.i);
        }
      else if(op1->data.varType == T_Doub)
        {
          printf("%f\n",op1->data.value.d);
        }
      else 
      {
        printf("%d\n",op1->data.value.i );
      }
      break;


    case C_Mul:

      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {

        res->data.value.d = op1->data.value.d * op2->data.value.d;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        res->data.value.i = op1->data.value.i * op2->data.value.i;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        res->data.value.d = op1->data.value.i * op2->data.value.d;
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        res->data.value.d = op1->data.value.d * op2->data.value.i;
      }
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      
      break;

    case C_Div:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

      if((op1->data.varType == T_Doub) && (op2->data.varType == T_Doub))
      {

        res->data.value.d = op1->data.value.d / op2->data.value.d;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Integ))
      {
        res->data.value.i = op1->data.value.i / op2->data.value.i;
      }
      else if((op1->data.varType == T_Integ) && (op2->data.varType == T_Doub))
      {
        res->data.value.d = op1->data.value.i / op2->data.value.d;
      }
      else if((op1->data.varType == T_Doub) && (op2->data.varType == T_Integ))
      {
        res->data.value.d = op1->data.value.d / op2->data.value.i;
      }
      //nekompatibilni typy
      else
      {
        return ESEM_TYP;
      }
      
      break;

   


    case C_IfGoTo:
      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;
      //printf("jednicka : %d\n", op1->data.value.i);
      
      if(op1->data.value.i == 0) 
      {
        //printf("adresa %p",*(op1->data.value.ptrI));
        listGoTo(L, afterIf);
      } 


      break;

    case C_ForGoTo:

      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;

      printf("1: %d\n", op1->data.value.i);
      printf("2: %d\n", op2->data.value.i);
      printf("3: %d\n", res->data.value.i);

      break;
    
    case C_ElseGoTo:

      op1 = (tHTItem*)ins->op1;
      op2 = (tHTItem*)ins->op2;
      res = (tHTItem*)ins->result;
      if(op1->data.value.i != 0)
      {
        listGoTo(L, else_goto );
      }

    break;

    case C_Cin:

      op1 = (tHTItem*)ins->op1;
      int vstup;

        printf("Enter name: ");
        scanf("%d", &op1->data.value.i);
      break;

    default:

      break;
        
    }
    listNext(L);
    if (listGetPointerAct(L) == endOfMain)
      {
       
        return ENOP;}
  //  printf("%s %d\n",res->key,res->data.value.i);
  //  printf("%s %f\n",res->key,res->data.value.d);
  }

  return ENOP;
}

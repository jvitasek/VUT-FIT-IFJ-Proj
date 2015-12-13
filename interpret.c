/**
* Implementace interpretu imperativniho jazyka IFJ15 *
* 
* interpret.c  
* 
* Autor:
*       xvysta02 - Jaroslav Vystavel
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include "ilist.h"
#include <string.h>
#include "stack.h"

extern int c;

//#include "interpret.h"
tListOfInstr *list;
tInst *I;
void *lastI;
union dat_typ_obsah unie;
union dat_typ_obsah unie2;

void *lastI;
void *afterIf;
void *beforeFor;
void *behindFor;
void *startLab;
void *endOfMain;

bool viceFci = FALSE;

tStackP stackI;
tStackP forstack;

int i=0;
int c;
int it_for=0;
int till;
int inter(tListOfInstr *list)
{
  listFirst(list);
  
  tInst *I = malloc(sizeof(tInst));
  I = listGetData(list);
  //printf("%d\n",startLab );
  //listGoto(list, startLab);
while(1)
{
  //printf("INSTRUKCE: %d\n",I->instType);


  i++;
   I = listGetData(list);
   if (I->instType == I_START)
   {
    viceFci = TRUE;
    //printf("TRUTRUETRAJN BJKIBHBHBHUHJINHBOUJBHLGOUV BH JGVLHJN L\n");
   }
   if(I->instType == I_STOP)
    break;
   listNext(list);

}

  
  //tInst *I = malloc(sizeof(tInst));
  if(viceFci == TRUE)
  {
    listGoto(list,startLab);
    I = listGetData(list);
  }
      
  while(1)
  {
    
    
printf("INSTRUKCE: %d\n",I->instType);
    switch (I->instType)
    {
     

      //sčítaní
     

        case I_ADD:
        
        if((I->op1->type == INT) && (I->op2->type == INT))
        {
        I->res->obsah.obsah = (I->op1->obsah.obsah)+(I->op2->obsah.obsah);
        printf("%d\n",(I->res->obsah.obsah) );
        }
        else if((I->op1->type == DOUBLE)|| (I->op2->type == DOUBLE))
        {
          I->res->obsah.obsah_d = (I->op1->obsah.obsah_d)+(I->op2->obsah.obsah_d);
          printf("%f\n",(I->res->obsah.obsah_d) );
        } 
       
        break;

        //odčítání
      case I_SUB:
         //printf("I_SUB\n");
        if((I->op1->type == INT) && (I->op2->type == INT))
        {
        I->res->obsah.obsah = (I->op1->obsah.obsah) - (I->op2->obsah.obsah);
        printf("%d\n",(I->res->obsah.obsah) );
        }
        else if((I->op1->type == DOUBLE)|| (I->op2->type == DOUBLE))
        {
          I->res->obsah.obsah_d = (I->op1->obsah.obsah_d) - (I->op2->obsah.obsah_d);
          printf("%f\n",(I->res->obsah.obsah_d) );
        } 
       break;

       //násobení
       case I_MUL:

        if((I->op1->type == INT) && (I->op2->type == INT))
        {
        I->res->obsah.obsah = (I->op1->obsah.obsah) * (I->op2->obsah.obsah);
        printf("%d\n",(I->res->obsah.obsah) );
        }
        else if((I->op1->type == DOUBLE)|| (I->op2->type == DOUBLE))
        {
          I->res->obsah.obsah_d = (I->op1->obsah.obsah_d)+(I->op2->obsah.obsah_d);
          printf("%f\n",(I->res->obsah.obsah_d) );
        } 

       case I_DIV:

        if((I->op1->type == INT) && (I->op2->type == INT))
        {
          //deleni nulou, asi uz se resi v semantice
          if((I->op2->obsah.obsah) == 0)
            {return 0;}
          I->res->obsah.obsah = (I->op1->obsah.obsah) / (I->op2->obsah.obsah);
          printf("%d\n",(I->res->obsah.obsah) );
        }
        else if((I->op1->type == DOUBLE)|| (I->op2->type == DOUBLE))
        {
          I->res->obsah.obsah_d = (I->op1->obsah.obsah_d) / (I->op2->obsah.obsah_d);
          printf("%f\n",(I->res->obsah.obsah_d) );
        } 

    case I_STOP:
      // instrukce konce programu
      return 0;
      break;

    case I_PRINT:
        printf("IPRINT: ");
        if((I->op1->type == STRING) && (I->op2->type == STRING))
        {
        
        printf("%s\n",(I->op1->obsah.obsah_s) );
        }
        //printf("%d\n", vys);
        break;

       case I_NEG:

        if(I->op1->type == DOUBLE)
         {
            (I->res->obsah.obsah_d = ((-1)*(I->op1->obsah.obsah_d)));
            printf("%f\n",(I->res->obsah.obsah_d ));
         }
        else if (I->op1->type == INT)
        {
          (I->res->obsah.obsah = ((-1)*(I->op1->obsah.obsah)));
           printf("%d\n",(I->res->obsah.obsah ));

        }
         break;


    case I_GEQ:

        if ((I->op1->type == DOUBLE) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah_d)) >= (I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == DOUBLE))
        {
          if (((I->op1->obsah.obsah)) >= (int)(I->op2->obsah.obsah_d))
          {
            I->res->obsah.obsah = 1;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
        }

        else if((I->op1->type == STRING) && (I->op2->type == STRING))
        {

          if (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)>=0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 1;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
          else if  (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)<0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 0; 
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
        }
      else 
        { 
          return 0;
        }

        break;

    case I_LE:
      printf("%s\n","less" );
     if ((I->op1->type == DOUBLE) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah_d)) < (I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
            c=1;
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
            c=0;
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == DOUBLE))
        {
          if (((I->op1->obsah.obsah)) < (int)(I->op2->obsah.obsah_d))
          {
            I->res->obsah.obsah = 1;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
            c=1;
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
            c=0;
          }
        }

         else if ((I->op1->type == INT) && (I->op2->type == INT))
        {
          if (((I->op1->obsah.obsah)) < (int)(I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
            c=1;
            SPushP(&stackI, c);
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
            c=0;
            SPushP(&stackI, c);

          }
        }
        else if((I->op1->type == STRING) && (I->op2->type == STRING))
        {

        
          if (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)<0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 1;
            c=1;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
          else if  (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)>=0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 0; 
            c=0;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
        }
      else 
        { 
          return 0;
        }

        break;

    case I_GE:
       printf("%s\n","great" );
       if ((I->op1->type == DOUBLE) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah_d)) > (I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == DOUBLE))
        {
          if (((I->op1->obsah.obsah)) > (int)(I->op2->obsah.obsah_d))
          {
            I->res->obsah.obsah = 1;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
        }
         else if ((I->op1->type == INT) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah)) > (int)(I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("INT a INTik: %d\n",(I->res->obsah.obsah));
            c=1;
            SPushP(&stackI, c);

          }
          else if ( ((int)(I->op1->obsah.obsah)) < (int)(I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 0;
            printf("INT a INTek: %d\n",(I->res->obsah.obsah));
            c=0;
            SPushP(&stackI, c);

          }
        }
        else if((I->op1->type == STRING) && (I->op2->type == STRING))
        {

          if (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)>0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 1;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
          else if  (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)<=0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 0; 
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
        }
      else 
        { 
          return 0;
        }

        break;

case I_LEQ:
        printf("LEQ");

        if ((I->op1->type == DOUBLE) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah_d)) <= (I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == DOUBLE))
        {
          if (((I->op1->obsah.obsah)) <= (int)(I->op2->obsah.obsah_d))
          {
            I->res->obsah.obsah = 1;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == INT))
        {
          if (((I->op1->obsah.obsah)) <= (int)(I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
          }
        }
        else if((I->op1->type == STRING) && (I->op2->type == STRING))
        {

          if (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)>=0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 1;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
          else if  (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)<0)
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 0; 
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
        }
      else 
        { 
          return 0;
        }

        
        break;

case I_NEQ:
        printf("NEQ");
        if ((I->op1->type == DOUBLE) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah_d)) != (I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == DOUBLE))
        {
          if (((I->op1->obsah.obsah)) != (int)(I->op2->obsah.obsah_d))
          {
            I->res->obsah.obsah = 1;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
        }
        else if ((I->op1->type == INT) && (I->op2->type == INT))
        {
          if (((I->op1->obsah.obsah)) != (int)(I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
          }
        }
        else if((I->op1->type == STRING) && (I->op2->type == STRING))
        {
          printf("%s\n","shi" );
          printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
          if (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)==0)
          {
            
            I->res->obsah.obsah = 0;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
          else if  ((strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)>0) || (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)<0))
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 1; 
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
        }
      else 
        { 
          return 0;
        }

        
        break;
   
   case I_EQ:
        printf("EQ");
        if ((I->op1->type == DOUBLE) && (I->op2->type == INT))
        {
          if (((int)(I->op1->obsah.obsah_d)) == (I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("DOUBLE a INT: %d\n",(I->res->obsah.obsah));
          }
        }

        else if ((I->op1->type == INT) && (I->op2->type == DOUBLE))
        {
          if (((I->op1->obsah.obsah)) == (int)(I->op2->obsah.obsah_d))
          {
            I->res->obsah.obsah = 1;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a DOUBLE: %d\n",(I->res->obsah.obsah));
          }
        }
        else if ((I->op1->type == INT) && (I->op2->type == INT))
        {
          if (((I->op1->obsah.obsah)) == (int)(I->op2->obsah.obsah))
          {
            I->res->obsah.obsah = 1;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
          }
          else
          {
            I->res->obsah.obsah = 0;
            printf("INT a INT: %d\n",(I->res->obsah.obsah));
          }
        }
        else if((I->op1->type == STRING) && (I->op2->type == STRING))
        {
          printf("%s\n","shi" );
          printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
          if (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)==0)
          {
            
            I->res->obsah.obsah = 1;
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
          else if  ((strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)>0) || (strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s)<0))
          {
            printf("%d\n",strcmp((I->op1->obsah.obsah_s),I->op2->obsah.obsah_s) );
            I->res->obsah.obsah = 0; 
            printf("STRING a STRING: %d\n", I->res->obsah.obsah  );
          }
        }
      else 
        { 
          return 0;
        }

      case I_ASSIGN:
        //u stringu mozna \0 ?? TODO
        //if((I->op1->obsah.obsah_s == NULL) ||  (I->op1->obsah.obsah_d == NULL) || (I->op1->obsah.obsah_s == NULL ))
        //nedefinovany obsah
        //{return;}

        if ((I->op1->type == DOUBLE) && (I->op2->type == DOUBLE))
        { 
          I->op2->obsah.obsah_d = I->op1->obsah.obsah_d; 
        }
        else if ((I->op1->type == INT) && (I->op2->type == INT))
        { 
          I->op2->obsah.obsah_d = I->op1->obsah.obsah_d; 
        }
        else if ((I->op1->type == STRING) && (I->op2->type == STRING))
        { 

          strcpy(I->op2->obsah.obsah_s,I->op1->obsah.obsah_s );
        }
        else
          //prirazuji nekompatibilni typy
          {return;}
        break;


      case I_IFGOTO:
      // instrukce pro provedeni podmineneho skoku
        //printf("IFGOOTO");
       // printf("CECKO: %d\n",c );

        //I->op1->obsah.obsah=NULL;
      //c=STopPopP(&stackI);
      //void * adresa = STopPopP (&stackI);
        printf("lastI: %p\n", afterIf);
      
       if(c==0) {listGoto(list, afterIf);}

      break;

      case I_GOTO:
      printf("I_GOTO");
      // instrukce pro provedeni podmineneho skoku
        
      break;

      case I_FOR_GOTO:
        //printf("IFGOTO\n");
        //printf("  CECKO: %d\n",c );

        //I->op1->obsah.obsah=NULL;
      
        //printf("  lastI ve foru: %p\n", (adresa));
        printf("ADRESA ADRESA: --------> %d\n", beforeFor);
       if(it_for<5) {printf("kolikrat jeste: %d\n",it_for ); listGoto(list, beforeFor);it_for++;}
       //listGoto(list, behindFor);
      break;

      case I_SET_FOR:
          printf("I_SET_FOR\n");
          //it_for = (I->op1->obsah.obsah);
          till = (I->op2->obsah.obsah);
        break;

      case I_FOR_IFGOTO:
        printf("I_FOR_IFGOTO\n");
        //listGoto(list,behindFor);
        break;

      case I_START:

        break;
      default:

        return 0;
      
      break;

    }
   
    listNext(list);
    I = listGetData(list);
    //printf("%d\n",endOfMain );
    if (listGetPointerAct(list) == endOfMain)
      {
       
        return;}

    
  }

  return 0;
  
}*/


// int pracuj()
// {
//    list = malloc(sizeof(tListOfInstr));
//    listInit(list);

//   //(8+1)-3

//   int a;       // definice proměnné i datového typu int
//   a=1;
//   int *b;      // definice proměnné p, ukazatele na datový typ int
//   b = &a; 

//   int c;       // definice proměnné i datového typu int
//   c=1;
//   int *d;      // definice proměnné p, ukazatele na datový typ int
//   d = &c; 

//   int g;       // definice proměnné i datového typu int
//   g=2;
//   int *r;      // definice proměnné p, ukazatele na datový typ int
//   r = &g; 
//   union dat_typ_obsah unie;
//   strcpy(unie.obsah_s, "achoj");

//   union dat_typ_obsah unie2;
//   //unie2.obsah_s="achoj";

//   void *addr=NULL;
//   //generateInstruction(I_ADD, INT, &unie, INT, &unie2, INT, NULL);
//   //generateInstruction(I_ADD, DOUBLE, &unie, DOUBLE,&unie2, DOUBLE, NULL);
//   generateInstruction(I_PRINT, STRING, &unie, STRING, &unie2,  DOUBLE, NULL);
  

//   inter(list);
//   listFree(list);

//   return 0;
  
// }


#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include "ial.h"
#include "instlist.h"
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

void * afterIf;
void * else_goto;
int viceFci = FALSE;
void *startLab ;
void *endOfMain;

TError interpret(tInstList *L)
{
  printf("Interpret\n");
  printElementsOfList(*L);
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
      printf("jednicka : %d\n", op1->data.value.i);
      
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





















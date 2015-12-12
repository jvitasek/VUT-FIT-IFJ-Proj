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

//#define DEBUG 1

TError interpret(tInstList *L)
{
  #ifdef DEBUG
	fprintf(stderr, "Interpret\n");
  #endif
	printElementsOfList(*L);
	listFirst(L);
	tInstruct *ins;
	tHTItem *op1;
	tHTItem *op2;
	tHTItem *res;
	while(1)
	{
		ins = listGetData(L);
		if(ins == NULL) break;
		op1 = (tHTItem*)ins->op1;
		op2 = (tHTItem*)ins->op2;
		res = (tHTItem*)ins->result;
		res->data.value.d = op1->data.value.d;
		
		listNext(L);
	}
  #ifdef DEBUG
	fprintf(stderr, "%s %f\n",res->key,res->data.value.d);
  #endif
	return ENOP;
}


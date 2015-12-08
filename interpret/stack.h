
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>

#define FALSE 0
#define TRUE 1
#define MAXSTACK 30                      /* maximální počet prvků v zásobníku */


/* pomocné zásobníky */
typedef struct	{                          /* zásobník hodnot typu tBTNodePtr */
    int a[MAXSTACK];
    int top;
} tStackP;

typedef struct {                                 /* zásobník hodnot typu bool */
    bool a[MAXSTACK];
    int top;
} tStackB;

void SInitP (tStackP *S);
void SPushP (tStackP *S, int);
int STopPopP (tStackP *S);
int STopP (tStackP *S);
void SPopP (tStackP *S);
bool SEmptyP (tStackP *S);
/***prototypes***/

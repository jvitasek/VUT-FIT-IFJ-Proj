/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * gc.c  -  Garbage collector
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */


#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

int itemCounter = 0;

void putInGC(void ***ptrArray, void * ptr)
{
	void **pom = realloc(*ptrArray,(itemCounter+1)*(sizeof(void*)));
	if(pom == NULL)
	{
		printf("errooor\n");
		exit(99);
	}
	*ptrArray = pom;
	(*ptrArray)[itemCounter] = ptr;
	itemCounter++;
}

void freeGC(void ***ptrArray)
{
	for(int i = 0; i < itemCounter; i++)
	{
		free((*ptrArray)[i]);
	}
	free(*ptrArray);
	
}
	


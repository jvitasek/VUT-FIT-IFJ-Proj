/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * str.h  -  Hlavickovy subor pre pracu s retazcom
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#ifndef STR_H
#define STR_H

#include <stdio.h>

typedef struct
{
	char *str;		// retazec ukonceny '\0'
	int length;		// dlzka retazca
	int allocSize;	// velkost alokovanej pamete
} string;


int strInit(string *s);
int strAppend(string *s, char c);
void strFree(string *s);
void strClear(string *s);
char *strGetStr(string *s);
int strGetLength(string *s);
string toString(char *s);

#endif

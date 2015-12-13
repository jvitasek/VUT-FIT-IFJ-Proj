/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * str.c  -  Funkcie pre pracu s retazcom inspriovane Jednoduchym Interpretom
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#include <stdlib.h>
#include <string.h>
#include "ial.h"

#define STR_LEN 8

/**
 * [strInit description]
 * @param  s [description]
 * @return   [description]
 */
int strInit(string *s)
{
	if((s->str = (char*)malloc(STR_LEN)) == NULL)		// alokacia pamete
	{
		fprintf(stderr,"Chyba pri alokovani pamete.\n");	// chyba pri alokacii
		return 99;
	}
	s->str[0] = '\0';	// nastavenie null charakteru
	s->length = 0;
	s->allocSize = 8;
	return 0;
}

/**
 * [strAppend description]
 * @param  s [description]
 * @param  c [description]
 * @return   [description]
 */
int strAppend(string *s, char c)
{
	if(s->length + 1 >= s->allocSize)	// kontrola ci nam staci uz alokovana pamet
	{
		if((s->str = (char*) realloc(s->str, s->length + STR_LEN)) == NULL)	// ak nestacila tak realokujeme
		{
			fprintf(stderr,"Chyba pri realokovani pamete.\n");
			return 99;
		}
		s->allocSize = s->length + STR_LEN;
	}
	s->str[s->length] = c;		// prepisem posledny znak retazca (co bol vlastne null charakter) pridavanym znakom
	s->length++;				// inkrementujeme dlzku retazca
	s->str[s->length] = '\0';	// nastavenie null charakteru
	return 0;
}

/**
 * [strFree description]
 * @param s [description]
 */
void strFree(string *s)
{
	free(s->str);
}

/**
 * [toString description]
 * @param  s [description]
 * @return   [description]
 */
string toString(char *s)
{
	string result;
	strInit(&result);
	int index = 0;
	while(s[index] != '\0')
	{
		strAppend(&result, s[index]);
		++index;
	}
	return result;
}

/**
 * [strClear description]
 * @param s [description]
 */
void strClear(string *s)
{
	s->str[0] = '\0';
	s->length = 0;
}

/**
 * [strGetStr description]
 * @param  s [description]
 * @return   [description]
 */
char *strGetStr(string *s)
{
	return s->str;
}

/**
 * [strGetLength description]
 * @param  s [description]
 * @return   [description]
 */
int strGetLength(string *s)
{
	return s->length;
}

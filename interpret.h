/**
* Implementace interpretu imperativniho jazyka IFJ15 *
* 
* interpret.c  
* 
* Autor:
*       xvidaj00 - Juraj Vida
*/

#ifndef INTERPRET_H
#define INTERPRET_H

#include "instlist.h"
#include "error.h"

TError interpret(tInstList *L);

#endif
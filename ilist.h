/**
* -----------------------------------------------------------------------------
*
* Implementace interpretu imperativniho jazyka IFJ15
*
* Soubor       : ilist.h
* Popis        : Seznam instrukci
* Datum        : Listopad 2015
* Autori       : xvysta02 - Jaroslav Vystavel
*              
* -----------------------------------------------------------------------------
**/

#ifndef ILIST_H_
#define ILIST_H_

/**
 * Typy instrukci
 */
typedef enum tInstCode {
 	// aritmeticke operace 
	I_ADD=0, // (dest, addr, addr) + 
	I_SUB=1, // (dest, addr, addr) -  
	I_MUL=2, // (dest, addr, addr) *
	I_DIV=3, // (dest, addr, addr) /


	// relacni operace
	I_EQ=4, // (dest, addr, addr) ==
	I_NEQ=5, // (dest, addr, addr) !=
	I_LE=6, // (dest, addr, addr) <
	I_LEQ=7, // (dest, addr, addr) <=
	I_GE=8, // (dest, addr, addr) >
	I_GEQ=9, // (dest, addr, addr) >=
	I_NEG=10, // (dest, addr, - ) 

	I_LABEL=11,

	// prirazeni
	I_ASSIGN=12, // (dest, addr, - )

	// instrukce skoku
	I_GOTO=13, // (adr, - , - ) skoci na adresu adr 
	I_IFGOTO=14, // (adr, cond, - ) skoci na adresu adr, pokud plati podminka cond
	I_FOR_GOTO=15,
	I_SET_FOR=16,
	I_FOR_IFGOTO=17,
	// zasobnik parametru
	I_PUSH=18, // pushne na zasobnik
	I_POP=19, // odstrani a vrati hodnotu z vrcholu zasobniku
	I_NOP=20,  // prazdna operace

	I_STOP=21, //konec programu
	I_PRINT=22,
	// return
	I_RETURN=23, // (-, -, -)

	// preda rizeni volane funkci
	I_CALL=24, // (id_funkce, addr, - )
	I_MAIN_END=25,
	I_START,
	// vestavene funkce
	I_LENGTH,
	I_SUBSTR,
	I_CONCAT,
	I_FIND,
	I_SORT 
} tInstCode;


/**
 * Datovy typ
 */

typedef enum TypeI{
	INT=0, 	
	DOUBLE, 
	STRING
} TypeI;

/**
 * obsah
 */

 union dat_typ_obsah{
 	int obsah;
 	double obsah_d;
 	char obsah_s[255];
 }dat_typ_obsah;
/**
 * Struktura operandu
 */

typedef struct iOperand
{
	TypeI type; // typ operandu
	union dat_typ_obsah obsah; // obsah promenne/operandu
} iOperand;


/**
 * Struktura reprezentujici 3AC instrukci
 */

typedef struct tInst 
{
	tInstCode instType; // typ instrukce
	iOperand *op1; // operand 1
	iOperand *op2; // operand 2
	iOperand *res; // result
} tInst;

/**
 * Polozka seznamu, ktera obsahuje konkretni instrukci
 */

typedef struct ListItem 
{
	tInst Instruction;
	struct ListItem *NextItem;
} tListItem;

/**
 * Struktura reprezentujici seznam instrukci, aktivni prvek predstavuje aktualni instrukci
 */

typedef struct ListOfInstr 
{
	struct ListItem *First;
	struct ListItem *Last;
	struct ListItem *Act;
} tListOfInstr;

void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
int listInsertLast(tListOfInstr *L, tInst I);
void listFirst(tListOfInstr *L);
void listNext(tListOfInstr *L);
void listGoto(tListOfInstr *L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr *L);
void *listGetPointerAct(tListOfInstr *L);
tInst *listGetData(tListOfInstr *L);

#endif

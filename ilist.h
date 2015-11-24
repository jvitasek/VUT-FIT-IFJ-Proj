/**
* -----------------------------------------------------------------------------
*
* Implementace interpretu imperativniho jazyka IFJ15
*
* Soubor       : ilist.h
* Popis        : Seznam instrukci
* Datum        : Listopad 2015
* Autori       : xvyrou05 - Marek Vyroubal
*              
* -----------------------------------------------------------------------------
**/

#ifndef ILIST_H_
#define ILIST_H_

/*
 * Typy instrukci
 */
typedef enum {
 	/* Aritmeticke operace */
	I_ADD, // (dest, addr, addr) +
	I_SUB, // (dest, addr, addr) -
	I_MUL, // (dest, addr, addr) *
	I_DIV, // (dest, addr, addr) /

	/* relacni operace*/
	I_EQ, // (dest, addr, addr) ==
	I_NEQ, // (dest, addr, addr) !=
	I_LE, // (dest, addr, addr) <
	I_LEQ, // (dest, addr, addr) <=
	I_GE, // (dest, addr, addr) >
	I_GEQ, // (dest, addr, addr) >=
	I_NEG, // (dest, addr, - ) 

	/* prirazeni */
	I_ASSIGN, // (dest, addr, - )

	/* instrukce skoku */
	I_GOTO, // (adr, - , - ) skoci na adresu adr 
	I_IFGOTO, // (adr, cond, - ) skoci na adresu adr, pokud plati podminka cond

	/* zasobnik parametru*/
	I_PUSH, // pushne na zasobnik
	I_POP, // odstrani a vrati hodnotu z vrcholu zasobniku
	I_NOP,  // prazdna operace

	I_STOP, //konec programu

	/*
	* Pokud interpret narazi na instrukci I_FSKIP, preskoci vsechny instrukce
	* az po instrukci I_FSTOP
	*/
	I_FSKIP, // (-,-,-) => nema operandy
	I_FSTOP, // zarazka pro instrukci I_FSKIP: (-,-,-)

	/*
	* Return
	*/
	I_RETURN, // (-, -, -)

	/*
	* Preda rizeni volane funkci
	*/
	I_CALL, // (id_funkce, adresa_kam_se_vynori, - )

	/* Navesti */
	I_LABEL, // navesti urcene pro skoky (priznak, -, -)

	/*
	* Instrunkce pro vestavene funkce
	*/
	I_LENGTH;
	I_SUBSTR;
	I_CONCAT;
	I_FIND;
	I_SORT; 
} tInstcruction;

/*
 * Struktura reprezentujici 3AC instrukci
 */
typedef struct Inst 
{
	int InstType; // typ instrukce
	void *addr1; // adresa 1
	void *addr2; // adresa 2
	void *addr3; // adresa 3
} tInst;

/*
 * Polozka seznamu, ktera obsahuje konkretni instrukci
 */
typedef struct ListItem 
{
	tInst Instruction;
	struct ListItem *NextItem;

} tListItem;

/*
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
tInst *listGetData(tListOfInstr *L);

#endif

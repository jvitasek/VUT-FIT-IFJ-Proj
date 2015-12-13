/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * scanner.h  -  Hlavickovy subor pre scanner
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "str.h"

// Stavy pre konecny automat
typedef enum
{
	S_Start,
	S_Id,		// identifikator/kluc.slovo
	S_Integ,	// cele cislo
	S_Des1,		// pomocny stav pri spracovavani desatinnych cisel
	S_Des2,		// pomocny stav pri spracovavani desatinnych cisel
	S_Expo1,	// pomocny stav pri spracovavani cisel s expo.castou
	S_Expo2,	// pomocny stav pri spracovavani cisel s expo.castou
	S_Doub,		// desatinne cislo
	S_Str,		// retazec
	S_Slash,		// riadkovy/blokovy koment alebo delenie
	S_ComLine,	// riadkovy koment
	S_ComBlock,	// blokovy koment
	S_ComEnd,	// kontrola konca blokoveho komentu
	S_LeftBeak,	// (lavy zobak) < alebo <= alebo <<
	S_RightBeak,// (pravy zobak) > alebo >= alebo >>
	S_EqOrAss,	// rovnost == alebo priradenie =
	S_NotEqual,	// nerovnost !=
	S_EscSeq,	// escape sekvence
} S_State;

// Typy tokenov
typedef enum
{
	// Klucove slova
	T_Id,	//0
	T_Auto,	//1
	T_Cin,	//2
	T_Cout,	//3
	T_Double,	//4
	T_Else,	//5
	T_For,	//6
	T_If,	//7
	T_Int,	//8
	T_Return,	//9
	T_String,	//10
	T_Docas,	///////////11
	T_Integ,	// cele cisla
	T_Doub, 	// desatinne cisla
	T_Error,	// 14 znamena, ze bola chyba v lexikalnej casti
	T_Str,	// retazec 15
	T_Plus,	// + 16
	T_Min,	// - 17
	T_Mul,	// * 18
	T_Div,	// / 19
	T_LessThan,	// < 20
	T_LessEqual, // <= 21
	T_LeftShift, // << 22
	T_GreaterThan, // > 23
	T_GreaterEqual, // >= 24
	T_RightShift, // >> 25
	T_Equal,		// == 26
	T_Assig,	// = 27
	T_NotEqual, // != 28
	T_Comma, 	// ,	29
	T_Semicolon,	// ; 30
	T_LeftParenthesis,	// ( 31
	T_RightParenthesis,	// ) 32
	T_LeftBrace,		// { 33
	T_RightBrace,		// } 34
	T_EOF,				// EOF 35
} T_Type;

typedef struct
{
	T_Type type;
	int line;
} T_Token;

T_Token getToken(FILE *input, string *attr, int *line);

#endif

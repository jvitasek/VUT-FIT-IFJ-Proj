/**
 * Implementace interpretu imperativniho jazyka IFJ15 *
 * 
 * scanner.c  -  Lexikalny analyzator(Scanner)
 * 
 * Autori:
 * 			xvidaj00 - Juraj Vida
 */

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "error.h"

/*
 * Funkcia, ktora nam kontroluje ci bola zadana platna escape sekvencia.
 * V pripade, ze ano, vracia jej integer hodnotu.
 * V pripade, ze nie, vracia hodnotu -1
 */
int escapeSeq(FILE *myInput)
{
	string pom;				// pomocny string
	strInit(&pom);			// inicializujem ho
	strAppend(&pom,'0');	// escape sekvenci si ulozim v tvare 0xhex.cislo aby sa dalo lahko prekonvertovat do int
	strAppend(&pom,'x');
	char a = fgetc(myInput);		// nacitam prvy znak hexa cisla
	if((isdigit(a)) || ((a >= 'a') && ( a <= 'f')) || ((a >= 'A') && ( a <= 'F')))	// kontrola ci je v danom rozsahu prva cifra hexa cisla
	{
		strAppend(&pom,a);	// pridam na koniec pom retazca
	}else return -1;	// ak nie je, tak ide o neplatnu escape sekvenci => Error
	a = fgetc(myInput);		// nacitam druhy znak hexa cisla
	if((isdigit(a)) || ((a >= 'a') && ( a <= 'f')) || ((a >= 'A') && ( a <= 'F')))	// kontrola ci je v danom rozsahu druha cifra hexa cisla
	{
		strAppend(&pom,a);	// pridam na koniec pom retazca
	}else return -1;	// ak nie je, tak ide o neplatnu escape sekvenci => Error
	int es = (int)strtol(pom.str, NULL, 0);		// prekonvertujem hexa cislo na int
	strFree(&pom);	// uvolnim pomocny string
	if(es == 0)
	{
		return -1;	// \x00 je neplatna... az od \01
	}else return es;	// int hodnota zadanej esc.sekvencie
}

/*
 * Funkcia, ktora rozsekava vstupny subor na jednotlive lexemy.
 * Konecny stavovy automat.
 */
T_Token getToken(FILE *input, string *attr, int *line)
{
	char c;						// premenna na nacitavanie znaku
	T_Token token;				// vytvorim si token
	token.type = T_Error;
	strClear(attr);				// vymazem obsah stringu
	S_State state = S_Start;	// nastavim si pociatocny stav
	int go = 1;					// pomocna premenna pre nacitavanie znakov (kde 1 znamena TRUE, 0 znamena FALSE)
	while(go)					// konecny stavovy automat na nacitavanie znakov zo suboru
	{
		if((c = fgetc(input)) == EOF)	// nacitanie prveho znaku
		{
			go = 0;		// koniec subora => ukoncenie cyklu
			if(state != S_Start)// kontrolujem ci sme neni v nejakom stave => stav nebol ukonceny
			{
				//token.type = T_Error;
				//return token;
				print_error(ELEX, token.line);
				exit(ELEX);
			}else
			{
				token.type = T_EOF;		// koniec subora
				return token;
			}
		}else 
		{
			switch(state)	// stavy automatu
			{
				case S_Start:
					token.line = *line;
					if((isalpha(c)) || (c == '_'))	// identifikator alebo klucove slovo
					{
						strAppend(attr,c);
						state = S_Id;
					}else if(isdigit(c))	// cislo
					{
						strAppend(attr,c);
						state = S_Integ;
					}else if(c == '"')		// retazec
					{
						strAppend(attr,c);
						state = S_Str;
					}else if(c == '/')		// riadkovy/blokovy komentar alebo delenie (tie prekladac maze, cize nebudem ukladat nacitane znaky do stringu)
					{
						state = S_Slash;
					}else if((c == ' ') || (c == '\t') || (c == '\v') || (c == '\f') || (c == '\r'))	// vynechavanie medzier
					{
						state = S_Start;
					}else if(c == '\n')
					{
						(*line)++;
						state = S_Start;
					}else if(c == '+')		// +
					{
						token.type =  T_Plus;
						return token;
					}else if(c == '-')		// -
					{
						token.type =  T_Min;
						return token;
					}else if(c == '*')		// *
					{
						token.type =  T_Mul;
						return token;
					}else if(c == '<')		// < alebo <= alebo <<
					{
						state = S_LeftBeak;	// nastavim stav na lavy zobak
					}else if(c == '>')		// > alebo >= alebo >>
					{
						state = S_RightBeak;	// nastavim stav na pravy zobak
					}else if(c == '=')		// priradenie = alebo rovnost ==
					{
						state = S_EqOrAss;
					}else if(c == '!')		// not equal !=
					{
						state = S_NotEqual;
					}else if(c == ',')		// ,
					{
						token.type =  T_Comma;
						return token;
					}else if(c == ';')		// ;
					{
						token.type =  T_Semicolon;
						return token;
					}else if(c =='(')		// (
					{
						token.type =  T_LeftParenthesis;
						return token;
					}else if(c ==')')		// )
					{
						token.type =  T_RightParenthesis;
						return token;
					}else if(c =='{')		// {
					{
						token.type =  T_LeftBrace;
						return token;
					}else if(c =='}')		// }
					{
						token.type =  T_RightBrace;
						return token;
					}else
					{
						//token.type =  T_Error;	// inak bol zly lexem a vraciam error
						//return token;
						print_error(ELEX, token.line);
						exit(ELEX);
					}
					break;
				case S_Id:
					if((isalpha(c)) || (isdigit(c)) || (c == '_'))	// identifikator alebo klucove slovo
					{
						strAppend(attr,c);
						state = S_Id;
					}else 
					{
						ungetc(c,input); 	// vratim posledny nacitany znak, ktory uz do id/kluc. slova nepatri
						
						if(strcmp(attr->str,"auto") == 0)	// kontrola zhody s jednotlivymi klucovymi slovami
						{
							token.type =  T_Auto;	// nastavenie typu tokenu
							return token;
							
						}else if(strcmp(attr->str,"cin") == 0)
						{
							token.type =  T_Cin;
							return token;
						}else if(strcmp(attr->str,"cout") == 0)
						{
							token.type =  T_Cout;
							return token;
						}else if(strcmp(attr->str,"double") == 0)
						{
							token.type =  T_Double;
							return token;
						}else if(strcmp(attr->str,"else") == 0)
						{
							token.type =  T_Else;
							return token;
						}else if(strcmp(attr->str,"for") == 0)
						{
							token.type =  T_For;
							return token;
						}else if(strcmp(attr->str,"if") == 0)
						{
							token.type =  T_If;
							return token;
						}else if(strcmp(attr->str,"int") == 0)
						{
							token.type =  T_Int;
							return token;
						}else if(strcmp(attr->str,"return") == 0)
						{
							token.type =  T_Return;
							return token;
						}else if(strcmp(attr->str,"string") == 0)
						{
							token.type =  T_String;
							return token;
						}else
						{
							token.type =  T_Id;
							return token;
						}
					}
					break;
				case S_Integ:					// cele cisla
					if(isdigit(c))
					{
						strAppend(attr,c);
						state = S_Integ;
					}else if(c == '.')		// znamena ze ide o desatinne cislo, preto musim zmenit stav
					{
						strAppend(attr,c);
						state = S_Des1;
					}else if((c == 'e') || (c == 'E'))	// ide o cele cislo s expo. castou
					{
						strAppend(attr,c);
						state = S_Expo1;
					}else
					{
						ungetc(c,input); 	// vratim posledny nacitany znak, ktory uz do cisla nepatri
						token.type =  T_Integ;
						return token;
					}
					break;
				case S_Des1:		// kontrola ci po '.' nasleduje cislo
					if(isdigit(c))	// ak ano tak pokracuje na dalsi stav
					{
						strAppend(attr,c);
						state = S_Des2;
					}else
					{
						//token.type =  T_Error;	// inak vracia error, lebo tam bolo nieco, co tam byt nema
						//return token;
						print_error(ELEX, token.line);
						exit(ELEX);
					}
					break;
				case S_Des2:		// kontrola ci je obyc. desat.cislo alebo aj s exponentom
					if(isdigit(c))
					{
						strAppend(attr,c);
						state = S_Des2;
					}else if((c == 'e') || (c == 'E'))	// ide o desatinne cislo s expo. castou
					{
						strAppend(attr,c);
						state = S_Expo1;		// nastavim dalsi stav, ktory skontroluje ci je za e/E nepovinne +/-
					}else// if((isdigit(c)) || (c == ';'))
					{
						ungetc(c,input);	// vratim posledny nacitany znak
						token.type =  T_Doub;
						return token;
					}/*else
					{
						token.type =  T_Error;	// inak vracia error, lebo tam bolo nieco, co tam byt nema
						return token;
					}*/
					break;
				case S_Expo1:	// kontroluje spravny zapis expo. casti
					if((c == '+') || (c == '-'))	// ide o expo. cislo aj s +/-
					{
						strAppend(attr,c);
						state = S_Expo2;		// nastavim dalsi stav, ktoru prekontroluje ci su za e/E cisla
					}else if(isdigit(c))		// za e/E je hned cislo
					{
						strAppend(attr,c);
						state = S_Doub;			// preto nastavim stav, ktory uz len docita cisla po koniec
					}else
					{
						//token.type =  T_Error;	// inak vracia error, lebo tam bolo nieco, co tam byt nema
						//return token;
						print_error(ELEX, token.line);
						exit(ELEX);
					}
					break;
				case S_Expo2: 	// kontroluje ci za +/- je cislo
					if(isdigit(c))	// je tam cislo
					{
						strAppend(attr,c);
						state = S_Doub;			// preto nastavim stav, ktory uz len docita cisla po koniec
					}else
					{
						//token.type =  T_Error;	// inak vracia error, lebo tam bolo nieco, co tam byt nema
						//return token;
						print_error(ELEX, token.line);
						exit(ELEX);
					}
					break;
				case S_Doub:
					if(isdigit(c))
					{
						strAppend(attr,c);
						state = S_Doub;
					}else 
					{
						ungetc(c,input);	// vratim posledny nacitany znak
						token.type =  T_Doub;
						return token;
					}
					break;
				case S_Str:
					if(c == '"')	// koniec retazca
					{
						strAppend(attr,c);
						token.type =  T_Str;
						return token;
					}else if(c == '\\')		// ide o escape sekvenci
					{
						state = S_EscSeq;	
					}else 	// inak pridavam znaky do stringu
					{
						strAppend(attr,c);
						state = S_Str;
					}
					break;
				case S_Slash:	// komentare alebo delenie
					if(c == '/')	// ide o riadkovy
					{
						state = S_ComLine;
					}else if(c == '*')	// ide o blokovy
					{
						state = S_ComBlock;
					}else  		// inak je to len podiel
					{
						ungetc(c,input); 	// vratim posledny nacitany znak,
						token.type =  T_Div;
						return token;
					}
					break;
				case S_ComLine:	// riadkovy komentar
					if(c == '\n')
					{
						(*line)++;
						state = S_Start;
					}else state = S_ComLine;
					break;
				case S_ComBlock:		// blokovy komentar
					if(c == '\n')
					{
						(*line)++;
						state = S_ComBlock;
					}else if(c == '*')	// nastavim stav, kde prebehne kontrola ci je uz koniec komentara
					{
						state = S_ComEnd;
					}else state = S_ComBlock;
					break;
				case S_ComEnd:	// kontrola ci uz je koniec komentu
					if(c == '/')	// komentar bol ukonceny
					{
						state = S_Start;
					}else state = S_ComBlock;	// nebol este ukonceny a preto sa vrati zase na stav prechadzania komentu
					break;
				case S_LeftBeak:	// kontrola o ktoru moznost ide (< <= <<)
					if(c == '=')
					{
						token.type =  T_LessEqual;	// <=
						return token;
					}else if(c == '<')
					{
						token.type =  T_LeftShift;	// <<
						return token;
					}else
					{
						ungetc(c,input);
						token.type =  T_LessThan;	// <
						return token;
					}
					break;
				case S_RightBeak:	// kontrola o ktoru moznost ide (> >= >>)
					if(c == '=')
					{
						token.type =  T_GreaterEqual;	// >=
						return token;
					}else if(c == '>')
					{
						token.type =  T_RightShift;	// >>
						return token;
					}else
					{
						ungetc(c,input);
						token.type =  T_GreaterThan;	// >
						return token;
					}
					break;
				case S_EqOrAss:			// kontrola ci ide o priradenie alebo rovnost
					if(c == '=')
					{
						token.type =  T_Equal;			// ==
						return token;
					}else
					{
						ungetc(c,input);
						token.type =  T_Assig;			// =
						return token;
					}
					break;
				case S_NotEqual:	// kontrola ci ide o !=
					if(c == '=')
					{
						token.type =  T_NotEqual;	// ide o !=
						return token;
					}else
					{
						//token.type =  T_Error;		// inak ide o chybu
						//return token;
						print_error(ELEX, token.line);
						exit(ELEX);
					}
					break;
				case S_EscSeq:		// escape sekvencia
					if(c == '"')	// ide o '\"'
					{
						strAppend(attr,'\"');	// az tu pridam aj spolu s '\' 
						state = S_Str;			// vratim sa do stavu nacitavania retazca
					}else if(c == 'n')	// ide o '\n'
					{
						strAppend(attr,'\n');	// az tu pridam aj spolu s '\' 
						state = S_Str;			// vratim sa do stavu nacitavania retazca
					}else if(c == '\\')	// ide o '\\'
					{
						strAppend(attr,'\\');	// az tu pridam aj spolu s '\' 
						state = S_Str;			// vratim sa do stavu nacitavania retazca
					}else if(c == 't')	// ide o '\t'
					{
						strAppend(attr,'\t');	// az tu pridam aj spolu s '\' 
						state = S_Str;			// vratim sa do stavu nacitavania retazca
					}else if(c == 'x')	// ide o obecnu escape sekvenci
					{
						
						int esc = escapeSeq(input);		// volam funkciu, ktora mi vypocita int hodnotu danej sekvencie
						if(esc == -1)
						{
							//token.type =  T_Error;	// bola zadana neplatna escape sekvence
							//return token;
							print_error(ELEX, token.line);
							exit(ELEX);
						}else
						{
							strAppend(attr,esc);	// pridam uz vysledok escape sekvence do retazca
							state = S_Str;	// dalej nacitavam retazec
						}
					}else
					{
						//token.type =  T_Error;	// inak error
						//return token;
						print_error(ELEX, token.line);
						exit(ELEX);
					}
					break;
				default:
					break;
			}
		}
	}
	return token;
}

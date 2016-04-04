/* front.c - a lexical analyzer system for simple
   arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
int i;//for iterator
FILE *in_fp, *fopen();
/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define RESERVED 12
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMICOLON 27
#define EQUAL 28
#define COMMA 29
#define FOR_CODE 30
#define IF_CODE 31
#define ELSE_CODE 32
#define WHILE_CODE 33
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37
#define BIG_LEFT_PAREN 41
#define BIG_RIGHT_PAREN 42
#define RETURN 39
#define DOT 43
#define SHOP 44
#define LEFT_BRACKET 45
#define RIGHT_BRACKET 46
#define INCLUDE 38
#define MAIN 40
char *reserved[11]={"for","if","else","while","do","int","float","switch","include","return","main"};
/******************************************************/
/* main driver */
main() {
		/* Open the input data file and process its contents */
		if ((in_fp = fopen("a.c", "r")) == NULL)
				printf("ERROR - cannot open front.in \n");
		else {
				getChar();
				do {
						lex();
				} while (nextToken != EOF);
		}
}
/*****************************************************/
/* lookup - a function to lookup operators and parentheses
   and return the token */
int lookup(char ch) {
		switch (ch) {
				case '{':
						addChar();
						nextToken = BIG_LEFT_PAREN;
						break;
				case '}':
						addChar();
						nextToken = BIG_RIGHT_PAREN;
						break;
				case '(':
						addChar();
						nextToken = LEFT_PAREN;
						break;
				case ')':
						addChar();
						nextToken = RIGHT_PAREN;
						break;
				case '+':
						addChar();
						nextToken = ADD_OP;
						break;
				case '-':
						addChar();
						nextToken = SUB_OP;
						break;
				case '*':
						addChar();
						nextToken = MULT_OP;
						break;
				case '/':
						addChar();
						nextToken = DIV_OP;
						break;
				case ';':
						addChar();
						nextToken = SEMICOLON;
						break;
				case '=':
						addChar();
						nextToken = EQUAL;
						break;
				case ',':
						addChar();
						nextToken = COMMA;
						break;
				case '.':
						addChar();
						nextToken = DOT;
						break;
				case '#':
						addChar();
						nextToken = SHOP;
						break;
				case '<':
						addChar();
						nextToken = LEFT_BRACKET;
						break;
				case '>':
						addChar();
						nextToken = RIGHT_BRACKET;
						break;
				default:
						addChar();
						nextToken = EOF;
						break;
		}
		return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
		if (lexLen <= 98) {
				lexeme[lexLen++] = nextChar;
				lexeme[lexLen] = 0;
		}
		else
				printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
   input and determine its character class */
void getChar() {
		if ((nextChar = getc(in_fp)) != EOF) {
				if (isalpha(nextChar))
						charClass = LETTER;
				else if (isdigit(nextChar))
						charClass = DIGIT;
				else charClass = UNKNOWN;
		}
		else
				charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it
   returns a non-whitespace character */
void getNonBlank() {
		while (isspace(nextChar))
				getChar();
}
int lex() {
		lexLen = 0;
		getNonBlank();
		switch (charClass) {
				/* Parse identifiers */
				case LETTER:
						addChar();
						getChar();
						while (charClass == LETTER || charClass == DIGIT) {
								addChar();
								getChar();
						}
						for(i=0;i<11;++i){
							if(strcmp(reserved[i],lexeme)==0){
								nextToken=30+i;
								break;
							}
							else{
								nextToken=IDENT;
							}
						}
						break;
						/* Parse integer literals */
				case DIGIT:
						addChar();
						getChar();
						while (charClass == DIGIT) {
								addChar();
								getChar();
						}
						nextToken = INT_LIT;
						break;
						/* Parentheses and operators */
				case UNKNOWN:
						lookup(nextChar);
						getChar();
						break;
						/* EOF */
				case EOF:
						nextToken = EOF;
						lexeme[0] = 'E';
						lexeme[1] = 'O';
						lexeme[2] = 'F';
						lexeme[3] = 0;
						break;
		} /* End of switch */
		printf("Next token is: %d, Next lexeme is %s\n",
						nextToken, lexeme);
		return nextToken;
} /* End of function lex */

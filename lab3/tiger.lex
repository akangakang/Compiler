%{
/* Lab2 Attention: You are only allowed to add code in this file and start at Line 26.*/
#include <string.h>
#include "util.h"

#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
// #include "tokens.h"
#include "y.tab.h"    // last
int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}

void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

/*
* Please don't modify the lines above.
* You can add C declarations of your own below.
*/
#define MAXSTR 2048
int comments = 0;
char res[MAXSTR];
int len = 0;

/* @function: getstr
 * @input: a string literal
 * @output: the string value for the input which has all the escape sequences 
 * translated into their meaning.
 */
char *getstr(const char *str)
{
	//optional: implement this function if you need it
	return NULL;
}

%}
/* You can add lex definitions here. */
letter [a-zA-Z]
digit [0-9]
id {letter}({letter}|{digit}|_)*
space " "|"\t"

%Start COMMENT ERROR INITAL STR 
%%

  /* 
  * Below is an example, which you can wipe out
  * and write reguler expressions and actions of your own.
  */ 

<INITIAL>"while" {adjust();return WHILE;}
<INITIAL>"for" {adjust(); return FOR;}
<INITIAL>"to" {adjust(); return TO;}
<INITIAL>"do" {adjust(); return DO;}
<INITIAL>"break" {adjust(); return BREAK;}
<INITIAL>"let" {adjust(); return LET;}
<INITIAL>"in" {adjust(); return IN;}
<INITIAL>"end" {adjust(); return END;}
<INITIAL>"function" {adjust(); return FUNCTION;}
<INITIAL>"var" {adjust(); return VAR;}
<INITIAL>"type" {adjust(); return TYPE;}
<INITIAL>"array" {adjust(); return ARRAY;}
<INITIAL>"if" {adjust(); return IF;}
<INITIAL>"then" {adjust(); return THEN;}
<INITIAL>"else" {adjust(); return ELSE;}
<INITIAL>"of" {adjust(); return OF;}
<INITIAL>"nil" {adjust(); return NIL;}
<INITIAL>"," {adjust(); return COMMA;}
<INITIAL>":" {adjust(); return COLON;}
<INITIAL>";" {adjust(); return SEMICOLON;}
<INITIAL>"(" {adjust(); return LPAREN;}
<INITIAL>")" {adjust(); return RPAREN;}
<INITIAL>"[" {adjust(); return LBRACK;}
<INITIAL>"]" {adjust(); return RBRACK;}
<INITIAL>"{" {adjust(); return LBRACE;}
<INITIAL>"}" {adjust(); return RBRACE;}
<INITIAL>"." {adjust(); return DOT;}
<INITIAL>"+" {adjust(); return PLUS;}
<INITIAL>"-" {adjust(); return MINUS;}
<INITIAL>"*" {adjust(); return TIMES;}
<INITIAL>"/" {adjust(); return DIVIDE;}
<INITIAL>"=" {adjust(); return EQ;}
<INITIAL>"<>" {adjust(); return NEQ;}
<INITIAL>"<" {adjust(); return LT;}
<INITIAL>"<=" {adjust(); return LE;}
<INITIAL>">" {adjust(); return GT;}
<INITIAL>">=" {adjust(); return GE;}
<INITIAL>"&" {adjust(); return AND;}
<INITIAL>"|" {adjust(); return OR;}
<INITIAL>":=" {adjust(); return ASSIGN;}
	/* ignored tokens */
<INITIAL>{space} {adjust(); continue;}
<INITIAL>"\n" {adjust(); EM_newline(); continue;}
<INITIAL>\" {adjust();len=0;BEGIN STR;}
    /* \n \t*/
<STR>"\\t" {charPos += yyleng; res[len] = '\t'; len++;}
<STR>"\\n" {charPos += yyleng; res[len] = '\n'; len++;}
    /*
     \ddd   
     yytext+1 means to jump "\"   
     why atoi  --- 
     */
<STR>\\{digit}{1,3} {charPos += yyleng; res[len] = atoi(yytext+1); len++;} 
  /* handle \" */
<STR>\\\" {charPos += yyleng; res[len] = '"'; len++;}
  /* handle \\ */
<STR>\\\\ {charPos += yyleng; res[len] = '\\'; len++;}
  /* handle \f...f\  空格 制表符 换行符 走纸符 */
<STR>\\[ \t\n\f]+\\ {charPos += yyleng;}

<STR>\\\^[@A-Z\[\\\]\^_] {
  charPos+=yyleng;
  res[len]=yytext[2]-'@';
  len++;
  }

<STR>\" {
	charPos += yyleng;  
  if(len==0)
  {
    yylval.sval= String("");
    BEGIN INITIAL;
 
	return STRING;
  }
	res[len] = '\0'; 
	yylval.sval = String(res); 
	BEGIN INITIAL;
 
	return STRING;
}
  /* 匹配剩下的其他字符 */
<STR>. {charPos += yyleng; strcpy(res + len, yytext); len += yyleng;}
<INITIAL>{digit}+ {adjust(); yylval.ival = atoi(yytext); return INT;}
<INITIAL>{id} {adjust(); yylval.sval = String(yytext); return ID;}
<INITIAL>"/*" {
	adjust(); 
	if (comments == 0) {
		BEGIN COMMENT;
	}
	comments++;
}
<COMMENT>"/*" {
	adjust();
	comments++;
}
<COMMENT>"*/" {
	adjust();
	comments--;
	if (comments == 0) {
		BEGIN INITIAL;
	}
}
<COMMENT>"\n" {adjust(); EM_newline(); continue;}
<COMMENT><<EOF>> {EM_tokPos=charPos;EM_error(EM_tokPos, "unclosed comments"); BEGIN ERROR;}
<COMMENT>. {adjust();}

	/* error handler */
<ERROR>.|"\n" {BEGIN INITIAL; yyless(0);}
<INITIAL><<EOF>> {adjust(); yyterminate(); }
<INITIAL>. {adjust(); EM_error(EM_tokPos, "illegal character"); BEGIN ERROR;}

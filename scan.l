%{
extern void yyerror(const char *s);
extern void yyerror(const char *s, const char ch);
#include <iostream>
#include <cstring>
#include "ast.h"
#include "parse.tab.h"
%}
letter      [a-zA-Z]
ident       {letter}({letter}|{digit})*
digit       [0-9]
digits      [0-9]+
float       {digits}"."{digits}

%x COMMENT
%x OTHER_COMMENT
%%


"'''"                { BEGIN(COMMENT); }
<COMMENT>"'''"       { BEGIN(0); }
<COMMENT>"\n"       { ; }
<COMMENT>.          { ; }

"#"                { BEGIN(OTHER_COMMENT); }
<OTHER_COMMENT>"\n"       { BEGIN(0); }
<OTHER_COMMENT>.          { ; }

"**"       { return DOUBLE_MULT; }
"="        { return ASSIGN; }
"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return MULT; }
"/"        { return DIV; }
"%"        { return MOD; }
"("        { return LPAREN; }
")"        { return RPAREN; }
":"        { return COLON; }
","        { return COMMA; }
"<"        { return LESS; }
">"        { return GREAT; }
"<="       { return LESSEQ; }
">="       { return GREATEQ; }
"=="       { return EQ; }
"!="       { return NOTEQ; }
"if"       { return IF; }
"else"     { return ELSE; }
"def"      { return DEF; }
"pass"     { return PASS; }
"print"    { return PRINT; }
"end"      { return END; }
"return"   { return RETURN; }
"["        { return LBRACKET; }
"]"        { return RBRACKET; }
"."        { return PERIOD; }
"len"      { return LEN; }
"while"      { return WHILE; }

{ident}    {
			yylval.id   = new char[strlen(yytext)+1];
	  		strcpy(yylval.id, yytext);
			return IDENT; 
		}

{float}    { 
			yylval.fval = atof(yytext);
			return FLOAT; 
		}
{digits}   { 
			yylval.ival = atoi(yytext);
			return NUMBER; 
		}
"\n"       { return CR; }
.          { ; }

%%
int yywrap() {
  yylex_destroy();
  return 1;
}

%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

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

%}

%%
" "  {adjust(); continue;}
\t  {adjust(); continue;}
\n  {adjust(); EM_newline(); continue;}

","	 {adjust(); return COMMA;}
"="  {adjust(); return EQ;}
"{"  {adjust(); return LBRACE;}
"}"  {adjust(); return RBRACE;}
":"  {adjust(); return COLON;}
";"  {adjust(); return SEMICOLON;}
"."  {adjust(); return DOT;}
"*"  {adjust(); return TIMES;}
"/"  {adjust(); return DIVIDE;}
"+"  {adjust(); return PLUS;}
"-"  {adjust(); return MINUS;}
"("  {adjust(); return LPAREN;}
")"  {adjust(); return RPAREN;}
"["  {adjust(); return LBRACK;}
"]"  {adjust(); return RBRACK;}
":="  {adjust(); return ASSIGN;}
">="  {adjust(); return GE;}
"<="  {adjust(); return LE;}
"<>"  {adjust(); return NEQ;}
">"  {adjust(); return GT;}
"<"  {adjust(); return LT;}
"&"  {adjust(); return AND;}
"|"  {adjust(); return OR;}

for  {adjust(); return FOR;}
while  {adjust(); return WHILE;}
do  {adjust(); return DO;}
var  {adjust(); return VAR;}
let  {adjust(); return LET;}
type  {adjust(); return TYPE;}
function  {adjust(); return FUNCTION;}
in  {adjust(); return IN;}
if  {adjust(); return IF;}
then  {adjust(); return THEN;}
else  {adjust(); return ELSE;}
nil  {adjust(); return NIL;}
end  {adjust(); return END;}
array  {adjust(); return ARRAY;}
of  {adjust(); return OF;}
to  {adjust(); return TO;}

[a-zA-Z_][a-zA-Z0-9_]*  {adjust(); yylval.sval = String(yytext); return ID;}
[0-9]+	 {adjust(); yylval.ival=atoi(yytext); return INT;}
\"([^\"\n]|(\\\"))*\"  {adjust(); string s = String(yytext); yylval.sval = parseStr(stripQuote(s)); return STRING;}
"/*"([^\*]|(\*)*[^\*/])*"*/"  {adjust(); continue;}

.	 {adjust(); EM_error(EM_tokPos,"illegal token");}

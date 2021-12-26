%{
  #include "stdio.h"
  #include "stdlib.h"

  #include "main.h"
  #include "parser.tab.h"

  #define YY_USER_ACTION yylval.offset += yyleng;
%}

%%

[ \t]+
[()[,^*+T\-\]\n] { return yytext[0]; }
(0*[0-9]+) { yylval.number = atoi(yytext); return NUMBER; }
. { SEMANTIC_ERROR(yylval.offset); }

%%
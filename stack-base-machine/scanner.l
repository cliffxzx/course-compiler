%{
  #include "stdio.h"
  #include "stdlib.h"

  #include "parser.tab.h"
%}

%%

load { return LOAD; }
add { return ADD; }
sub { return SUB; }
mul { return MUL; }
mod { return MOD; }
inc { return INC; }
dec { return DEC; }
0*[0-9]+ {
  yylval.number = atoi(yytext);
  return NUMBER;
}
<<EOF>> { return EOF_TOKEN; }
\n
[ \t]+

%%
%option noyywrap
%%

^([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)[eE][+-]?[0-9]+\n { 
  if (strcmp(yytext, ".\n") != 0)
    ECHO;
}

^[0-9]+[eE][+-]?[0-9]+\n { 
  if (strcmp(yytext, ".\n") != 0)
    ECHO;
}

^([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)\n { 
  if (strcmp(yytext, ".\n") != 0)
    ECHO;
}

^.*\n { }

%%

int main() {
  yylex();
  return 0;
}

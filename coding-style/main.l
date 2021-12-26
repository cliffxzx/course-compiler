%option noyywrap
%{
  char *trim_start(char *str) {
    while(*str == ' ' || *str == '\t') str++;
    str[strlen(str)] = '\0';
    return str;
  }

  unsigned int _indent = 0;
  void indent() {
    for(int i = 0; i < _indent; i++) 
      printf("\t");

    printf("%s", trim_start(yytext));
  }
%}

%%

^([^\t ]*.+|[\t ]*)\{ { indent(); ++_indent; }
^([^\t ]*.+|[\t ]*)\} { --_indent; indent();}
^[ \t]*[^{}\n]*\n { indent(); }


%%

int main() {
  yylex();
  return 0;
}

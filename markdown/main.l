%option noyywrap
%{
  char *trim_start(char *str, int omit) {
    while(omit-- > 0) str++;
    str[strlen(str)] = '\0';
    return str;
  }

  char *trim_end(char *str, int len, int omit) {
    while(str[len -1 ] == '\r'|| str[len -1 ] == '\n') str[--len] = '\0';
    return str;
  }

  void print_img(char *str, int len) {
    str = trim_start(str, 2);
    char alt[4096], src[4096];
    int alt_pos = 0, src_pos = 0;
    while(*str != ']') {
      alt[alt_pos++] = *str;
      ++str;
    }
    str = trim_start(str, 2);
    while(*str != ')') {
      src[src_pos++] = *str;
      ++str;
    }

    alt[alt_pos] = 0;
    src[src_pos] = 0;

    printf("<img alt=\"%s\" src=\"%s\">\n", alt, src);
  }

  void print_link(char *str, int len) {
    str = trim_start(str, 1);
    char alt[4096], src[4096];
    int alt_pos = 0, src_pos = 0;
    while(*str != ']') {
      alt[alt_pos++] = *str;
      ++str;
    }
    str = trim_start(str, 2);
    while(*str != ')') {
      src[src_pos++] = *str;
      ++str;
    }

    alt[alt_pos] = 0;
    src[src_pos] = 0;

    printf("<a href=\"%s\">\n\t<span>%s</span>\n</a>\n", src, alt);
  }

  void print_checkbox(char *str, int len) {
    int checked = str[3] == 'X' || str[3] == 'x';
    str = trim_start(trim_end(str, len, 1), 5);
    if (checked) {
      printf("<p>\n\t<input checked=\"checked\" type=\"checkbox\" />%s\n</p>\n", str);
      return;
    }
    printf("<p>\n\t<input type=\"checkbox\" />%s\n</p>\n", str);
  }

  void print_list(char *str, int len) {
    printf("<ul>\n");
    while(str[0] != '-' || str[1] != '-' || str[2] != '-') {
      int pos = 0;
      char ul[4096];
      str = trim_start(str, 2);
      while(*str != '\n') { 
        ul[pos] = *str;
        ++pos;
        ++str;
      }
      ul[pos] = 0;
      printf("\t<li>%s</li>\n", ul);
      ++str;
    }

    printf("</ul>\n<hr />\n");
  }

  void print_quote(char *str, int len) {
    printf("<blockquote>\n");
    while(str[0] != '-' || str[1] != '-' || str[2] != '-') {
      int pos = 0;
      char ul[4096];
      str = trim_start(str, 2);
      while(*str != '\n') { 
        if(str[0] == '*' && str[1] == '*') {
          ul[pos++] = '<';
          ul[pos++] = 's';
          ul[pos++] = 't';
          ul[pos++] = 'r';
          ul[pos++] = 'o';
          ul[pos++] = 'n';
          ul[pos++] = 'g';
          ul[pos++] = '>';
          str = trim_start(str, 2);
          while(*str != '*') {
            ul[pos++] = *str;
            ++str;
          }
          str = trim_start(str, 2);
          ul[pos++] = '<';
          ul[pos++] = '/';
          ul[pos++] = 's';
          ul[pos++] = 't';
          ul[pos++] = 'r';
          ul[pos++] = 'o';
          ul[pos++] = 'n';
          ul[pos++] = 'g';
          ul[pos++] = '>';
          continue;
        }
        
        ul[pos] = *str;
        ++str;
        ++pos;
      }
      ul[pos] = 0;
      printf("%s\n", ul);
      ++str;
    }

    printf("</blockquote>\n<hr />\n");
  }

%}
%%

^(\>\x20.*\n)+---\r?\n?  { print_quote(yytext, yyleng); }
^(\*\x20.*\n)+---\r?\n?  { print_list(yytext, yyleng); }
^#\x20.*\r?\n? { printf("<h1>%s</h1>\n", trim_start(trim_end(yytext, yyleng, 1), 2)); }
^##\x20.*\r?\n? { printf("<h2>%s</h2>\n", trim_start(trim_end(yytext, yyleng, 1), 3)); }
^###\x20.*\r?\n? { printf("<h3>%s</h3>\n", trim_start(trim_end(yytext, yyleng, 1), 4)); }
^####\x20.*\r?\n? { printf("<h4>%s</h4>\n", trim_start(trim_end(yytext, yyleng, 1), 5)); }
^#####\x20.*\r?\n? { printf("<h5>%s</h5>\n", trim_start(trim_end(yytext, yyleng, 1), 6)); }
^######\x20.*\r?\n? { printf("<h6>%s</h6>\n", trim_start(trim_end(yytext, yyleng, 1), 7)); }
^---\r?\n? { printf("<hr />\n"); }
^\[.*\]\(.*\)\r?\n? { print_link(yytext, yyleng); }
^\!\[.*\]\(.*\)\r?\n? { print_img(yytext, yyleng); }
^-\x20\[(\x20|X|x)\]\x20.*\r?\n? { print_checkbox(trim_end(yytext, yyleng, 1), yyleng); }

^.*\r?\n? { }

%%

int main() {
  yylex();
  return 0;
}

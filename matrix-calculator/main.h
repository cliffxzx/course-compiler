#ifndef MAIN_H
#define MAIN_H

typedef struct type {
  int offset;
  union {
    int shape[2];
    int number;
  };
} Type;

#define YYSTYPE Type

extern int yylex();
extern int yyparse();
void yyerror(const char *message);

#define INT_MAX_LEN 25
#define SEMANTIC_ERROR(t_col) do { char col[INT_MAX_LEN]; sprintf(col, "Semantic error on col %d", t_col); yyerror(col); } while(0)

#endif
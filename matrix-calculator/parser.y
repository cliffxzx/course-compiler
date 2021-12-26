%{
#include "stdio.h"
#include "stdlib.h"

#include "main.h"

void yyerror(const char *message) { fprintf(stdout, "%s\n", message); exit(0); }

#define CHECK_ERROR(error, pos) do { if (error) SEMANTIC_ERROR(pos); } while(0)
#define ASSIGN_SHAPE(a, bx, by) do { a[0] = bx; a[1] = by; } while(0)
%}

%type <shape> matrix
%token <number> NUMBER

%left <offset> '+' '-'
%left <offset> '*'
%left <offset> '^'

%%

stmts: | stmts expr;
expr: matrix;
expr: '\n' { printf("%s\n", "Accepted"); exit(0); };
matrix: matrix '^' 'T' { ASSIGN_SHAPE($$, $1[1], $1[0]); };
matrix: '(' matrix ')' { ASSIGN_SHAPE($$, $2[0], $2[1]); };
matrix: '[' NUMBER ',' NUMBER ']' { ASSIGN_SHAPE($$, $2, $4); };
matrix: matrix '*' matrix { CHECK_ERROR($1[1] != $3[0], $2); ASSIGN_SHAPE($$, $1[0], $3[1]);};
matrix: matrix '+' matrix { CHECK_ERROR($1[0] != $3[0] || $1[1] != $3[1], $2); ASSIGN_SHAPE($$, $1[0], $1[1]); };
matrix: matrix '-' matrix { CHECK_ERROR($1[0] != $3[0] || $1[1] != $3[1], $2); ASSIGN_SHAPE($$, $1[0], $1[1]); };

%%

int main() {
    yylval.offset = 0;
    yyparse();
    return 0;
}
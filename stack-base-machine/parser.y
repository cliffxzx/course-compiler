%{
#define STACK_MAXSIZE 100

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern int yylex();
extern int yyparse();
void yyerror(const char *message) { fprintf(stdout, "Invalid format\n"); exit(0); }

int top = -1;
int stack[STACK_MAXSIZE];
int is_empty() { return top == -1; }
int is_full() { return top == STACK_MAXSIZE; }
int size() { return top + 1; }
int peek() {
  if(is_empty())
    yyerror("");

  return stack[top];
}
void push(int val) {
  if(is_full())
    yyerror("");

  stack[++top] = val;
}
int pop() {
  int temp = peek();
  --top;
  return temp;
}

%}

%union {
  int number;
}

%token <number> NUMBER
%token LOAD ADD SUB MUL MOD INC DEC NUMBER EOF_TOKEN

%%

stmts: | stmts stmt;

stmt: LOAD NUMBER { push($2); };
stmt: ADD { push(pop() + pop()); };
stmt: SUB { push(pop() - pop()); };
stmt: MUL { push(pop() * pop()); };
stmt: MOD { push(pop() % pop()); };
stmt: INC { push(pop() + 1); };
stmt: DEC { push(pop() - 1); };
stmt: EOF_TOKEN {
  if(size() != 1)
    yyerror("");

  printf("%d\n", pop());
  exit(0);
};
%%

int main() {
    yyparse();
    return 0;
}
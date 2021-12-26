#include <iostream>
#include <string>

using namespace std;

const string TOKENS[] = {"NUM", "PLUS", "MINUS", "MUL", "DIV", "LPR", "RPR"};

struct Token {
  int type;
  string val;
};

Token next_token() {
  char symbol;
  Token token = (Token){.type = -1};
  while ((symbol = cin.peek()) != EOF) {
    while (symbol >= '0' && symbol <= '9') {
      cin.get();
      token = (Token){.type = 0, .val = token.val + symbol};
      symbol = cin.peek();
    }

    if (!token.type)
      return token;

    cin.get();
    switch (symbol) {
      case '+':
        return (Token){.type = 1};
      case '-':
        return (Token){.type = 2};
      case '*':
        return (Token){.type = 3};
      case '/':
        return (Token){.type = 4};
      case '(':
        return (Token){.type = 5};
      case ')':
        return (Token){.type = 6};
    }
  }

  return token;
}

void scanner() {
  Token token;
  while ((token = next_token()).type != -1)
    cout << TOKENS[token.type]
         << (token.val.empty() ? "\n" : " " + token.val + "\n");
}

int main() {
  scanner();

  return 0;
}
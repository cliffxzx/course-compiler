#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class STATUS : int {
  SUCCESS,
  ALL,
  IN_TAG,
  NOT_IN_TAG,
  ERROR
};

class Token {
public:
  string type, val;
  Token() : type(), val(){};
  Token(string _type, string _val = "") : type(_type), val(_val){};
  bool operator==(const Token &token) { return this->type == token.type; };
  static STATUS to_tokens(string input, vector<Token> &tokens);
} TAG_OPEN_SLASH("TAG_OPEN_SLASH", "</"),
    TAG_OPEN("TAG_OPEN", "<"),
    TAG_CLOSE("TAG_CLOSE", ">"),
    TAG_EQUALS("TAG_EQUALS", "="),
    SINGLE_QUOTE_STRING("SINGLE_QUOTE_STRING"),
    DOUBLE_QUOTE_STRING("DOUBLE_QUOTE_STRING"),
    TAG_NAME("TAG_NAME"),
    HTML_TEXT("HTML_TEXT");
ostream &operator<<(ostream &os, Token token) { return os << token.type << " " + token.val << "\n"; }

class Parser {
public:
  STATUS parse(string input, vector<Token> &tokens) {
    size_t pos = 0;
    STATUS stat = STATUS::ALL;
    while (pos < input.size()) {
      char symbol = input[pos];
      if (symbol == ' ') {
        symbol = next(pos, input);
        continue;
      }

      if (stat == STATUS::IN_TAG) {
        if (symbol == '\'') {
          Token token = SINGLE_QUOTE_STRING;
          while (true) {
            if ((symbol = next(pos, input)) == -1)
              return STATUS::ERROR;

            if (symbol == '\'')
              break;

            token.val += symbol;
          }

          tokens.push_back(token);
          symbol = next(pos, input);
          continue;
        } else if (symbol == '"') {
          Token token = DOUBLE_QUOTE_STRING;
          while (true) {
            if ((symbol = next(pos, input)) == -1)
              return STATUS::ERROR;

            if (symbol == '"')
              break;

            token.val += symbol;
          }

          tokens.push_back(token);
          symbol = next(pos, input);
          continue;
        } else if ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || isdigit(symbol)) {
          Token token = TAG_NAME;
          while ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || isdigit(symbol)) {
            token.val += symbol;
            if ((symbol = next(pos, input)) == -1)
              return STATUS::ERROR;
          }
          tokens.push_back(token);
        }
      }

      switch (symbol) {
        case '<':
          if (pos + 1 < input.size() && input[pos + 1] == '/') {
            tokens.push_back(TAG_OPEN_SLASH);
            symbol = next(pos, input);
          } else
            tokens.push_back(TAG_OPEN);
          stat = STATUS::IN_TAG;
          symbol = next(pos, input);
          continue;
        case '>':
          tokens.push_back(TAG_CLOSE);
          stat = STATUS::NOT_IN_TAG;
          symbol = next(pos, input);
          continue;
        case '=':
          tokens.push_back(TAG_EQUALS);
          symbol = next(pos, input);
          continue;
      }

      if (stat == STATUS::NOT_IN_TAG) {
        if (symbol != '<') {
          Token token = HTML_TEXT;
          while (symbol != '<') {
            token.val += symbol;
            if ((symbol = next(pos, input)) == -1)
              return STATUS::ERROR;
          }
          tokens.push_back(token);
        }
      }
    }

    return STATUS::SUCCESS;
  }

private:
  vector<Token> _tokens;
  char next(size_t &pos, string &input) { return (input.size() > ++pos ? input[pos] : -1); }
};

int main() {
  string input;
  while (getline(cin, input)) {
    vector<Token> tokens;
    if (Parser().parse(input, tokens) == STATUS::SUCCESS) {
      for (Token token : tokens)
        cout << token;
      cout << "\n";
    } else
      cout << "invalid input\n";
  }

  return 0;
}
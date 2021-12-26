#include <cctype>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class STATUS : int { SUCCESS,
                          ERROR };

class Token {
public:
  string type, val;
  Token() : type(), val(){};
  Token(string _type, string _val = "") : type(_type), val(_val){};
  bool operator==(const Token &token) { return this->type == token.type; };
  static STATUS to_tokens(string input, vector<Token> &tokens, const map<char, Token> &symbol_to_token, bool omit_space);
} NUM("NUM"), PLUS("PLUS"), MINUS("MINUS"), MUL("MUL"), DIV("DIV"), LPR("LPR"), RPR("RPR");
ostream &operator<<(ostream &os, Token token) { return os << token.type << (token == NUM ? " " + token.val : "") << "\n"; }
STATUS Token::to_tokens(string input, vector<Token> &tokens, const map<char, Token> &symbol_to_token, bool omit_spaces = true) {
  for (int w = 0; w < input.size();) {
    char symbol = input[w];
    Token token = NUM;
    while (isdigit(symbol)) {
      token.val += symbol;

      if (++w >= input.size())
        break;

      symbol = input[w];
    }

    if (!token.val.empty()) {
      tokens.push_back(token);
      continue;
    }

    auto it = symbol_to_token.find(symbol);
    if (!(omit_spaces && (symbol != ' ' || symbol != '\n')) && it == symbol_to_token.end())
      return STATUS::ERROR;

    tokens.push_back(it->second);
    ++w;
  }

  return STATUS::SUCCESS;
}

int main() {
  vector<Token> tokens;
  string stdin_input = string(istream_iterator<char>(cin), istream_iterator<char>());
  const map<char, Token> symbol_to_token{{'+', PLUS}, {'-', MINUS}, {'*', MUL}, {'/', DIV}, {'(', LPR}, {')', RPR}};
  if (Token::to_tokens(stdin_input, tokens, symbol_to_token) == STATUS::SUCCESS)
    for (Token token : tokens)
      cout << token;

  return 0;
}
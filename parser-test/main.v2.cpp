#include <cctype>
#include <functional>
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
} ID("ID"), STRLIT("STRLIT"), LBR("LBR", "("), RBR("RBR", ")"), DOT("DOT", ".");
ostream &operator<<(ostream &os, Token token) { return os << token.type << " " + token.val << "\n"; }
STATUS Token::to_tokens(string input, vector<Token> &tokens, const map<char, Token> &symbol_to_token, bool omit_spaces = true) {
  for (int w = 0; w < input.size();) {
    char symbol = input[w];
    Token token = ID;
    while ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || symbol == '_' || (!token.val.empty() && isdigit(symbol))) {
      token.val += symbol;

      if (++w >= input.size())
        break;

      symbol = input[w];
    }

    if (!token.val.empty()) {
      tokens.push_back(token);
      continue;
    }

    if (symbol == '"') {
      token = STRLIT;
      do {
        token.val += symbol;
        if (++w >= input.size())
          return STATUS::ERROR;

        symbol = input[w];
      } while (symbol != '"');

      token.val += '"';
      tokens.push_back(token);
      ++w;
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

bool is_program(string &output);
bool is_stmts(string &output);
bool is_stmt(string &output);
bool is_primary(string &output);
bool is_primary_tail(string &output);

class Parser {
public:
  STATUS parse(string input, vector<Token> &tokens, const map<char, Token> symbol_to_token) {
    if (Token::to_tokens(input, _tokens, symbol_to_token, false) == STATUS::SUCCESS)
      if (is_program(0)) {
        tokens = _tokens;
        return STATUS::SUCCESS;
      }
    return STATUS::ERROR;
  }

private:
  vector<Token> _tokens;
  bool is_lambda(int pos) { return pos >= _tokens.size(); }
  bool is_token(int pos, Token token) { return pos < _tokens.size() && _tokens[pos] == token; }
  bool peek_correct(int &pos, function<bool(int &)> func) {
    int moved_pos = pos;
    if (func(moved_pos)) {
      pos = moved_pos;
      return true;
    }

    return false;
  }

  bool is_program(int pos = 0) { return is_stmts(pos); }
  bool is_stmts(int &pos) {
    return peek_correct(pos, [&](int &pos) { return is_lambda(pos) || is_stmt(pos) && is_stmts(++pos); });
  }

  bool is_stmt(int &pos) {
    return peek_correct(pos, [&](int &pos) { return is_lambda(pos) || is_token(pos, STRLIT) || is_primary(pos); });
  }

  bool is_primary(int &pos) {
    return peek_correct(pos, [&](int &pos) { return is_token(pos, ID) && is_primary_tail(++pos); });
  }

  bool is_primary_tail(int &pos) {
    if (is_token(pos, LBR))
      return peek_correct(pos, [&](int &pos) { return is_stmt(++pos) && is_token(++pos, RBR) && is_primary_tail(++pos); }) || peek_correct(pos, [&](int &pos) { return is_token(++pos, RBR) && is_primary_tail(++pos); });

    if (is_token(pos, DOT))
      return peek_correct(pos, [&](int &pos) { return is_token(++pos, ID) && is_primary_tail(++pos); });

    return true; // is_lambda(pos);
  }
};

int main() {
  string input;
  const map<char, Token> symbol_to_token{{'(', LBR}, {')', RBR}, {'.', DOT}};
  while (getline(cin, input)) {
    vector<Token> tokens;
    if (Parser().parse(input, tokens, symbol_to_token) == STATUS::SUCCESS)
      for (Token token : tokens)
        cout << token;
    else
      cout << "invalid input\n";
  }

  return 0;
}
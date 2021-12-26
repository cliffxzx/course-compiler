#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool is_program(string &output);
bool is_stmts(string &output);
bool is_stmt(string &output);
bool is_primary(string &output);
bool is_primary_tail(string &output);

const string TOKENS[] = {"ID", "STRLIT", "LBR", "RBR", "DOT"};

struct Token {
  int type;
  string val;
};

// -1 error, -2 eof
Token next_token() {
  char symbol;
  Token token = (Token){.type = -2};
  if ((symbol = cin.peek()) != EOF) {
    while ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || symbol == '_' || (!token.val.empty() && isdigit(symbol))) {
      cin.get();
      token = (Token){.type = 0, .val = token.val + symbol};
      symbol = cin.peek();
    }

    if (!token.type)
      return token;

    if (symbol == '"') {
      do {
        cin.get();
        token.val += symbol;
        if ((symbol = cin.peek()) == EOF || symbol == '\n')
          return (Token){.type = -1};
      } while (symbol != '"');

      return (Token){.type = 1, .val = token.val + (char)cin.get()};
    }

    cin.get();
    switch (symbol) {
      case '(':
        return (Token){.type = 2, .val = "("};
      case ')':
        return (Token){.type = 3, .val = ")"};
      case '.':
        return (Token){.type = 4, .val = "."};
      case '\n':
        return (Token){.type = 5, .val = "\n"};
      default:
        return (Token){.type = -1};
    }
  }

  return token;
}

class Parser {
public:
  void parse_stdin() {
    bool eof;
    while (!eof) {
      bool error = false;
      string output;
      while (true) {
        _token.push_back(next_token());

        if (is_eoi(_token.size() - 1) || (eof = is_eof()))
          break;

        if (error || (error = is_error(_token.size() - 1)))
          continue;

        output += TOKENS[_token.back().type] + ' ' + _token.back().val + '\n';
      }

      cout << (!error && is_program(0) ? output : "invalid input\n");
      _token.clear();
    }
  }

private:
  vector<Token> _token;
  bool is_eof() { return _token.back().type == -2; }
  bool is_error(int idx = 0) { return _token[idx].type == -1; }
  bool is_id(int idx = 0) { return _token[idx].type == 0; }
  bool is_strlit(int idx = 0) { return _token[idx].type == 1; }
  bool is_lbr(int idx = 0) { return _token[idx].type == 2; }
  bool is_rbr(int idx = 0) { return _token[idx].type == 3; }
  bool is_dot(int idx = 0) { return _token[idx].type == 4; }
  bool is_eoi(int idx = 0) { return _token[idx].type == 5; }
  bool is_program(int pos) { return is_stmts(pos); }
  bool is_stmts(int pos) { return is_eoi(pos) || ((is_stmt(pos)) && is_stmts(pos + 1)); }
  bool is_stmt(int pos) { return is_eoi(pos) || is_primary(pos) || is_strlit(pos); }
  bool is_primary(int pos) { return is_id(pos) && is_primary_tail(pos + 1); }
  bool is_primary_tail(int pos) {
    if (is_lbr(pos))
      return is_stmt(pos + 1) && is_rbr(pos + 2) && is_primary_tail(pos + 3);

    if (is_dot(pos))
      return is_id(pos + 1) && is_primary_tail(pos + 2);

    return is_eoi(pos);
  }
};

int main() {
  Parser().parse_stdin();

  return 0;
}
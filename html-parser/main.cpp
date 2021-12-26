#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
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
  STATUS parse(string input, vector<Token> &tokens, string &log) {
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

          _tokens.push_back(token);
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

          _tokens.push_back(token);
          symbol = next(pos, input);
          continue;
        } else if ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || isdigit(symbol)) {
          Token token = TAG_NAME;
          while ((symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || isdigit(symbol)) {
            token.val += symbol;
            if ((symbol = next(pos, input)) == -1)
              return STATUS::ERROR;
          }
          _tokens.push_back(token);
        }
      }

      switch (symbol) {
        case '<':
          if (pos + 1 < input.size() && input[pos + 1] == '/') {
            _tokens.push_back(TAG_OPEN_SLASH);
            symbol = next(pos, input);
          } else
            _tokens.push_back(TAG_OPEN);
          stat = STATUS::IN_TAG;
          symbol = next(pos, input);
          continue;
        case '>':
          _tokens.push_back(TAG_CLOSE);
          stat = STATUS::NOT_IN_TAG;
          symbol = next(pos, input);
          continue;
        case '=':
          _tokens.push_back(TAG_EQUALS);
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
          _tokens.push_back(token);
        }
      }
    }

    if (!is_html_file(log)) {
      log = error_log;
      return STATUS::ERROR;
    }

    tokens = _tokens;
    return STATUS::SUCCESS;
  }

private:
  vector<Token> _tokens;
  string error_log;
  bool is_lambda(int pos) { return pos >= _tokens.size(); }
  bool is_token(int pos, Token token) { return pos < _tokens.size() && _tokens[pos] == token; }
  char next(size_t &pos, string &input) { return (input.size() > ++pos ? input[pos] : -1); }
  bool peek_correct(int &pos, string &log, string info, function<bool(int &, string &)> func) {
    int moved_pos = pos;
    string moved_log = log;
    if (func(moved_pos, moved_log)) {
      pos = moved_pos;
      log = moved_log + info;
      return true;
    }

    if (!moved_log.empty())
      error_log = moved_log;
    return false;
  }

  bool is_html_file(string &log, int pos = 0) { return htmlDocument(pos, log); }
  bool htmlDocument(int &pos, string &log) {
    return peek_correct(
               pos, log, "htmlDocument\n",
               [&](int &pos, string &log) { return htmlElement(pos, log) && htmlDocument(++pos, log); }) ||
           peek_correct(
               pos, log, "htmlDocument\n",
               [&](int &pos, string &log) { return htmlElement(pos, log); });
  }

  bool htmlElement(int &pos, string &log) {
    return peek_correct(
               pos, log, "htmlElement\n",
               [&](int &pos, string &log) {
                 return is_token(pos, TAG_OPEN) &&
                        is_token(++pos, TAG_NAME) &&
                        htmlAttributeList(++pos, log) &&
                        is_token(++pos, TAG_CLOSE) &&
                        htmlContent(++pos, log) &&
                        is_token(++pos, TAG_OPEN_SLASH) &&
                        is_token(++pos, TAG_NAME) &&
                        is_token(++pos, TAG_CLOSE);
               }) ||
           peek_correct(
               pos, log, "htmlElement\n",
               [&](int &pos, string &log) {
                 return is_token(pos, TAG_OPEN) &&
                        is_token(++pos, TAG_NAME) &&
                        is_token(++pos, TAG_CLOSE) &&
                        htmlContent(++pos, log) &&
                        is_token(++pos, TAG_OPEN_SLASH) &&
                        is_token(++pos, TAG_NAME) &&
                        is_token(++pos, TAG_CLOSE);
               }) ||
           peek_correct(
               pos, log, "htmlElement\n",
               [&](int &pos, string &log) {
                 return is_token(pos, TAG_OPEN) &&
                        is_token(++pos, TAG_NAME) &&
                        htmlAttributeList(++pos, log) &&
                        is_token(++pos, TAG_CLOSE) &&
                        is_token(++pos, TAG_OPEN_SLASH) &&
                        is_token(++pos, TAG_NAME) &&
                        is_token(++pos, TAG_CLOSE);
               }) ||
           peek_correct(
               pos, log, "htmlElement\n",
               [&](int &pos, string &log) {
                 return is_token(pos, TAG_OPEN) &&
                        is_token(++pos, TAG_NAME) &&
                        is_token(++pos, TAG_CLOSE) &&
                        is_token(++pos, TAG_OPEN_SLASH) &&
                        is_token(++pos, TAG_NAME) &&
                        is_token(++pos, TAG_CLOSE);
               });
  }

  bool htmlContent(int &pos, string &log) {
    return peek_correct(
               pos, log, "htmlContent\n",
               [&](int &pos, string &log) {
                 return (htmlChardata(pos, log) && htmlContent(++pos, log));
               }) ||
           peek_correct(
               pos, log, "htmlContent\n",
               [&](int &pos, string &log) {
                 return (htmlChardata(pos, log));
               }) ||
           peek_correct(
               pos, log, "htmlContent\n",
               [&](int &pos, string &log) {
                 return (htmlElement(pos, log) && htmlContent(++pos, log));
               }) ||
           peek_correct(
               pos, log, "htmlContent\n",
               [&](int &pos, string &log) {
                 return (htmlElement(pos, log));
               });
  }

  bool htmlAttributeList(int &pos, string &log) {
    return peek_correct(
               pos, log, "htmlAttributeList\n",
               [&](int &pos, string &log) {
                 return htmlAttribute(pos, log) && htmlAttributeList(++pos, log);
               }) ||
           peek_correct(
               pos, log, "htmlAttributeList\n",
               [&](int &pos, string &log) {
                 return htmlAttribute(pos, log);
               });
  }

  bool htmlAttribute(int &pos, string &log) {
    return peek_correct(
        pos, log, "htmlAttribute\n",
        [&](int &pos, string &log) { return is_token(pos, TAG_NAME) && is_token(++pos, TAG_EQUALS) && attribute(++pos, log); });
  }

  bool htmlChardata(int &pos, string &log) {
    return peek_correct(
        pos, log, "htmlCharData\n",
        [&](int &pos, string &log) { return is_token(pos, HTML_TEXT); });
  }

  bool attribute(int &pos, string &log) {
    return peek_correct(
               pos, log, "attribute\n",
               [&](int &pos, string &log) { return is_token(pos, SINGLE_QUOTE_STRING); }) ||
           peek_correct(
               pos, log, "attribute\n",
               [&](int &pos, string &log) { return is_token(pos, DOUBLE_QUOTE_STRING); });
  }
};

int main() {
  string input;
  while (getline(cin, input)) {
    vector<Token> tokens;
    string log;
    if (Parser().parse(input, tokens, log) == STATUS::SUCCESS) {
      cout << log << "valid\n";
    } else
      cout << log << "invalid\n";
  }

  return 0;
}
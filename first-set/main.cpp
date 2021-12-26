#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

#define range(_range_var) _range_var.begin(), _range_var.end()

using namespace std;

// TODO:
// 1. the first_set is recursive, can store each token result.

const char CEOF = '$';
const char CNULL = ';';
const string END_OF_GRAMMAR = "END_OF_GRAMMAR";
const string END_OF_FIRST = "END_OF_FIRST";

template <typename T>
T split(const string &str, const string &reg_str) {
  regex reg(reg_str);
  return T(sregex_token_iterator(range(str), reg, -1), sregex_token_iterator());
}

set<char> first_set(const map<char, set<string>> &ebnf, const char &token) {
  if (islower(token) || token == CNULL || token == CEOF)
    return {token};

  bool nullable = false;
  set<char> _first_set;
  for (const string &rule : ebnf.find(token)->second) {
    bool sub_nullable = true;
    set<char> _sub_first_set;
    for (const char &tok : rule) {
      if (islower(tok) || tok == CEOF) {
        _sub_first_set.insert(tok);
        sub_nullable = false;
        break;
      }

      if (tok == CNULL) {
        _sub_first_set.insert(tok);
        continue;
      }

      set<char> res = first_set(ebnf, tok);
      sub_nullable &= res.count(CNULL);
      _sub_first_set.insert(range(res));
      if (!sub_nullable)
        break;
    }

    nullable |= sub_nullable;
    _first_set.insert(range(_sub_first_set));
  }

  if (!nullable || _first_set.count(CEOF))
    _first_set.erase(CNULL);

  return _first_set;
}

int main() {
  string line;
  map<char, set<string>> ebnf;
  while (getline(cin, line) && line != END_OF_GRAMMAR)
    ebnf[line[0]] = split<set<string>>(line.substr(2), "\\|");

  for (const pair<char, set<string>> rules : ebnf) {
    set<char> _first_set = first_set(ebnf, rules.first);
    cout << rules.first << ' ';
    copy(range(_first_set), ostream_iterator<char>(cout, ""));
    cout << "\n";
  }

  cout << END_OF_FIRST << '\n';
  return 0;
}
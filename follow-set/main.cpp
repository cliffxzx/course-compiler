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

const char CEOF = '$';
const char CNULL = ';';
const string END_OF_GRAMMAR = "END_OF_GRAMMAR";
const string END_OF_FIRST_SET = "END_OF_FIRST_SET";

template <typename T>
T split(const string &str, const string &reg_str) {
  regex reg(reg_str);
  return T(sregex_token_iterator(range(str), reg, -1), sregex_token_iterator());
}

bool is_nonterminal(const char B) { return isupper(B); }
bool is_terminal(const char B) { return islower(B); }

char next_B(const string R, const size_t pos) {
  if (R.size() > pos + 1)
    return R[pos + 1];

  return CEOF;
}

set<char> first_set_of(const map<char, set<char>> first_set, const string C) {
  if (C.empty() || C[0] == CNULL)
    return {CNULL};

  set<char> first_set_of_C = first_set.at(C[0]);
  size_t w = 1;
  for (; w < C.size() && first_set.at(C[w - 1]).count(CNULL); ++w) {
    set<char> sub_first_set_of_C = first_set.at(C[w]);
    sub_first_set_of_C.erase(CNULL);
    first_set_of_C.insert(range(sub_first_set_of_C));
  }

  if (w == C.size() && first_set.at(C[w - 1]).count(CNULL)) {
    first_set_of_C.insert(CNULL);
  } else {
    first_set_of_C.erase(CNULL);
  }

  return first_set_of_C;
}

int main() {
  string line;
  multimap<char, string> ebnf;
  map<char, set<char>> first_set = {{CEOF, {CEOF}}};
  while (getline(cin, line) && line != END_OF_GRAMMAR) {
    for (string rules : split<set<string>>(line.substr(2), "\\|")) {
      ebnf.insert({line[0], rules});

      for (const char C : rules) {
        if (is_terminal(C)) {
          first_set[C] = {C};
        }
      }
    }
  }

  while (getline(cin, line) && line != END_OF_FIRST_SET) {
    string R = line.substr(2);
    first_set[line[0]] = set<char>(range(R));

    for (const char C : line) {
      if (is_terminal(C)) {
        first_set[C] = {C};
      }
    }
  }

  bool changed;
  map<char, set<char>> follow_set = {{'S', {CEOF}}};
  do {
    changed = false;
    for (pair<char, string> rules : ebnf) {
      char LHS = rules.first;
      string RHS = rules.second;
      for (int w = 0; w < RHS.size(); ++w) {
        char B = RHS[w];
        if (is_nonterminal(B)) {
          set<char> first_set_of_C = first_set_of(first_set, RHS.substr(w + 1));

          auto size = follow_set[B].size();
          if (first_set_of_C.count(CNULL)) {
            first_set_of_C.erase(CNULL);
            follow_set[B].insert(range(first_set_of_C));
            follow_set[B].insert(range(follow_set[LHS]));
          } else {
            follow_set[B].insert(range(first_set_of_C));
          }

          changed |= follow_set[B].size() != size;
        }
      }
    }
  } while (changed);

  for (pair<char, set<char>> rules : follow_set) {
    rules.second.erase(CNULL);
    cout << rules.first << ' ';
    copy(range(rules.second), ostream_iterator<char>(cout, ""));
    cout << "\n";
  }

  cout << '\n';
  return 0;
}
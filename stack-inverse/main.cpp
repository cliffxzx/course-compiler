#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  string s;
  vector<int> stk;
  while (cin >> s && s != "$") {
    if (s == "push") {
      int i;
      cin >> i;
      stk.push_back(i);
    }

    if (s == "dec") {
      int i = stk.back();
      stk.pop_back();
      stk.push_back(--i);
    }

    if (s == "inc") {
      int i = stk.back();
      stk.pop_back();
      stk.push_back(++i);
    }

    if (s == "inverse") {
      swap(*--stk.end(), *(stk.end() - 2));
    }
  }

  cout << stk.back() << endl;

  return 0;
}
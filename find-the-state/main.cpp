#include <iostream>
#include <string>

using namespace std;

enum class STAT : int {
  S1 = 1,
  S2,
  S3,
  S4,
  SERROR
};

string to_string(STAT stat) { return "s" + to_string((int)stat); }

bool dft(STAT &stat, char symbol) {
  switch (stat) {
    case STAT::S1:
      if (symbol == 'a')
        stat = STAT::S2;
      else if (symbol == 'b')
        stat = STAT::S3;
      else
        return false;

      return true;
    case STAT::S2:
      if (symbol == 'a')
        stat = STAT::S2;
      else if (symbol == 'b')
        stat = STAT::S4;
      else
        return false;

      return true;
    case STAT::S3:
      if (symbol == 'a')
        stat = STAT::S3;
      else if (symbol == 'c')
        stat = STAT::S4;
      else
        return false;

      return true;
    case STAT::S4:
      if (symbol != '$')
        stat = STAT::SERROR;
      return false;
    default:
      break;
  }
}

int main() {
  char symbol;
  STAT stat = STAT::S1;
  while ((symbol = cin.get()) && dft(stat, symbol))
    ;

  if (stat == STAT::S3 || stat == STAT::S4)
    cout << "YES " << to_string(stat);
  else
    cout << "NO";
  return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Phone_Pattern {
  string country_code;
  string operatr_code;
  string personl_code;

  string country;
  string operatr;

  string Get_num() const {
    return country_code + operatr_code + personl_code;
  }

  size_t Get_size() const {
    return Get_num().size();
  }

  bool operator<(const Phone_Pattern& other) const {
    return Get_num() < other.Get_num();
  }
  bool operator<(const string& str) const {
    return Get_num() < str;
  }
};

int main() {
  int n;
  cin >> n;
  cin.ignore();
  vector<string> numbers(n);
  for (string& number : numbers) {
    string line;
    getline(cin, line);
    for (char& ch : line) {
      if (isdigit(ch)) {
        number += ch;
      }
    }
  }

  int m;
  cin >> m;
  vector<Phone_Pattern> patterns(m);
  for (auto& pattern : patterns) {
    cin.ignore(2);
    getline(cin, pattern.country_code, ' ');
    cin.ignore();
    getline(cin, pattern.operatr_code, ')');
    cin.ignore();
    getline(cin, pattern.personl_code, ' ');
    cin.ignore(2);
    cin >> pattern.country >> pattern.operatr;
  }
  sort(patterns.begin(), patterns.end());

  for (string& number : numbers) {
    auto it = lower_bound(patterns.begin(), patterns.end(), number);
    while (it->Get_size() != number.size() && it != prev(patterns.end())) {
      ++it;
    }
    cout << '+' << it->country_code << " (" << it->operatr_code << ") "
         << number.substr(number.size() - it->personl_code.size()) << " - "
         << it->country << ' ' << it->operatr << '\n';
  }
	return 0;
}

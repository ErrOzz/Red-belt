#include <iostream>
#include <vector>
#include <list>

using namespace std;

class athlets {
private:
  static const int MAX_ATHLETS_CNT = 100'000;
  using pointer = list<int>::iterator;
  list<int> line_up;
  vector<pointer> locations = vector<pointer>(MAX_ATHLETS_CNT + 1, line_up.end());
public:
  void add(int athlet, int next) {
    locations[athlet] = line_up.insert(locations[next], athlet);
  }
  void print_line() const {
    for (const int athlet : line_up) {
      cout << athlet << "\n";
    }
  }
};

int main() {
  athlets athlets;
  size_t n;
  cin >> n;
  for (size_t i = 0; i < n; ++i) {
    int athlet, next;
    cin >> athlet >> next;
    athlets.add(athlet, next);
  }
  athlets.print_line();
}

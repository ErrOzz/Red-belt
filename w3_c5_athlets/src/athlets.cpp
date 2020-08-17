#include <iostream>
#include <list>
#include <map>

using namespace std;

class Athlets {
private:
  list<int> athlets_que_;
  using Position = list<int>::iterator;
  map<int, Position> athlets_pos_;
public:
  void Add(const int& athlet, const int& next) {
    Position pos_it = athlets_que_.end();
    auto next_it = athlets_pos_.find(next);
    if (next_it != athlets_pos_.end()) {
      pos_it = next_it->second;
    }
    athlets_pos_[athlet] = athlets_que_.emplace(pos_it, athlet);
  }
  void Print() const {
    for (const int& athlet : athlets_que_) {
      cout << athlet << ' ';
    }
  }
};

int main() {
  Athlets athlets;
  int n;
  cin >> n;
  for (int query = 0; query < n; ++query) {
    int athlet, next;
    cin >> athlet >> next;
    athlets.Add(athlet, next);
  }
  athlets.Print();
}

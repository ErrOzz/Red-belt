#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct Edge {
  set<string>::iterator first;
  set<string>::iterator second;

  bool operator<(const Edge& other) const {
    return tie(*first, *second) < tie(*other.first, *other.second);
  }
};

int main() {
  int t;
  cin >> t;
  vector<string> words(t);
  for (string& word : words) {
    cin >> word;
  }
  set<string> vertexes;
  map<Edge, int> words_grahp;
  for (string& word : words) {
    for (size_t i = 0; i < word.size() - 3; ++i) {
      Edge edge{vertexes.insert(string(word, i, 3)).first, vertexes.insert(string(word, (i + 1), 3)).first};
      words_grahp[move(edge)]++;
    }
  }
  cout << vertexes.size() << '\n';
  cout << words_grahp.size() << '\n';
  for (auto& e_v : words_grahp) {
    cout << *e_v.first.first << ' ' << *e_v.first.second << ' ' << e_v.second << '\n';
  }
	return 0;
}

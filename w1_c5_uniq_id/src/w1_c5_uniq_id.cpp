#include <string>
#include <vector>
using namespace std;

#define LINE_IMPL(line, cntr) uuid_##line##_##cntr
#define LINE(line, cntr)  LINE_IMPL(line, cntr)
#define UNIQ_ID LINE(__LINE__, __COUNTER__)

int main() {
  int UNIQ_ID = 0; int UNIQ_ID = 1;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}

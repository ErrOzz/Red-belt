//#include "profile.h"

#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
  ReadingManager() : user_to_page_(MAX_USER_COUNT_ + 1, -1),
                     page_to_user_count_ (MAX_PAGE_COUNT_ + 1, 0),
                     user_count_(0) {}

  void Read(const int& user_id, const int& page_count) {
    if (user_to_page_[user_id] == -1) {
      ++user_count_;
    } else {
      --page_to_user_count_[user_to_page_[user_id]];
    }
    user_to_page_[user_id] = page_count;
    ++page_to_user_count_[page_count];
  }

  double Cheer(const int& user_id) const {
    if (user_count_ == 0) { return 0; }
    else if (user_count_ == 1) { return 1; }
    int less_pages_users = 0;
    for (int i = 1; i < user_to_page_[user_id]; ++i) {
      less_pages_users += page_to_user_count_[i];
    }
    return (less_pages_users) * 1.0 / (user_count_ - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGE_COUNT_ = 1'000;

  vector<int> user_to_page_;
  vector<int> page_to_user_count_;
  int user_count_;
};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;
//LOG_DURATION("total with delay")
  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}

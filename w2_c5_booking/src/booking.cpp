#include <cstdint>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

class Hotel {
private:
  struct Order {
    int client;
    int rooms;
  };

  static const int SEC_IN_DAY = 86400;
  uint64_t _rooms_qty;
  queue<int64_t> _time_que;
  queue<Order> _client_que;
  map<int, int> _clients_qty;

  void DayDurationMaintenance(const int64_t& cur_time) {
    while (!_time_que.empty() && _time_que.front() <= (cur_time - SEC_IN_DAY)) {
      _rooms_qty -= _client_que.front().rooms;
      map<int, int>::iterator client_entries = _clients_qty.find(_client_que.front().client);
      if (--(*client_entries).second == 0) {
        _clients_qty.erase(client_entries);
      }
      _client_que.pop();
      _time_que.pop();
    }
  }

public:
  Hotel() : _rooms_qty(0) {}

  void Book(const int64_t& cur_time, const int& client_id, const int& room_count) {
    _time_que.push(cur_time);
    _client_que.push({client_id, room_count});
    _rooms_qty += room_count;
    map<int, int>::iterator client_entries = _clients_qty.find(client_id);
    if (client_entries == _clients_qty.end()) {
      client_entries = _clients_qty.emplace(client_id, 1).first;
    } else {
      ++(*client_entries).second;
    }
  }

  uint64_t Clients(const int64_t& cur_time) {
    DayDurationMaintenance(cur_time);
    return _clients_qty.size();
  }

  uint64_t Rooms(const int64_t& cur_time) {
    DayDurationMaintenance(cur_time);
    return _rooms_qty;
  }
};

class BookingMgr {
private:
  map<string, Hotel> _orders_base;
  int64_t _current_time;

public:
  BookingMgr() : _current_time(0) {}

  void Book(const int64_t& time, const string& hotel_name, const int& client_id, const int& room_count) {
    _current_time = time;
    _orders_base[hotel_name].Book(_current_time, client_id, room_count);
  }

  uint64_t Clients(const string& hotel_name) {
    return _orders_base[hotel_name].Clients(_current_time);
  }

  uint64_t Rooms(const string& hotel_name) {
    return _orders_base[hotel_name].Rooms(_current_time);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  BookingMgr manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time;
      string hotel_name;
      int client_id, room_count;
      cin >> time >> hotel_name >> client_id >> room_count;
      manager.Book(time, hotel_name, client_id, room_count);
    } else if (query_type == "CLIENTS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Clients(hotel_name) << '\n';
    } else if (query_type == "ROOMS") {
      string hotel_name;
      cin >> hotel_name;
      cout << manager.Rooms(hotel_name) << '\n';
    }
  }

  return 0;
}

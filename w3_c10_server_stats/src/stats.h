#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <set>
#include <array>
using namespace std;

class StatsField {
public:
  template <typename Container>
  StatsField(const string_view default_key, Container& known_keys) : default_key(default_key) {
    for(const auto& key : known_keys) {
      field_stats[key] = 0;
    }
    field_stats[default_key] = 0;
  }

  void Add(string_view field);
  const map<string_view, int>& GetStats() const {
    return field_stats;
  }
private:
  map<string_view, int> field_stats;
  const string_view default_key;
};

class Stats {
public:
  Stats() = default;
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;
private:
  const array<string, 4> known_methods{ "DELETE",
                                        "GET",
                                        "POST",
                                        "PUT" };
  const string default_method = "UNKNOWN";
  const array<string, 5> known_uris{ "/",
                                     "/basket",
                                     "/help",
                                     "/order",
                                     "/product" };
  const string default_uri = "unknown";
  StatsField methods_stats = {default_method, known_methods};
  StatsField uri_stats = {default_uri, known_uris};
  using Iterator = map<string_view, int>::iterator;
};

HttpRequest ParseRequest(string_view line);

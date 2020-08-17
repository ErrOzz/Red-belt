#include "stats.h"

#include <iostream>

using namespace std;

void StatsField::Add(string_view field) {
  if(auto it = field_stats.find(field); it != field_stats.end()) {
    ++it->second;
  } else {
    ++field_stats[default_key];
  }
}

void Stats::AddMethod(string_view method) {
  methods_stats.Add(method);
}
void Stats::AddUri(string_view uri) {
  uri_stats.Add(uri);
}
const map<string_view, int>& Stats::GetMethodStats() const {
  return methods_stats.GetStats();
}
const map<string_view, int>& Stats::GetUriStats() const {
  return uri_stats.GetStats();
}

void LeftStrip(string_view& sv) {
  size_t pref = sv.find_first_not_of(' ');
  sv.remove_prefix(pref);
}

string_view ReadToken(string_view& sv) {
  LeftStrip(sv);
  size_t pos = sv.find(' ');
  string_view result = sv.substr(0, pos);
  sv.remove_prefix(pos + 1);
  return result;
}

HttpRequest ParseRequest(string_view line) {
  string_view method = ReadToken(line);
  string_view uri = ReadToken(line);
  return {method, uri, ReadToken(line)};
}

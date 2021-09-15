#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <unordered_set>

using namespace std;

class Stats {
private:
    unordered_set<string_view> available_uris_ = {"/", "/order", "/product", "/basket", "/help"};
    unordered_set<string_view> available_methods_ = {"GET", "POST", "PUT", "DELETE"};

    map<string_view, int> method_stats_ = {{"GET", 0}, {"POST", 0}, {"PUT", 0}, {"DELETE", 0}, {"UNKNOWN", 0}};
    map<string_view, int> uri_stats_ = {{"/",        0},
                                        {"/order",   0},
                                        {"/product", 0},
                                        {"/basket",  0},
                                        {"/help",    0},
                                        {"unknown",  0}};
public:
    void AddMethod(string_view method);

    void AddUri(string_view uri);

    const map<string_view, int> &GetMethodStats() const;

    const map<string_view, int> &GetUriStats() const;
};

HttpRequest ParseRequest(string_view line);
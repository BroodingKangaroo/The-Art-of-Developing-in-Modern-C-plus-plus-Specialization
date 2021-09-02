#include "date.h"

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <set>
#include <list>

#pragma once

using namespace std;

struct Events {
    list<string> ordered;
    set<string> unique;

    Events(const string& event) {
        Insert(event);
    }

    void Insert(const string& event) {
        ordered.push_back(event);
        unique.insert(event);
    }

    bool Contains(const string& event) const {
        return unique.count(event);
    }

    const string& GetLast() const {
        return ordered.back();
    }


};

class Database {
public:
    void Add(const Date &date, const string &event);

    void Print(ostream &os) const;

    int RemoveIf(const std::function<bool(Date, string)> &predicate);

    vector<string> FindIf(const std::function<bool(Date, string)> &predicate) const;

    string Last(const Date &date) const;

private:
    map<Date, Events> database;
};
#include "date.h"

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <set>
#include <list>

#pragma once

using namespace std;

class Events {
public:
    Events(const string &event);

    void Insert(const string &event);

    bool Contains(const string &event) const;

    const string &GetLast() const;

    bool Empty() const;

    const set<string> &GetUnique() const;

    const list<string> &GetOrdered() const;

    void Erase(list<string>::iterator iterator);

    list<string>::iterator Begin();

    list<string>::iterator End();

private:
    list<string> ordered_;
    set<string> unique_;
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
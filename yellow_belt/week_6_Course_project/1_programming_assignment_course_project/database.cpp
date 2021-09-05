#include "database.h"

#include <algorithm>

void Database::Add(const Date &date, const string &event) {
    if (database.count(date)) {
        auto &events = database.at(date);
        if (events.Contains(event)) return;
        events.Insert(event);
    } else {
        database.insert({date, Events(event)});
    }
}

void Database::Print(ostream &os) const {
    for (const auto&[date, events]: database) {
        for (const auto &event: events.GetOrdered()) {
            os << date << " " << event << endl;
        }
    }
}

string Database::Last(const Date &date) const {
    ostringstream os;
    auto upperBoundResult = database.upper_bound(date);
    if (upperBoundResult == database.begin()) {
        os << "No entries";
    } else {
        auto&[prevDate, events] = *prev(upperBoundResult);
        os << prevDate << " " << events.GetLast();
    }
    return os.str();
}

int Database::RemoveIf(const function<bool(Date, string)> &predicate) {
    int count = 0;

    for (auto databaseIterator = database.begin(); databaseIterator != database.end();) {
        auto &date = databaseIterator->first;
        auto &events = databaseIterator->second;
        for (auto eventsIterator = events.Begin(); eventsIterator != events.End();) {
            if (predicate(date, *eventsIterator)) {
                count++;
                databaseIterator->second.Erase(eventsIterator++);
            } else {
                eventsIterator++;
            }
        }
        if (events.Empty()) {
            database.erase(databaseIterator++);
        } else {
            ++databaseIterator;
        }
    }
    return count;
}

vector<string> Database::FindIf(const function<bool(Date, string)> &predicate) const {
    vector<string> result;

    for (const auto&[date, events]: database) {
        for (const auto &event: events.GetOrdered()) {
            if (predicate(date, event)) {
                ostringstream os;
                os << date << " " << event;
                result.push_back(os.str());
            }
        }
    }
    return result;
}

Events::Events(const string &event) {
    Insert(event);
}

void Events::Insert(const string &event) {
    ordered_.push_back(event);
    unique_.insert(event);
}

bool Events::Contains(const string &event) const {
    return unique_.count(event);
}

const string &Events::GetLast() const {
    return ordered_.back();
}

list<string>::iterator Events::Begin() {
    return ordered_.begin();
}

list<string>::iterator Events::End() {
    return ordered_.end();
}

void Events::Erase(list<string>::iterator iterator) {
    unique_.erase(*iterator);
    ordered_.erase(iterator);
}

const list<string> &Events::GetOrdered() const {
    return ordered_;
}

const set<string> &Events::GetUnique() const {
    return unique_;
}

bool Events::Empty() const {
    return unique_.empty();
}

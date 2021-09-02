#include "database.h"

#include <algorithm>

void Database::Add(const Date &date, const string &event) {
    if (database.count(date)) {
        auto& events = database.at(date);
        if (events.Contains(event)) return;
        events.Insert(event);
    } else {
        database.insert({date, Events(event)});
    }
}

void Database::Print(ostream &os) const {
    for (const auto&[date, events]: database) {
        for (const auto &event: events.ordered) {
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
        auto entry = *prev(upperBoundResult);
        os << entry.first << " " << entry.second.GetLast();
    }
    return os.str();
}

int Database::RemoveIf(const function<bool(Date, string)> &predicate) {
    int count = 0;

    for (auto databaseIterator = database.begin(); databaseIterator != database.end();) {
        auto date = databaseIterator->first;
        for (auto eventsIterator = databaseIterator->second.ordered.begin();
             eventsIterator != databaseIterator->second.ordered.end();) {
            if (predicate(date, *eventsIterator)) {
                count++;
                databaseIterator->second.unique.erase(*eventsIterator);
                databaseIterator->second.ordered.erase(eventsIterator++);
            } else {
                eventsIterator++;
            }
        }
        if (databaseIterator->second.unique.empty()) {
            database.erase(databaseIterator++);
        } else {
            ++databaseIterator;
        }
    }
    return count;
}

vector<string> Database::FindIf(const function<bool(Date, string)> &predicate) const {
    vector<string> result;

    for (const auto& [date, events] : database) {
        for (const auto& event : events.ordered) {
            if (predicate(date, event)) {
                ostringstream os;
                os << date << " " << event;
                result.push_back(os.str());
            }
        }
    }
    return result;
}

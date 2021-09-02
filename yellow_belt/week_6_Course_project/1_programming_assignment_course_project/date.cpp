#include "date.h"

#include <iomanip>

Date ParseDate(istream &is) {
    string dateString;
    is >> dateString;
    int firstHyphen = dateString.find('-');
    int secondHyphen = dateString.find('-', firstHyphen + 1);
    int year = stoi(dateString.substr(0, firstHyphen));
    int month = stoi(dateString.substr(firstHyphen + 1, secondHyphen));
    int day = stoi(dateString.substr(secondHyphen + 1, dateString.length()));
    return Date(year, month, day);
}

int Date::GetYear() const {
    return year_;
}

int Date::GetMonth() const {
    return month_;
}

int Date::GetDay() const {
    return day_;
}

ostream &operator<<(ostream &os, const Date &date) {
    os.fill('0');
    os << setw(4) << date.year_ << "-"
       << setw(2) << date.month_ << "-"
       << setw(2) << date.day_;
    return os;
}

bool Date::operator<(const Date &rhs) const {
    if (year_ != rhs.GetYear()) {
        return year_ < rhs.GetYear();
    } else if (month_ != rhs.GetMonth()) {
        return month_ < rhs.GetMonth();
    } else {
        return day_ < rhs.GetDay();
    }
}

bool Date::operator==(const Date &rhs) const {
    return year_ == rhs.GetYear() && month_ == rhs.GetMonth() && day_ == rhs.GetDay();
}

bool Date::operator!=(const Date &rhs) const {
    return !operator==(rhs);
}

bool Date::operator<=(const Date &rhs) const {
    return operator==(rhs) || operator<(rhs);
}

bool Date::operator>(const Date &rhs) const {
    return !operator<=(rhs);
}

bool Date::operator>=(const Date &rhs) const {
    return !operator<(rhs);
}
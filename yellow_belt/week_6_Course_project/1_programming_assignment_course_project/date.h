#include <string>
#include <sstream>

#pragma once

using namespace std;


class Date {
public:
    Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

    bool operator<(const Date &rhs) const;

    bool operator==(const Date &rhs) const;

    bool operator!=(const Date &rhs) const;

    bool operator<=(const Date &rhs) const;

    bool operator>(const Date &rhs) const;

    bool operator>=(const Date &rhs) const;

    int GetYear() const;

    int GetMonth() const;

    int GetDay() const;

private:
    const int year_;
    const int month_;
    const int day_;

    friend ostream &operator<<(ostream &os, const Date &date);
};

Date ParseDate(istream &is);
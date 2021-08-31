#include <iostream>
#include <string>
#include <map>

using namespace std;

class Person {
public:
    void ChangeFirstName(int year, const string &first_name) {
        _first_names[year] = first_name;
    }

    void ChangeLastName(int year, const string &last_name) {
        _last_names[year] = last_name;
    }

    string GetFullName(int year) {
        auto first_name_upper_bound = _first_names.upper_bound(year);
        auto last_name_upper_bound = _last_names.upper_bound(year);

        if (first_name_upper_bound == _first_names.begin() && last_name_upper_bound == _last_names.begin()) {
            return "Incognito";
        } else {
            if (first_name_upper_bound == _first_names.begin()) {
                auto last_name = prev(last_name_upper_bound);
                return last_name->second + " with unknown first name";
            } else if (last_name_upper_bound == _last_names.begin()) {
                auto first_name = prev(first_name_upper_bound);
                return first_name->second + " with unknown last name";
            } else {
                auto first_name = prev(first_name_upper_bound);
                auto last_name = prev(last_name_upper_bound);
                return first_name->second + " " + last_name->second;
            }
        }
    }

private:
    map<int, string> _first_names;
    map<int, string> _last_names;
};

int main() {
    Person person;

    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year: {1900, 1965, 1990}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeFirstName(1970, "Appolinaria");
    for (int year: {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    person.ChangeLastName(1968, "Volkova");
    for (int year: {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }

    return 0;
}

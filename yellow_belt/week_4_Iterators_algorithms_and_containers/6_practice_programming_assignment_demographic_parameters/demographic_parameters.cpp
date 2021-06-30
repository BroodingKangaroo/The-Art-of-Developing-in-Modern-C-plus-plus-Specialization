#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

enum class Gender {
    FEMALE,
    MALE
};


struct Person {
    int age;  // возраст
    Gender gender;  // пол
    bool is_employed;  // имеет ли работу
};

// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template<typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(
            begin(range_copy), middle, end(range_copy),
            [](const Person &lhs, const Person &rhs) {
                return lhs.age < rhs.age;
            }
    );
    return middle->age;
}

template<typename UnaryPredicate>
vector<Person> getPersonsByPredicate(vector<Person> persons, UnaryPredicate predicate) {
    auto it = partition(persons.begin(), persons.end(), predicate);
    persons.erase(it, persons.end());
    return persons;
}


/*
// Преобразуем порядок людей к следующему:
//                  persons
//                 /      \
    //          females        males
//         /       \     /      \
    //      empl.  unempl. empl.   unempl.
auto females_end = partition(
        begin(persons), end(persons), [](const Person& p) {
            return p.gender == Gender::FEMALE;
        }
);
auto employed_females_end = partition(
        begin(persons), females_end, [](const Person& p) {
            return p.is_employed;
        }
);
auto employed_males_end = partition(
        females_end, end(persons), [](const Person& p) {
            return p.is_employed;
        }
);
*/

void PrintStats(vector<Person> persons) {
    auto females = getPersonsByPredicate(persons, [](const Person& person) {
        return person.gender == Gender::FEMALE;
    });
    auto males = getPersonsByPredicate(persons, [](const Person& person) {
        return person.gender == Gender::MALE;
    });
    auto employed_females = getPersonsByPredicate(persons, [](const Person& person) {
        return person.gender == Gender::FEMALE && person.is_employed;
    });
    auto unemployed_females = getPersonsByPredicate(persons, [](const Person& person) {
        return person.gender == Gender::FEMALE && !person.is_employed;
    });
    auto employed_males = getPersonsByPredicate(persons, [](const Person& person) {
        return person.gender == Gender::MALE && person.is_employed;
    });
    auto unemployed_males = getPersonsByPredicate(persons, [](const Person& person) {
        return person.gender == Gender::MALE && !person.is_employed;
    });
    cout << "Median age = " << ComputeMedianAge(persons.begin(), persons.end()) << endl;
    cout << "Median age for females = " << ComputeMedianAge(females.begin(), females.end()) << endl;
    cout << "Median age for males = " << ComputeMedianAge(males.begin(), males.end()) << endl;
    cout << "Median age for employed females = " << ComputeMedianAge(employed_females.begin(), employed_females.end()) << endl;
    cout << "Median age for unemployed females = " << ComputeMedianAge(unemployed_females.begin(), unemployed_females.end()) << endl;
    cout << "Median age for employed males = " << ComputeMedianAge(employed_males.begin(), employed_males.end()) << endl;
    cout << "Median age for unemployed males = " << ComputeMedianAge(unemployed_males.begin(), unemployed_males.end()) << endl;
}


int main() {
    vector<Person> persons = {
            {31, Gender::MALE,   false},
            {40, Gender::FEMALE, true},
            {24, Gender::MALE,   true},
            {20, Gender::FEMALE, true},
            {80, Gender::FEMALE, false},
            {78, Gender::MALE,   false},
            {10, Gender::FEMALE, false},
            {55, Gender::MALE,   true},
    };
    PrintStats(persons);
    return 0;
}
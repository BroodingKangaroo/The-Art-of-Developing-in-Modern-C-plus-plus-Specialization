#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

//enum class Gender {
//    FEMALE,
//    MALE
//};


//struct Person {
//    int age;  // возраст
//    Gender gender;  // пол
//    bool is_employed;  // имеет ли работу
//};


//// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
//template<typename InputIt>
//int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
//    if (range_begin == range_end) {
//        return 0;
//    }
//    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
//    auto middle = begin(range_copy) + range_copy.size() / 2;
//    nth_element(
//            begin(range_copy), middle, end(range_copy),
//            [](const Person& lhs, const Person& rhs) {
//                return lhs.age < rhs.age;
//            }
//    );
//    return middle->age;
//}

void PrintMessage(int median_age, const string& employment = "", const string& gender = "") {
    cout << "Median age for " << (!employment.empty() ? employment + " " : "") << gender << " = " << median_age << endl;
}

void PrintStats(vector<Person> persons) {
    cout << "Median age = " << ComputeMedianAge(persons.begin(), persons.end()) << endl;
    auto females = std::partition(persons.begin(), persons.end(), [](Person& person) {
        return person.gender == Gender::FEMALE;
    });
    auto employed_females = std::partition(persons.begin(), females, [](Person& person) {
        return person.is_employed;
    });
    auto employed_males = std::partition(females, persons.end(), [](Person& person) {
        return person.is_employed;
    });
    PrintMessage(ComputeMedianAge(persons.begin(), females), "", "females");
    PrintMessage(ComputeMedianAge(females, persons.end()), "", "males");
    PrintMessage(ComputeMedianAge(persons.begin(), employed_females), "employed", "females");
    PrintMessage(ComputeMedianAge(employed_females, females), "unemployed", "females");
    PrintMessage(ComputeMedianAge(females, employed_males), "employed", "males");
    PrintMessage(ComputeMedianAge(employed_males, persons.end()), "unemployed", "males");
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

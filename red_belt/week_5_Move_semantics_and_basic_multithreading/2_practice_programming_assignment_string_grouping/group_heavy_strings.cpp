#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <list>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template<typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template<typename String>
using Char = typename String::value_type;

template<typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
    map<String, Group<String>> groupsByUniqueChars;
    for (String &str: strings) {
        String unique_chars = str;
        sort(unique_chars.begin(), unique_chars.end());
        unique_chars.erase(unique(unique_chars.begin(), unique_chars.end()), unique_chars.end());

        groupsByUniqueChars[move(unique_chars)].push_back(move(str));
    }

    vector<Group<String>> result;
    result.reserve(groupsByUniqueChars.size());
    for (auto&[k, v]: groupsByUniqueChars) {
        result.push_back(move(v));
    }


    return move(result);
}


void TestGroupingABC() {
    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 2);
    sort(begin(groups), end(groups));  // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
    ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
    vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
    auto groups = GroupHeavyStrings(strings);
    ASSERT_EQUAL(groups.size(), 4);
    sort(begin(groups), end(groups));  // Порядок групп не имеет значения
    ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
    ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
    ASSERT_EQUAL(groups[2], vector<string>({"port"}));
    ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestGroupingABC);
    RUN_TEST(tr, TestGroupingReal);
    return 0;
}
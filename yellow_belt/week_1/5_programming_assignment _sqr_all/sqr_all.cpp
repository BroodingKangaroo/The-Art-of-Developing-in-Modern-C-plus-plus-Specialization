#include <vector>
#include <iostream>
#include <map>

using namespace std;

template<typename First, typename Second>
pair<First, Second> operator*(pair<First, Second> &, pair<First, Second> &);

template<typename Key, typename Value>
map<Key, Value> operator*(map<Key, Value> &, map<Key, Value> &);

template<typename T>
vector<T> operator*(vector<T> &, vector<T> &);

template<typename T>
T Sqr(T);

template<typename First, typename Second>
pair<First, Second> operator*(pair<First, Second> &lhs, pair<First, Second> &rhs) {
    return make_pair(lhs.first * rhs.first, lhs.second * rhs.second);
}

template<typename Key, typename Value>
map<Key, Value> operator*(map<Key, Value> &lhs, map<Key, Value> &rhs) {
    map<Key, Value> result;
    for (const auto&[key, value] : lhs) {
        result[key] = lhs.at(key) * rhs.at(key);
    }
    return result;
}

template<typename T>
vector<T> operator*(vector<T> &lhs, vector<T> &rhs) {
    vector<T> result;
    result.reserve(lhs.size());
    for (size_t i = 0; i < lhs.size(); i++) {
        result.push_back(lhs.at(i) * rhs.at(i));
    }
    return result;
}

template<typename T>
T Sqr(T container) {
    T result = container * container;
    return result;
}

int main() {
// Пример вызова функции
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
        cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
            {4, {2, 2}},
            {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto &x : Sqr(map_of_pairs)) {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
    return 0;
}





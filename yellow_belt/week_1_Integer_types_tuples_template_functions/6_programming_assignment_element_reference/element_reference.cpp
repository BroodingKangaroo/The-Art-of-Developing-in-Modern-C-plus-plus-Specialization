#include <string>
#include <map>
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename Key, typename Value>
Value& GetRefStrict(map<Key, Value>& collection, const Key& key) {
    if (collection.count(key)) {
        return collection[key];
    }
    throw runtime_error("");
}

int main() {
    map<int, string> m = {{0, "value"}};
    string &item = GetRefStrict(m, 1);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue
}
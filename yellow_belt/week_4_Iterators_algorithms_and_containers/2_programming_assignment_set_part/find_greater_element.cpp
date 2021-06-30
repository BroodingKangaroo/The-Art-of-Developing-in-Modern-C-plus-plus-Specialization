#include <set>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
vector<T> FindGreaterElements(const set<T> &elements, const T &border) {
    return {upper_bound(elements.begin(), elements.end(), border), elements.end()};
}

int main() {
    auto result = FindGreaterElements(set<int>{1, 5, 7, 8}, 5);
    for (int x : result) {
        cout << x << " ";
    }
    cout << endl;

    string to_find = "Python";
    cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
    return 0;
}

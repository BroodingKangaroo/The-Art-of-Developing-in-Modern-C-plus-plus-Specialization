#include <algorithm>
#include <iostream>

using namespace std;

template<typename T>
void printVector(vector<T> vec) {
    for (auto elem : vec) cout << elem << ' ';
    cout << endl;
}

int main() {
    int n;
    cin >> n;

    vector<int> vec;
    for (int i = n; i >= 1; i--) vec.push_back(i);

    // another possible way to fill in the vector with increasing values:
    // vector<int> vec(n);
    // iota(vec.rbegin(), vec.rend(), 1);

    printVector(vec);
    while (prev_permutation(vec.begin(), vec.end())) {
        printVector(vec);
    }

    return 0;
}
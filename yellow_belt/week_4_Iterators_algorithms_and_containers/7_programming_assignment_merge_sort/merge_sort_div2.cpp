#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 >= range_end) return;
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto middle_iterator = elements.begin() + elements.size() / 2;
    MergeSort(elements.begin(), middle_iterator);
    MergeSort(middle_iterator, elements.end());
    merge(elements.begin(), middle_iterator, middle_iterator, elements.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_begin + 1 >= range_end) return;
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto middle1_iterator = elements.begin() + elements.size() / 3;
    auto middle2_iterator = middle1_iterator + elements.size() / 3;
    MergeSort(elements.begin(), middle1_iterator);
    MergeSort(middle1_iterator, middle2_iterator);
    MergeSort(middle2_iterator, elements.end());
    vector<typename RandomIt::value_type>buffer;
    merge(elements.begin(), middle1_iterator, middle1_iterator, middle2_iterator, back_inserter(buffer));
    merge(buffer.begin(), buffer.end(), middle2_iterator, elements.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}

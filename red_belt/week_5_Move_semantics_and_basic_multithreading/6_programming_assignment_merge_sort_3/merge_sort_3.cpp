#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template<typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    size_t range_size = range_end - range_begin;
    if (range_size < 2) return;
    vector<typename RandomIt::value_type> range(make_move_iterator(range_begin), make_move_iterator(range_end));


    MergeSort(range.begin(), range.begin() + range_size / 3);
    MergeSort(range.begin() + range_size / 3, range.begin() + 2 * range_size / 3);
    MergeSort(range.begin() + 2 * range_size / 3, range.end());

    vector<typename RandomIt::value_type> first_part;
    merge(
            make_move_iterator(range.begin()), make_move_iterator(range.begin() + range_size / 3),
            make_move_iterator(range.begin() + range_size / 3), make_move_iterator(range.begin() + 2 * range_size / 3),
            back_inserter(first_part)
    );
    merge(
            make_move_iterator(first_part.begin()), make_move_iterator(first_part.end()),
            make_move_iterator(range.begin() + 2 * range_size / 3), make_move_iterator(range.end()),
            range_begin
    );
}

void TestIntVector() {
    vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
    MergeSort(begin(numbers), end(numbers));
    ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    return 0;
}

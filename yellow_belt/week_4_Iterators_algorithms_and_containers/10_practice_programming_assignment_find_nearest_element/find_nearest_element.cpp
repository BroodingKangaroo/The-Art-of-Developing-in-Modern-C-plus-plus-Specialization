#include <iostream>
#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(
        const set<int>& numbers,
        int border) {
    if (numbers.empty()) return numbers.begin();
    auto greaterOrEqualIterator = numbers.lower_bound(border);
    if (abs(*greaterOrEqualIterator - border) < abs(*prev(greaterOrEqualIterator) - border)) {
        return greaterOrEqualIterator;
    } else {
        return prev(greaterOrEqualIterator);
    }
}
// set<int>::const_iterator —
// тип итераторов для константного множества целых чисел

int main() {

    set<int> numbers = {1, 4, 6};
    cout <<
         *FindNearestElement(numbers, 0) << " " <<
         *FindNearestElement(numbers, 3) << " " <<
         *FindNearestElement(numbers, 5) << " " <<
         *FindNearestElement(numbers, 6) << " " <<
         *FindNearestElement(numbers, 100) << endl;

    set<int> empty_set;

    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
    return 0;
}
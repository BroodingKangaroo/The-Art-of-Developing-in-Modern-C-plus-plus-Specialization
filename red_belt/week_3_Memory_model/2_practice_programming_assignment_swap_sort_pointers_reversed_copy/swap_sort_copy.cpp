#include "test_runner.h"

#include <algorithm>
#include <numeric>

using namespace std;

template<typename T>
void Swap(T *first, T *second) {
    swap(*first, *second);
}

template<typename T>
void SortPointers(vector<T *> &pointers) {
    std::sort(pointers.begin(), pointers.end(), [](T *lhs, T *rhs) { return *lhs < *rhs; });
}

template<typename T>
void ReversedCopy(T *source, size_t count, T *destination) {
    T *source_left = source, *source_right = source + count, *destination_left = destination, *destination_right =
            destination + count;
    size_t intersection_count = count;
    if (destination_left >= source_left && destination_left < source_right) {
        intersection_count = (destination_right < source_right) ? count : (source_right - destination_left);
    } else if (destination_left <= source_left && destination_right > source_left) {
        intersection_count = destination_right - source_left;
    }


    intersection_count /= 2;
    for (size_t i = 0; i < count; i++) {
        if (!(destination + i >= source && destination + i < source + count)) {
            *(destination + i) = *(source + count - i - 1);
        } else if ((destination + i >= source && destination + i < source + count) && intersection_count > 0) {
            swap(*(destination + i), *(source + count - i - 1));
            intersection_count--;
        }
    }
}

void TestSwap() {
    int a = 1;
    int b = 2;
    Swap(&a, &b);
    ASSERT_EQUAL(a, 2);
    ASSERT_EQUAL(b, 1);

    string h = "world";
    string w = "hello";
    Swap(&h, &w);
    ASSERT_EQUAL(h, "hello");
    ASSERT_EQUAL(w, "world");
}

void TestSortPointers() {
    int one = 1;
    int two = 2;
    int three = 3;

    vector<int *> pointers;
    pointers.push_back(&two);
    pointers.push_back(&three);
    pointers.push_back(&one);

    SortPointers(pointers);

    ASSERT_EQUAL(pointers.size(), 3u);
    ASSERT_EQUAL(*pointers[0], 1);
    ASSERT_EQUAL(*pointers[1], 2);
    ASSERT_EQUAL(*pointers[2], 3);
}

void TestReverseCopyDestinationInside() {
    const size_t count = 10;

    int *source = new int[count];

    for (size_t i = 0; i < count; ++i) {
        source[i] = i + 1;
    }

    // Области памяти могут перекрываться
    ReversedCopy(source, count - 5, source + 3);
    const vector<int> expected2 = {1, 2, 3, 5, 4, 3, 2, 1, 9, 10};
    ASSERT_EQUAL(vector<int>(source, source + count), expected2);

    delete[] source;
}

void TestReverseCopy() {
    const size_t count = 7;

    int *source = new int[count];
    int *dest = new int[count];

    for (size_t i = 0; i < count; ++i) {
        source[i] = i + 1;
    }
    ReversedCopy(source, count, dest);
    const vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
    ASSERT_EQUAL(vector<int>(dest, dest + count), expected1);

    // Области памяти могут перекрываться
    ReversedCopy(source, count - 1, source + 1);
    const vector<int> expected2 = {1, 6, 5, 4, 3, 2, 1};
    ASSERT_EQUAL(vector<int>(source, source + count), expected2);

    delete[] dest;
    delete[] source;
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestSortPointers);
    RUN_TEST(tr, TestReverseCopy);
    RUN_TEST(tr, TestReverseCopyDestinationInside);
    return 0;
}
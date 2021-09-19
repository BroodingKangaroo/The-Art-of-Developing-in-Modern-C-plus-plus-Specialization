#include "test_runner.h"
#include "profile.h"

#include <vector>
#include <future>
#include <numeric>

using namespace std;


template<typename T>
class Page {
public:
    Page(T &begin, T &end) : begin_(begin), end_(end) {}

    size_t size() const {
        return end_ - begin_;
    }

    auto begin() const {
        return begin_;
    }

    auto end() const {
        return end_;
    }

private:
    T begin_, end_;
};

template<typename Iterator>
class Paginator {
public:

    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (auto it = begin; it < end; it += page_size) {
            auto first = it;
            auto last = it + page_size;
            if (last >= end) last = end;
            pages.push_back(Page(first, last));
        }
    }

    size_t size() const {
        return pages.size();
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }

private:
    vector<Page<Iterator>> pages;
};

template<typename C>
auto Paginate(C &c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}


int64_t CalculateMatrixSum(const vector<vector<int>> &matrix) {
    size_t matrix_size = matrix.size();

    vector<future<int64_t>> futures;
    size_t page_size = (matrix_size > 4) ? 4 : matrix_size;

    for (auto &page: Paginate(matrix, matrix_size / page_size)) {
        futures.push_back(
                async([page] {
                    int64_t result = 0;
                    for (auto &row: page) {
                        result += accumulate(row.begin(), row.end(), int64_t(0));
                    }
                    return result;
                })
        );
    }

    int64_t result = 0;
    for (auto &future: futures) {
        result += future.get();
    }
    return result;
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
            {1}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 1);
}

auto GenerateBigMatrix(size_t matrix_size) {
    vector<vector<int>> matrix(matrix_size);
    int64_t sum = 0;
    for (auto &row: matrix) {
        row.reserve(matrix_size);
        for (int i = 0; i < matrix_size; i++) {
            int elem = rand() % 100;
            row.push_back(elem);
            sum += elem;
        }
    }
    return pair{matrix, sum};
}

void TestBigMatrix() {
    size_t matrix_size = 8000;
    const auto& [matrix, sum] = GenerateBigMatrix(matrix_size);
    {
        LOG_DURATION("Multiple thread")
        ASSERT_EQUAL(CalculateMatrixSum(matrix), sum);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
    RUN_TEST(tr, TestBigMatrix);
}
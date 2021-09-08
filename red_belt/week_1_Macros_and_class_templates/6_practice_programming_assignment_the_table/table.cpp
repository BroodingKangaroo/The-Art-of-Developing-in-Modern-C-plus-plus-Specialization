#include "test_runner.h"

using namespace std;

template<typename T>
class Table {
public:
    Table(size_t row_count, size_t column_count) : row_count_(row_count), column_count_(column_count) {
        Resize(row_count_, column_count_);
    }

    void Resize(size_t row_count, size_t column_count) {
        row_count_ = row_count;
        column_count_ = column_count;
        if (!row_count_ || !column_count_) {
            row_count_ = 0;
            column_count_ = 0;
        }
        table_.resize(row_count_);
        for (auto &row: table_) {
            row.resize(column_count_);
        }
    }

    const vector<T> &operator[](size_t index) const {
        return table_[index];
    }

    vector<T> &operator[](size_t index) {
        return table_[index];
    }

    pair<size_t, size_t> Size() const {
        return {row_count_, column_count_};
    }

private:
    size_t row_count_;
    size_t column_count_;
    vector<vector<T>> table_;
};


void TestTable() {
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
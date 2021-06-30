#include <iostream>
#include <vector>

using namespace std;

class Matrix {
    int _num_rows;
    int _num_columns;

    std::vector<std::vector<int>> _matrix;

    static void check_bounds(const int &num_rows, const int &num_columns) {
        if (num_rows < 0 || num_columns < 0) {
            throw std::out_of_range("The parameters you have provided are incorrect.");
        }
    }

    void check_position_bounds(const int &row_position, const int &column_position) const {
        if (row_position >= _num_rows || column_position >= _num_columns || row_position < 0 || column_position < 0) {
            throw std::out_of_range("The parameters you have provided are incorrect.");
        }
    }

public:
    Matrix() : _num_rows(0), _num_columns(0) {}

    Matrix(int num_rows, int num_columns) {
        check_bounds(num_rows, num_columns);

        _num_rows = num_rows;
        _num_columns = num_columns;
        _matrix = std::vector<std::vector<int>>(_num_rows, std::vector<int>(_num_columns));
    }

    void Reset(int num_rows_new, int num_columns_new) {
        check_bounds(num_rows_new, num_columns_new);

        _num_rows = num_rows_new;
        _num_columns = num_columns_new;
        _matrix.resize(num_rows_new);
        for (auto &row : _matrix) {
            row.resize(_num_columns);
            for (auto &element : row) {
                element = 0;
            }
        }
    }

    int At(int row_number, int column_number) const {
        check_position_bounds(row_number, column_number);
        return _matrix[row_number][column_number];
    }

    int &At(int row_number, int column_number) {
        check_position_bounds(row_number, column_number);
        return _matrix[row_number][column_number];
    }

    int GetNumRows() const {
        return _num_rows;
    }

    int GetNumColumns() const {
        return _num_columns;
    }

    std::istream &operator>>(std::istream &in) {
        int num_rows, num_columns;
        in >> num_rows >> num_columns;

        this->Reset(num_rows, num_columns);
        for (int row = 0; row < num_rows; ++row) {
            for (int column = 0; column < num_columns; ++column) {
                in >> At(row, column);
            }
        }

        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);

    friend std::istream &operator>>(std::istream &in, Matrix &matrix);

    friend bool operator==(const Matrix &lhs, const Matrix &rhs);
};

std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
    out << matrix.GetNumRows() << " " << matrix.GetNumColumns() << "\n";
    for (int i = 0; i < matrix.GetNumRows(); i++) {
        for (int j = 0; j < matrix.GetNumColumns() - 1; j++) {
            out << matrix.At(i, j) << " ";
        }
        out << matrix.At(i, matrix.GetNumColumns() - 1) << "\n";
    }
    return out;
}

std::istream &operator>>(std::istream &in, Matrix &matrix) {
    int num_rows, num_columns;
    in >> num_rows >> num_columns;
    matrix.Reset(num_rows, num_columns);
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            in >> matrix.At(i, j);
        }
    }
    return in;
}

bool operator==(const Matrix &lhs, const Matrix &rhs) {
    if ((lhs.GetNumRows() == 0 || lhs.GetNumColumns() == 0) && (rhs.GetNumRows() == 0 || rhs.GetNumColumns() == 0))
        return true;
    if (lhs.GetNumRows() != rhs.GetNumRows() || lhs.GetNumColumns() != rhs.GetNumColumns())
        return false;
    for (int i = 0; i < lhs.GetNumRows(); i++) {
        for (int j = 0; j < lhs.GetNumColumns(); j++) {
            if (lhs.At(i, j) != rhs.At(i, j)) {
                return false;
            }
        }
    }
    return true;
}

Matrix operator+(const Matrix &lhs, const Matrix &rhs) {
    if ((lhs.GetNumRows() == 0 || lhs.GetNumColumns() == 0) && (rhs.GetNumRows() == 0 || rhs.GetNumColumns() == 0))
        return Matrix();
    if (lhs.GetNumRows() != rhs.GetNumRows() || lhs.GetNumColumns() != rhs.GetNumColumns()) {
        throw invalid_argument("Invalid argument");
    }
    int num_rows = lhs.GetNumRows();
    int num_columns = lhs.GetNumColumns();
    Matrix matrix(num_rows, num_columns);
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            matrix.At(i, j) = lhs.At(i, j) + rhs.At(i, j);
        }
    }
    return matrix;
}

int main() {
    return 0;
}
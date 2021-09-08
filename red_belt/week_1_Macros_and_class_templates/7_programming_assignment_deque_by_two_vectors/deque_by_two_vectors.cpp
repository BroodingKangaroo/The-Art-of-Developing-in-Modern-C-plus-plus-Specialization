#include <vector>
#include <iostream>

using namespace std;

template<typename T>
class Deque {
public:
    size_t Size() const {
        return left_.size() + right_.size();
    }

    bool Empty() const {
        return left_.empty() && right_.empty();
    }

    const T& operator[] (size_t index) const {
        return AtImpl(index);
    }

    T& operator[] (size_t index) {
        return AtImpl(index);
    }

    const T& At(size_t index) const {
        checkIndex(index);
        return AtImpl(index);
    }

    T& At(size_t index) {
        checkIndex(index);
        return AtImpl(index);
    }

    T& Front() {
        return (left_.empty()) ? right_.front() : left_.back();
    }

    const T& Front() const {
        return (left_.empty()) ? right_.front() : left_.back();
    }

    T& Back() {
        return (right_.empty()) ? left_.front() : right_.back();
    }

    const T& Back() const {
        return (right_.empty()) ? left_.front() : right_.back();
    }

    void PushFront(const T& elem) {
        left_.push_back(elem);
    }

    void PushBack(const T& elem) {
        right_.push_back(elem);
    }

private:

    const T& AtImpl(size_t index) const {
        return (index < left_.size()) ? left_[left_.size() - index - 1] : right_[index - left_.size()];
    }

    T& AtImpl(size_t index) {
        return (index < left_.size()) ? left_[left_.size() - index - 1] : right_[index - left_.size()];
    }

    void checkIndex(size_t index) const {
        if (index > this->Size()){
            throw out_of_range("Index is out of range");
        }
    }

    vector<T> left_;
    vector<T> right_;
};

int main() {
    Deque<int> d;

    d.PushBack(1);
    d.PushBack(2);
    d.PushBack(3);
    d.PushBack(4);
    d.PushBack(5);
    d.PushFront(-1);
    d.PushFront(-2);
    d[0] = 5;
    cout << d[0];

    return 0;
}
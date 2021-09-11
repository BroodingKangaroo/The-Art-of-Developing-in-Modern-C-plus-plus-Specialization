#pragma once

#include <cstdlib>
#include <algorithm>

template<typename T>
class SimpleVector {
public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit SimpleVector(size_t size) : data_(new T[size]), size_(size), capacity_(size) {}

    ~SimpleVector() {
        delete[] data_;
    }

    T &operator[](size_t index) {
        return data_[index];
    }

    T *begin() {
        return data_;
    }

    T *end() {
        return data_ + size_;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(const T &value) {
        if (size_ == capacity_) {
            IncreaseCapacity();
        }
        data_[size_++] = value;
    }

private:

    void IncreaseCapacity() {
        capacity_ = (capacity_) ? capacity_ * 2 : 1;
        T *tmp = new T[capacity_];
        std::copy(begin(), end(), tmp);
        delete [] data_;
        data_ = tmp;
    }

    T *data_;
    size_t size_;
    size_t capacity_;
};
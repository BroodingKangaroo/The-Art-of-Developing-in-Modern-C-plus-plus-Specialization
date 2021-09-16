#pragma once

#include <cstdlib>
#include <algorithm>
#include <utility>

template<typename T>
class SimpleVector {
public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit SimpleVector(size_t size) : data_(new T[size]), size_(size), capacity_(size) {}

    ~SimpleVector() {
        delete[] data_;
    }

    SimpleVector(const SimpleVector &rhs) :
            data_(new T[rhs.capacity_]),
            size_(rhs.size_),
            capacity_(rhs.capacity_) {
        std::copy(rhs.begin(), rhs.end(), begin());
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (rhs.size_ <= capacity_) {
            // У нас достаточно памяти - просто копируем элементы
            std::copy(rhs.begin(), rhs.end(), begin());
            size_ = rhs.size_;
        } else {
            // Это так называемая идиома copy-and-swap.
            // Мы создаём временный вектор с помощью
            // конструктора копирования, а затем обмениваем его поля со своими.
            // Так мы достигаем двух целей:
            //  - избегаем дублирования кода в конструкторе копирования
            //    и операторе присваивания
            //  - обеспечиваем согласованное поведение конструктора копирования
            //    и оператора присваивания
            SimpleVector<T> tmp(rhs);
            std::swap(tmp.data_, data_);
            std::swap(tmp.size_, size_);
            std::swap(tmp.capacity_, capacity_);
        }
        return *this;
    }

    T &operator[](size_t index) {
        return data_[index];
    }

    const T *begin() const {
        return data_;
    }

    const T *end() const {
        return data_ + size_;
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
        delete[] data_;
        data_ = tmp;
    }

    T *data_;
    size_t size_{};
    size_t capacity_{};
};
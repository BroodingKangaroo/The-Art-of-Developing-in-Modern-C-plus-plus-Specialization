#include <cstdint>
#include <algorithm>
#include <memory>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template<typename T>
class SimpleVector {
public:
    SimpleVector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit SimpleVector(size_t size) : data_(new T[size]), size_(size), capacity_(size) {}

    ~SimpleVector() {
        delete[] data_;
        size_ = 0;
        capacity_ = 0;
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
//
//    void PushBack(const T &value) {
//        if (size_ == capacity_) {
//            IncreaseCapacity();
//        }
//        data_[size_++] = value;
//    }

    void PushBack(T value) { // can be used to push either temporary or persistent objects
        if (size_ == capacity_) {
            IncreaseCapacity();
        }
        data_[size_++] = move(value);
    }

//    SimpleVector(SimpleVector &&rhs) noexcept:
//            data_(new T[rhs.capacity_]),
//            size_(rhs.size_),
//            capacity_(rhs.capacity_) {
//        std::move(rhs.begin(), rhs.end(), begin());
//    }
//
//    void operator=(SimpleVector &&rhs) noexcept {
//        if (rhs.size_ <= capacity_) {
//            std::move(rhs.begin(), rhs.end(), begin());
//            size_ = rhs.size_;
//        } else {
//            SimpleVector<T> tmp(move(rhs));
//            std::swap(tmp.data_, data_);
//            std::swap(tmp.size_, size_);
//            std::swap(tmp.capacity_, capacity_);
//        }
//    }


private:

    void IncreaseCapacity() {
        capacity_ = (capacity_) ? capacity_ * 2 : 1;
        T *tmp = new T[capacity_];
        std::move(begin(), end(), tmp);
        delete[] data_;
        data_ = tmp;
    }

    T *data_;
    size_t size_{};
    size_t capacity_{};
};
#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template<typename T>
class UniquePtr {
private:
    T* pointer_;
public:
    UniquePtr(): pointer_(nullptr) {
    };

    UniquePtr(T* ptr) : pointer_(ptr) {
    }

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;


    UniquePtr(UniquePtr&& other): pointer_(other.Release()) {
    }


    UniquePtr& operator=(nullptr_t) {
        Reset(nullptr);
        return *this;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        Reset(other.Release());
        return *this;
    }

    ~UniquePtr(){
        delete pointer_;
    }

    T& operator*() const {
        return *pointer_;
    }

    T* operator->() const {
        return pointer_;
    }

    T* Release() {
        auto tmp = pointer_;
        pointer_ = nullptr;
        return tmp;
    }

    void Reset(T* ptr) {
        delete pointer_;
        pointer_ = ptr;
    }

    void Swap(UniquePtr& other) {
        swap(pointer_, other.pointer_);
    }

    T* Get() const {
        return pointer_;
    }
};


struct Item {
    static int counter;
    int value;

    Item(int v = 0) : value(v) {
        ++counter;
    }

    Item(const Item& other) : value(other.value) {
        ++counter;
    }

    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
}
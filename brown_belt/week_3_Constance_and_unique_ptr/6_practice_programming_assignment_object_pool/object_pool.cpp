#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template<class T>
class ObjectPool {
public:
    T* Allocate() {
        if (released_.empty()) {
            released_.push(new T);
        }
        return AllocateReleased();
    }

    T* TryAllocate() {
        if (released_.empty()) {
            return nullptr;
        }
        return AllocateReleased();
    }

    void Deallocate(T* object) {
        if (!allocated_.count(object)) throw invalid_argument("There is no such allocated object");

        allocated_.erase(object);
        released_.push(object);
    }

    ~ObjectPool() {
        ClearAllocated();
        ClearReleased();
    }

private:
    T* AllocateReleased() {
        T* tmp = released_.front();
        allocated_.insert(tmp);
        released_.pop();
        return tmp;
    }

    void ClearAllocated() {
        for (T* elem: allocated_) {
            delete elem;
        }
        allocated_.clear();
    }

    void ClearReleased() {
        while (!released_.empty()) {
            T* tmp = released_.front();
            released_.pop();
            delete tmp;
        }
    }

    queue<T*> released_;
    set<T*> allocated_;
};

void TestObjectPool() {
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
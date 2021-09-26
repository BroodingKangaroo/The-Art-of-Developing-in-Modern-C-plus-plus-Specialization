#include "test_runner.h"

#include <numeric>
#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

// Реализуйте шаблон Synchronized<T>.
// Метод GetAccess должен возвращать структуру, в которой есть поле T& value.
template<typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(move(initial)) {}

    struct Access {
        T &ref_to_value;
        lock_guard<mutex> lc;
    };

    Access GetAccess() {
        return {value, lock_guard(m)};
    }

private:
    mutex m;
    T value;
};
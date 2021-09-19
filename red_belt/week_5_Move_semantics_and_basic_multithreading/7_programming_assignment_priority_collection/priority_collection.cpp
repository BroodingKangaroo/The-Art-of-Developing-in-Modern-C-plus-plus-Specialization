#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <list>
#include <functional>
#include <stack>

using namespace std;

template<typename T>
class PriorityCollection {
public:
    using Id = size_t;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        Id id = objects_.size();
        objects_.push_back({move(object), 0});
        priorities_.insert({0, id});
        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for (;range_begin !=range_end; range_begin++) {
            *(ids_begin++) = Add(move(*range_begin));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return id < objects_.size() && objects_.at(id).priority != -1;
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const {
        return objects_.at(id).object;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        Object &object = objects_[id];
        priorities_.erase({object.priority, id});
        priorities_.insert({object.priority + 1, id});
        object.priority++;
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const {
        auto &max_object = objects_.at(priorities_.rbegin()->id);
        return {max_object.object, max_object.priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto max_priority = prev(priorities_.end());
        auto &max_object = objects_[max_priority->id];
        int old_priority = max_object.priority;
        max_object.priority = -1;
        priorities_.erase(max_priority);
        return {move(max_object.object), old_priority};
    }

private:

    struct Object {
        T object;
        int priority;
    };

    struct Priority {
        int priority;
        Id id;

        bool operator < (const Priority&rhs) const {
            if (priority != rhs.priority) {
                return priority < rhs.priority;
            } else {
                return id < rhs.id;
            }
        }
    };

    set<Priority> priorities_;
    vector<Object> objects_;
};


class StringNonCopyable : public string {
public:
    using string::string;  // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable &) = delete;

    StringNonCopyable(StringNonCopyable &&) = default;

    StringNonCopyable &operator=(const StringNonCopyable &) = delete;

    StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);

    {
        auto max = strings.GetMax();
        strings.IsValid(white_id);
        strings.Get(white_id);
        ASSERT_EQUAL(max.first, "red");
        ASSERT_EQUAL(max.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
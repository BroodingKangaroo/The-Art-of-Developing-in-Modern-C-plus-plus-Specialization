#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <functional>

using namespace std;

template<typename T>
class PriorityCollection {
public:
    using Id = size_t;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        ids_.push_back(prioritised_data_.insert({0, ids_.size(), move(object)}));
        return ids_.size() - 1;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template<typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for (; range_begin != range_end; range_begin++) {
            *ids_begin++ = Add(move(*range_begin));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        return ids_[id] != prioritised_data_.end();
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const {
        return ids_[id]->object;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        auto object = prioritised_data_.extract(ids_[id]);
        object.value().priority++;
        ids_[id] = prioritised_data_.emplace(move(object.value()));
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const {
        const Object &max_ = *prev(prioritised_data_.end());
        return {max_.object, max_.priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto max_ = prioritised_data_.extract(prev(prioritised_data_.end()));
        ids_[max_.value().id] = prioritised_data_.end();
        return {move(max_.value().object), max_.value().priority};
    }

private:

    struct Object {
        int priority{};
        Id id{};
        T object;

        bool operator<(const Object &rhs) const {
            return make_pair(priority, id) < make_pair(rhs.priority, rhs.id);
        }
    };

    multiset<Object> prioritised_data_;
    vector<typename multiset<Object>::iterator> ids_;
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

void TestOther() {
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
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
//    strings.Add(move(strings.PopMax().first));

    const auto brown_id = strings.Add("brown");
    for (int i = 0; i < 5; ++i) {
        strings.Promote(brown_id);
    }
    {
        auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "brown");
        ASSERT_EQUAL(item.second, 5);
    }
    {
        auto item = strings.PopMax();
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
//    RUN_TEST(tr, TestNoCopy);
    RUN_TEST(tr, TestOther);
    return 0;
}
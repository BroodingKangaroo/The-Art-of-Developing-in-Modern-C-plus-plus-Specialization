#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;

    bool operator==(const Record& other) const {
        return id == other.id && title == other.title && user == other.user && timestamp == other.timestamp && karma == other.karma;
    }
};

// Реализуйте этот класс
class Database {
    using Id = string_view;
    Id make_id(const string& s) {
        return Id(s);
    }
public:
    bool Put(const Record& record) {
        if (id_to_records_.find(record.id) != id_to_records_.end()) return false;
        shared_ptr<Record> copy_record = make_shared<Record>(record);
        auto id = make_id(copy_record->id);
        auto it1 = timestamp_to_records_.insert({copy_record->timestamp, copy_record});
        auto it2 = karma_to_records_.insert({copy_record->karma, copy_record});
        auto it3 = user_to_records_.insert({copy_record->user, copy_record});
        id_to_records_[id] = {copy_record, it1, it2, it3};
        return true;
    }

    const Record* GetById(const string& id) const {
        if (id_to_records_.find(id) == id_to_records_.end()) return nullptr;
        return id_to_records_.at(id).record.get();
    }
    bool Erase(const string& id) {
        if (id_to_records_.find(id) == id_to_records_.end()) return false;
        auto record_object = id_to_records_.extract(id);
        timestamp_to_records_.erase(record_object.mapped().timestamp_iterator);
        karma_to_records_.erase(record_object.mapped().karma_iterator);
        user_to_records_.erase(record_object.mapped().user_iterator);
        return true;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto left = timestamp_to_records_.lower_bound(low);
        auto right = timestamp_to_records_.upper_bound(high);
        ApplyToRange(left, right ,callback);
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto left = karma_to_records_.lower_bound(low);
        auto right = karma_to_records_.upper_bound(high);
        ApplyToRange(left, right ,callback);
    }

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        const auto& [left, right] = user_to_records_.equal_range(user);
        ApplyToRange(left, right ,callback);
    }

private:
    struct RecordObject {
        shared_ptr<Record> record;
        multimap<int, shared_ptr<Record>>::iterator timestamp_iterator;
        multimap<int, shared_ptr<Record>>::iterator karma_iterator;
        multimap<string, shared_ptr<Record>>::iterator user_iterator;
    };

    template <typename Iterator, typename Callback>
    void ApplyToRange(Iterator left, Iterator right, Callback callback) const {
        for (; left != right; left++) {
            if (!callback(*(left->second))) break;
        }
    }

    unordered_map<Id, RecordObject>id_to_records_;
    multimap<int, shared_ptr<Record>>timestamp_to_records_;
    multimap<int, shared_ptr<Record>>karma_to_records_;
    multimap<string, shared_ptr<Record>>user_to_records_;
};

void TestRangeBoundaries() {
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestSameUser() {
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master", [&count](const Record&) {
        ++count;
        return true;
    });

    ASSERT_EQUAL(2, count);
}

void TestReplacement() {
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

void TestOther() {
    const string final_body = "Feeling sad";

    Database db;
    const Record r1 = {"id1", "qwe1", "usr1", 1, 11};
    const Record r2 = {"id2", "qwe2", "usr2", 2, 22};
    const Record r3 = {"id3", "qwe3", "usr3", 3, 33};
    const Record r4 = {"id4", "qwe4", "usr4", 4, 44};
    const Record r5 = {"id5", "qwe5", "usr5", 5, 55};
    const Record r6 = {"id6", "qwe6", "usr6", 6, 66};

    db.Put(r1);
    db.Put(r2);
    db.Put(r3);
    db.Put(r4);
    db.Put(r5);
    db.Put(r2);
    db.Put(r6);
    db.Erase("id1");
    db.Erase("id3");
    db.Erase("id5");

    const Record* record2 = db.GetById("id2");
    const Record* record3 = db.GetById("id3");
    const Record* record4 = db.GetById("id4");
    const Record* record5 = db.GetById("id5");
    ASSERT((*record2 == r2));
    ASSERT((record3 == nullptr));
    ASSERT((*record4 == r4));
    ASSERT((record5 == nullptr));
    db.Put(r3);
    record3 = db.GetById("id3");
    ASSERT((*record3 == r3));

    const Record fake_r3 = {"id3", "fake3", "fake3", 4, 44};
    db.Put(fake_r3);
    record3 = db.GetById("id3");
    ASSERT((*record3 == r3));
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    RUN_TEST(tr, TestOther);
    return 0;
}
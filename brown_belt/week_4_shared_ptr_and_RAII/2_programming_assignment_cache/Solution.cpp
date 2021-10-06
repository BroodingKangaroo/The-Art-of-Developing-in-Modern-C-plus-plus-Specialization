#include "Common.h"

#include <unordered_map>
#include <set>
#include <atomic>
#include <utility>
#include <future>

using namespace std;


class LruCache : public ICache {

    using priorityToBook = pair<uint32_t, shared_ptr<IBook>>;

    struct Comp {
        bool operator()(const priorityToBook& lhs, const priorityToBook& rhs) const {
            return lhs.first < rhs.first;
        }
    };

public:
    LruCache(
            shared_ptr<IBooksUnpacker> books_unpacker,
            const Settings& settings
    ) : settings_(settings), books_unpacker_(move(books_unpacker)) {
    }

    BookPtr GetBook(const string& book_name) override {
        lock_guard lc(m);
        if (data_.count(book_name)) {
            IncreaseBookPriority(book_name);
            return data_[book_name]->second;
        } else {
            shared_ptr<IBook> book = books_unpacker_->UnpackBook(book_name);
            size_t content_size = book->GetContent().size();
            if (TryFitCacheInMemory(content_size)) {
                data_[book->GetName()] = prioritized_cache_.insert({++max_priority_, book}).first;
                data_size_ += content_size;
                return data_[book->GetName()]->second;
            } else {
                return book;
            }
        }
    }

private:
    void IncreaseBookPriority(const string& book_name) {
        auto it = data_[book_name];
        if (it != prev(prioritized_cache_.end())) {
            auto node = prioritized_cache_.extract(it);
            node.value().first = ++max_priority_;
            data_[book_name] = prioritized_cache_.insert(move(node.value())).first;
        }
    }

    bool TryFitCacheInMemory(size_t book_size) {
        while (data_size_ + book_size > settings_.max_memory && !prioritized_cache_.empty()) {
            data_size_ -= prioritized_cache_.begin()->second->GetContent().size();
            auto node = prioritized_cache_.extract(prioritized_cache_.begin());
            data_.erase(node.value().second->GetName());
        }
        if (book_size > settings_.max_memory) return false;
        else return true;
    }

    const Settings settings_;
    const shared_ptr<IBooksUnpacker> books_unpacker_;
    unordered_map<string, set<priorityToBook, Comp>::iterator> data_;
    set<priorityToBook, Comp> prioritized_cache_;
    atomic<size_t> data_size_{};
    atomic<size_t> max_priority_{};
    mutex m;
};


unique_ptr<ICache> MakeCache(
        shared_ptr<IBooksUnpacker> books_unpacker,
        const ICache::Settings& settings
) {
    return make_unique<LruCache>(move(books_unpacker), settings);
}
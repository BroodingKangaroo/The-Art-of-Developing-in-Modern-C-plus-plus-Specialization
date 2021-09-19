#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator+=(const Stats &other) {
        for (auto&[k, v]: other.word_frequences) {
            word_frequences[k] += v;
        }
    }
};

Stats ExploreLine(const set<string> &key_words, const vector<string> &lines, size_t start, size_t finish) {
    Stats result;
    string buf;
    for (; start < finish && start < lines.size(); start++) {
        stringstream ss(lines[start]);
        while (ss >> buf) if (key_words.count(buf)) result.word_frequences[buf]++;
    }
    return result;
}


Stats ExploreKeyWords(const set<string> &key_words, istream &input) {
    vector<future<Stats>> futures;
    vector<string> lines;
    for (string line; getline(input, line);) {
        lines.push_back(line);
    }

    size_t page_size = lines.size() > 4 ? lines.size() / 4 : lines.size();
    for (size_t i = 0; i < lines.size(); i += page_size) {
        futures.push_back(async(ExploreLine, ref(key_words), ref(lines), i, i + page_size));
    }

    Stats result;
    for (auto &future_: futures) {
        result += future_.get();
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(const set<string> &key_words, istream &input) {
    vector<string> lines;
    for (string line; getline(input, line);) {
        lines.push_back(line);
    }

    return ExploreLine(ref(key_words), ref(lines), 0, lines.size());
}

void TestBasic() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
            {"yangle", 6},
            {"rocks",  2},
            {"sucks",  1}
    };
    ASSERT_EQUAL(stats.word_frequences, expected);
}

void TestSingleThread() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    {
        LOG_DURATION("Single thread")
        stringstream ss;
        for (int i = 0; i < 1000000; i++) {
            ss << "this new yangle service really rocks\n";
            ss << "It sucks when yangle isn't available\n";
            ss << "10 reasons why yangle is the best IT company\n";
            ss << "yangle rocks others suck\n";
            ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
            const auto stats = ExploreKeyWordsSingleThread(key_words, ss);
        }
    }
}

void TestMultiThread() {
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    {
        LOG_DURATION("Multi thread")
        stringstream ss;
        for (int i = 0; i < 1000000; i++) {
            ss << "this new yangle service really rocks\n";
            ss << "It sucks when yangle isn't available\n";
            ss << "10 reasons why yangle is the best IT company\n";
            ss << "yangle rocks others suck\n";
            ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
            const auto stats = ExploreKeyWords(key_words, ss);
        }
    }
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
    RUN_TEST(tr, TestSingleThread);
    RUN_TEST(tr, TestMultiThread);
}
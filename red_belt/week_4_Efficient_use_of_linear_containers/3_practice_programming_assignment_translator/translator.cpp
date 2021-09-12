#include <string>
#include <unordered_map>
#include <algorithm>
#include <list>

#include "test_runner.h"

using namespace std;

class Translator {
public:
    void Add(string_view source, string_view target) {
        const string_view source_view = GetClonedView(source);
        const string_view target_view = GetClonedView(target);
        forward_pairs_[source_view] = target_view;
        backward_pairs_[target_view] = source_view;
    }

    string_view TranslateForward(string_view source) const {
        return forward_pairs_.count(source) ? forward_pairs_.at(source) : "";
    }

    string_view TranslateBackward(string_view target) const {
        return backward_pairs_.count(target) ? backward_pairs_.at(target) : "";
    }

private:

    string_view GetClonedView(string_view s) {
        for (const auto *map_ptr: {&forward_pairs_, &backward_pairs_}) {
            const auto it = map_ptr->find(s);
            if (it != map_ptr->end()) {
                return it->first;
            }
        }
        return words_.emplace_back(s);
    }

    list<string> words_;
    unordered_map<string_view, string_view> forward_pairs_;
    unordered_map<string_view, string_view> backward_pairs_;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("1"), string("4"));
    ASSERT_EQUAL(translator.TranslateForward("1"), "4");
    translator.Add(string("2"), string("5"));
    translator.Add(string("3"), string("6"));
    ASSERT_EQUAL(translator.TranslateForward("1"), "4");
    ASSERT_EQUAL(translator.TranslateForward("2"), "5");
    ASSERT_EQUAL(translator.TranslateForward("3"), "6");
    translator.Add(string("3"), string("7"));
    ASSERT_EQUAL(translator.TranslateForward("3"), "7");

    translator.Add(string("qwe"), string("7"));
    ASSERT_EQUAL(translator.TranslateForward("qwe"), "7");
    ASSERT_EQUAL(translator.TranslateForward("3"), "7");
    ASSERT_EQUAL(translator.TranslateBackward("7"), "qwe");
}

void TestBothAbsent() {
    Translator translator;
    translator.Add(string("1"), string("4"));
    ASSERT_EQUAL(translator.TranslateForward("1"), "4");
    ASSERT_EQUAL(translator.TranslateForward("4"), "");
    ASSERT_EQUAL(translator.TranslateBackward("4"), "1");

    translator.Add(string("qwer"), string("ty"));
    ASSERT_EQUAL(translator.TranslateForward("qwer"), "ty");
    ASSERT_EQUAL(translator.TranslateForward("ty"), "");
    ASSERT_EQUAL(translator.TranslateBackward("ty"), "qwer");
}

void TestTargetAbsent() {
    Translator translator;
    translator.Add(string("1"), string("4"));
    translator.Add(string("1"), string("170"));
    ASSERT_EQUAL(translator.TranslateForward("1"), "170");
    ASSERT_EQUAL(translator.TranslateForward("4"), "");
    ASSERT_EQUAL(translator.TranslateForward("170"), "");
    ASSERT_EQUAL(translator.TranslateBackward("170"), "1");
    ASSERT_EQUAL(translator.TranslateBackward("4"), "1");
}


void TestSourceAbsent() {
    Translator translator;
    translator.Add(string("1"), string("4"));
    translator.Add(string("3"), string("1"));
    ASSERT_EQUAL(translator.TranslateForward("1"), "4");
    ASSERT_EQUAL(translator.TranslateForward("3"), "1");
    ASSERT_EQUAL(translator.TranslateBackward("1"), "3");
    ASSERT_EQUAL(translator.TranslateBackward("4"), "1");
}

void TestBothPresent() {
    Translator translator;
    translator.Add(string("1"), string("4"));
    translator.Add(string("4"), string("1"));
    ASSERT_EQUAL(translator.TranslateForward("1"), "4");
    ASSERT_EQUAL(translator.TranslateForward("4"), "1");
    ASSERT_EQUAL(translator.TranslateBackward("4"), "1");
    ASSERT_EQUAL(translator.TranslateBackward("1"), "4");
}

void TestEmpty() {
    Translator translator;
    translator.Add(string("1"), string("4"));
    translator.Add(string("4"), string("3"));
    ASSERT_EQUAL(translator.TranslateForward("3"), "");
    ASSERT_EQUAL(translator.TranslateBackward("1"), "");
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    RUN_TEST(tr, TestBothAbsent);
    RUN_TEST(tr, TestTargetAbsent);
    RUN_TEST(tr, TestSourceAbsent);
    RUN_TEST(tr, TestBothPresent);
    RUN_TEST(tr, TestEmpty);
    return 0;
}
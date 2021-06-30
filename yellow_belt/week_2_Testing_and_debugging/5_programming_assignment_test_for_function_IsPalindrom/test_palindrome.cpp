#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
    if (t != u) {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string& test_name) {
        try {
            func();
            cerr << test_name << " OK" << endl;
        } catch (exception& e) {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        } catch (...) {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

bool IsPalindrom(const string& str) {
    // Вы можете вставлять сюда различные реализации функции,
    // чтобы проверить, что ваши тесты пропускают корректный код
    // и ловят некорректный
}

void TestRules() {
    Assert(IsPalindrom(""), "Empty string");
    Assert(IsPalindrom("a"), "One symbol string");
    Assert(!IsPalindrom("a a "), "Should not be a palindrome if space is not ignored");
    Assert(!IsPalindrom("aa,"), "Should not be a palindrome if comma is not ignored");
    Assert(!IsPalindrom("aa,,,"), "Should not be a palindrome if commas is not ignored");
    Assert(!IsPalindrom("aa1"), "Should not be a palindrome if number is not ignored");
    Assert(!IsPalindrom("aa11"), "Should not be a palindrome if number is not ignored");
}

void TestRandomWords() {
    Assert(IsPalindrom("madam"), "madam is a palindrome");
    Assert(IsPalindrom("level"), "level is a palindrome");
    Assert(IsPalindrom("wasitacaroracatisaw"), "wasitacaroracatisaw is a palindrome");
    Assert(IsPalindrom("@@"), "@@ is a palindrome");
    Assert(IsPalindrom("////////////////////"), "//////////////////// is a palindrome");
    Assert(!IsPalindrom("123-+321"), "12323321 is not a palindrome");
    Assert(!IsPalindrom("1233210"), "1233210 is not a palindrome");
    Assert(!IsPalindrom("0123321"), "0123321 is not a palindrome");
    Assert(!IsPalindrom("-123321+"), "-123321+ is not a palindrome");
    Assert(!IsPalindrom("-12321+"), "-12321+ is not a palindrome");
    Assert(!IsPalindrom("121  12321 121"), "121  12321 121 is not a palindrome");
    Assert(!IsPalindrom("Anama"), "Anama is not a palindrome");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestRules, "Test rules");
    runner.RunTest(TestRandomWords, "Test random words");
    return 0;
}
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

class Person {
public:
    // Вы можете вставлять сюда различные реализации класса,
    // чтобы проверить, что ваши тесты пропускают корректный код
    // и ловят некорректный
    void ChangeFirstName(int year, const string& first_name) {
    }
    void ChangeLastName(int year, const string& last_name) {
    }
    string GetFullName(int year) {
    }
};

void TestIncognito() {
    Person pr;
    AssertEqual(pr.GetFullName(49), "Incognito");
    pr.ChangeFirstName(100, "firstName");
    pr.ChangeLastName(50, "lastName");
    AssertEqual(pr.GetFullName(49), "Incognito");
}

void TestUnknownFirstName() {
    Person pr;
    pr.ChangeFirstName(100, "firstName");
    pr.ChangeLastName(50, "lastName");
    AssertEqual(pr.GetFullName(99), "lastName with unknown first name");
}

void TestUnknownLastName() {
    Person pr;
    pr.ChangeFirstName(50, "firstName");
    pr.ChangeLastName(100, "lastName");
    AssertEqual(pr.GetFullName(99), "firstName with unknown last name");
}

void TestMultipleFirstNames() {
    Person pr;
    pr.ChangeFirstName(50, "firstName");
    pr.ChangeFirstName(51, "firstName1");
    pr.ChangeLastName(100, "lastName");
    AssertEqual(pr.GetFullName(101), "firstName1 lastName");
}

void TestMultipleLastNames() {
    Person pr;
    pr.ChangeFirstName(50, "firstName");
    pr.ChangeLastName(100, "lastName");
    pr.ChangeLastName(101, "lastName1");
    AssertEqual(pr.GetFullName(102), "firstName lastName1");
}

void TestReverseOrderChangingFirstName() {
    Person pr;
    pr.ChangeFirstName(10, "firstName");
    pr.ChangeLastName(0, "lastName");
    pr.ChangeFirstName(0, "firstName1");
    AssertEqual(pr.GetFullName(5), "firstName1 lastName");
    AssertEqual(pr.GetFullName(100), "firstName lastName");
}

void TestReverseOrderChangingLastName() {
    Person pr;
    pr.ChangeLastName(10, "lastName");
    pr.ChangeFirstName(0, "firstName");
    pr.ChangeLastName(0, "lastName1");
    AssertEqual(pr.GetFullName(5), "firstName lastName1");
    AssertEqual(pr.GetFullName(100), "firstName lastName");
}

void TestSameYear() {
    Person pr;
    pr.ChangeFirstName(0, "firstName");
    pr.ChangeLastName(0, "lastName");
    AssertEqual(pr.GetFullName(0), "firstName lastName");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestIncognito, "test incognito");
    runner.RunTest(TestUnknownFirstName, "test unknown first name");
    runner.RunTest(TestUnknownLastName, "test unknown last name");
    runner.RunTest(TestMultipleFirstNames, "test multiple first names");
    runner.RunTest(TestMultipleLastNames, "test multiple last names");
    runner.RunTest(TestReverseOrderChangingFirstName, "test reverse order of changing first name");
    runner.RunTest(TestReverseOrderChangingLastName, "test reverse order of changing last name");
    runner.RunTest(TestSameYear, "test same year check");


    return 0;
}
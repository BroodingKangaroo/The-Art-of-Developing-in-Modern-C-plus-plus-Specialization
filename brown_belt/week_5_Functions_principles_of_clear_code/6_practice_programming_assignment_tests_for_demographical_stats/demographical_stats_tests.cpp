#include "test_runner.h"
#include "good_solution.h"

void TestEmptyVector() {
    vector<Person>persons;
    auto stats = ComputeStats(persons);
    ASSERT_EQUAL(stats.total, 0);
    ASSERT_EQUAL(stats.females, 0);
    ASSERT_EQUAL(stats.males, 0);
    ASSERT_EQUAL(stats.employed_females, 0);
    ASSERT_EQUAL(stats.employed_males, 0);
    ASSERT_EQUAL(stats.unemployed_females, 0);
    ASSERT_EQUAL(stats.unemployed_males, 0);
}

void TestSimple() {
    vector<Person>persons = {
            {10, Gender::FEMALE, true},
            {15, Gender::FEMALE, true},
            {20, Gender::MALE, true},
            {25, Gender::MALE, true},
            {30, Gender::FEMALE, false},
            {35, Gender::MALE, false},
    };
    auto stats = ComputeStats(persons);
    ASSERT_EQUAL(stats.total, 25);
    ASSERT_EQUAL(stats.females, 15);
    ASSERT_EQUAL(stats.males, 25);
    ASSERT_EQUAL(stats.employed_females, 15);
    ASSERT_EQUAL(stats.employed_males, 25);
    ASSERT_EQUAL(stats.unemployed_females, 30);
    ASSERT_EQUAL(stats.unemployed_males, 35);
}

void TestReadPersons() {
    stringstream ss("4\n"
                    "10 0 1\n"
                    "20 1 0\n"
                    "30 0 0\n"
                    "40 1 1");
    auto actual_persons = ReadPersons(ss);
    vector<Person> expected_persons = {
            {10, Gender::FEMALE, true},
            {20, Gender::MALE, false},
            {30, Gender::FEMALE, false},
            {40, Gender::MALE, true},
    };
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQUAL(actual_persons[i], expected_persons[i]);
    }
}

void TestPrintStats() {
    AgeStats age_stats = {
            135,
            55,
            80,
            25,
            30,
            45,
            35
    };
    vector<string>expected_result = {
            "Median age = 135",
            "Median age for females = 55",
            "Median age for males = 80",
            "Median age for employed females = 25",
            "Median age for unemployed females = 30",
            "Median age for employed males = 45",
            "Median age for unemployed males = 35",
    };
    stringstream ss;
    PrintStats(age_stats, ss);
    string line;
    for (size_t i = 0; i < sizeof(AgeStats) / 4; ++i) {
        getline(ss, line);
        ASSERT_EQUAL(line, expected_result[i]);
    }
}

int main() {
    TestRunner tr;
    tr.RunTest(TestEmptyVector, "Empty vector");
    tr.RunTest(TestPrintStats, "Print stats");
    tr.RunTest(TestReadPersons, "Read persons");
    tr.RunTest(TestSimple, "Simple");
}
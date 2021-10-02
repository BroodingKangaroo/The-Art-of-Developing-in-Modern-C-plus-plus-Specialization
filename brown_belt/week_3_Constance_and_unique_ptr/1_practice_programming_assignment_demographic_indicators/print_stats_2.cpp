#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

class Population {
    using Age = uint16_t;
    using Wealth = uint64_t;

    string CalculateMostPopularName(unordered_map<string, int32_t> names) {
        int32_t max_amount = -1;
        for (const auto&[name, amount]: names) {
            if (amount > max_amount) max_amount = amount;
        }
        string most_popular_name = "";
        for (const auto&[name, amount]: names) {
            if (amount == max_amount && (most_popular_name.empty() || most_popular_name > name)) {
                most_popular_name = name;
            }
        }
        return most_popular_name;
    }

public:

    Population(istream &input) {
        unordered_map<string, int32_t> male_names;
        unordered_map<string, int32_t> female_names;
        uint32_t count, age;
        uint64_t income;
        string name;
        char gender;
        input >> count;
        for (size_t i = 0; i < count; ++i) {
            input >> name >> age >> income >> gender;
            if (gender == 'M') {
                male_names[name]++;
            } else {
                female_names[name]++;
            }

            sorted_by_age_.push_back(age);
            sorted_by_wealth_.push_back(income);
        }

        most_popular_female_name_ = female_names.empty() ? "" : CalculateMostPopularName(female_names);
        most_popular_male_name_ = male_names.empty() ? "" : CalculateMostPopularName(male_names);
        sort(sorted_by_age_.begin(), sorted_by_age_.end());
        sort(sorted_by_wealth_.begin(), sorted_by_wealth_.end(), greater_equal<>());
    }

    uint64_t GetWealthyStats(size_t M) const {
        uint64_t total_income = 0;
        for (size_t i = 0; i < M; i++) {
            total_income += sorted_by_wealth_[i];
        }
        return total_income;
    }

    uint32_t GetAgeStats(size_t N) const {
        if (sorted_by_age_.empty()) return 0;
        return sorted_by_age_.end() - lower_bound(sorted_by_age_.begin(), sorted_by_age_.end(), N);
    }

    const string &PopularMaleName() const {
        return most_popular_male_name_;
    }

    const string &PopularFemaleName() const {
        return most_popular_female_name_;
    }

private:
    vector<Age> sorted_by_age_;
    vector<Wealth> sorted_by_wealth_;
    string most_popular_male_name_;
    string most_popular_female_name_;
};


int main() {
    Population population(cin);

    for (string command; cin >> command;) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;
            auto result = population.GetAgeStats(adult_age);
            cout << "There are " << result << " adult people for maturity age " << adult_age << endl;
        } else if (command == "WEALTHY") {
            int count;
            cin >> count;
            auto result = population.GetWealthyStats(count);
            cout << "Top-" << count << " people have total income " << result << endl;
        } else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;
            string result;
            if (gender == 'M') {
                result = population.PopularMaleName();
            } else {
                result = population.PopularFemaleName();
            }
            if (result.empty()) {
                cout << "No people of gender " << gender << endl;
            } else {
                cout << "Most popular name among people of gender " << gender << " is " << result << endl;
            }
        }
    }
}
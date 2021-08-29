#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <queue>
#include <set>
#include <algorithm>
#include <functional>
#include <numeric>

using namespace std;

class Budget {
    vector<double> _income;
public :

    Budget() {
        _income.resize(366 * 400);
    }

    void earn(int date, double income) {
        _income[date] += income;
    }

    double computeIncome(int begin, int end) {
        return _income[end] - _income[begin - 1];
    }

    void precomputePartialSums() {
        partial_sum(_income.begin(), _income.end(), _income.begin());
    }
};

int getDays(const string &str) {
    stringstream ss(str);
    tm tm{};
    ss >> get_time(&tm, "%Y-%m-%d");
    chrono::system_clock::time_point tp = chrono::system_clock::from_time_t(mktime(&tm));
    return chrono::duration_cast<chrono::hours>(tp.time_since_epoch()).count() / 24;
}

int getDaysSince1700(const string &str) {
    return getDays(str) - getDays("1700-00-00");
}

int main() {

    int n;
    cin >> n;
    Budget budget;
    while (n--) {
        string date;
        double income;
        cin >> date >> income;
        int dateInt = getDaysSince1700(date);
        budget.earn(dateInt, income);
    }
    budget.precomputePartialSums();
    cin >> n;
    while (n--) {
        cout.precision(25);
        string begin, end;
        cin >> begin >> end;
        cout << budget.computeIncome(getDaysSince1700(begin), getDaysSince1700(end)) << endl;
    }

    return 0;
}
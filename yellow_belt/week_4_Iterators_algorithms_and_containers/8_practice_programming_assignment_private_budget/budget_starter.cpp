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
        _income.resize(366 * 100);
    }

    void earn(int begin, int end, double income) {
        for (int i = begin; i <= end; i++) {
            _income[i] += income;
        }
    }

    double computeIncome(int begin, int end) {
        return std::accumulate(_income.begin() + begin, _income.begin() + end + 1, 0.0);
    }
};

int getDays(const string &str) {
    stringstream ss(str);
    tm tm{};
    ss >> get_time(&tm, "%Y-%m-%d");
    chrono::system_clock::time_point tp = chrono::system_clock::from_time_t(mktime(&tm));
    return chrono::duration_cast<chrono::hours>(tp.time_since_epoch()).count() / 24;
}

int getDaysSince2000(const string &str) {
    return getDays(str) - getDays("2000-00-00");
}

int main() {

    int n;
    cin >> n;
    Budget budget;
    while (n--) {
        string query, begin, end;
        cin >> query >> begin >> end;
        int beginDays = getDaysSince2000(begin), endDays = getDaysSince2000(end);
        if (query == "Earn") {
            double income;
            cin >> income;
            budget.earn(beginDays, endDays, income / (endDays - beginDays + 1));
        } else if (query == "ComputeIncome") {
            cout.precision(25);
            cout << budget.computeIncome(getDaysSince2000(begin), getDaysSince2000(end)) << endl;
        }
    }

    return 0;
}
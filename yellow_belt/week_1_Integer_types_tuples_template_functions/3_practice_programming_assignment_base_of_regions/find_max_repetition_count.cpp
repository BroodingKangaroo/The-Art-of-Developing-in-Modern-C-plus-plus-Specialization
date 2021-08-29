#include <map>
#include <tuple>
#include <vector>

using namespace std;

class Region {
public:
    string std_name;
    string parent_std_name;
    string names;
    int population;
};

bool operator<(const Region &lhs, const Region &rhs) {
    return tie(lhs.std_name, lhs.parent_std_name, lhs.names, lhs.population) <
           tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
}

int FindMaxRepetitionCount(const vector<Region> &regions) {
    int result = 0;
    map<Region, int> repetition_count; // won't work without operator < for the keys
    for (const Region &region: regions) {
        result = max(result, ++repetition_count[region]);
    }
    return result;
}

int main() {
    return 0;
}
#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class Athletes {
public:
    void Insert(const int &number, const int &position) {
        if (!numberToPosition_.count(position)) {
            athletes.push_back(number);
            numberToPosition_[number] = prev(athletes.end());
        } else {
            athletes.insert(numberToPosition_[position], number);
            numberToPosition_[number] = prev(numberToPosition_[position]);
        }
    }

    void Print(ostream &os) {
        for (const auto &el: athletes) {
            os << el << endl;
        }
    }

private:
    unordered_map<int, list<int>::iterator> numberToPosition_;
    list<int> athletes;
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    int n;
    cin >> n;
    Athletes athletes;
    while (n--) {
        int number, position;
        cin >> number >> position;
        athletes.Insert(number, position);
    }
    athletes.Print(cout);
}
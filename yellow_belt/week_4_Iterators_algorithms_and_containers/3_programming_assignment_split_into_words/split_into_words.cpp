#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

vector<string> SplitIntoWords(const string &s) {
    auto prev_space = s.begin();
    auto next_space = find(s.begin(), s.end(), ' ');
    vector<string> result;
    do {
        result.emplace_back(prev_space, next_space);
        prev_space = next_space + 1;
        next_space = find(prev_space, s.end(), ' ');
    } while(next_space < s.end() || prev_space < s.end());
    return result;
}

int main() {
    string s = "C Cpp Java Python";
    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;

    return 0;
}

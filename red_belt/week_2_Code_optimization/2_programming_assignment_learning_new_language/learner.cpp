#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
private:
    set<string> dict;

public:
    int Learn(const vector<string> &words) {
        int newWords = 0;
        for (const auto &word: words) {
            int wordsBefore = dict.size();
            dict.insert(word);
            if (dict.size() > wordsBefore) ++newWords;
        }
        return newWords;
    }

    vector<string> KnownWords() const {
        return {dict.begin(), dict.end()};
    }
};
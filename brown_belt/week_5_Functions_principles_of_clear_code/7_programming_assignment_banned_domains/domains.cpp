#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_set>

using namespace std;

class DomainManager {
public:
    explicit DomainManager(istream&in) {
        ReadBannedDomains(in);
    }

    void ReadBannedDomains(istream& in) {
        size_t count;
        in >> count;
        banned_domains_.reserve(count);
        string domain;
        for (size_t i = 0; i < count; ++i) {
            in >> domain;
            banned_domains_.push_back(domain);
            banned_domains_view_.insert(string_view(banned_domains_.back()));
        }
    }

    void RunGradeDomainLoop(istream& in) {
        size_t count;
        string domain;
        string_view domain_view;
        cin >> count;
        while (count--) {
            cin >> domain;
            domain_view = domain;
            cout << GradeDomain(domain_view) << endl;
        }
    }

private:
    string GradeDomain(string_view domain) {
        for (string_view domain_postfix: GetPossibleDomainPostfixes(domain)) {
            if (banned_domains_view_.find(domain_postfix) != banned_domains_view_.end()) {
                return "Bad";
            }
        }
        return "Good";
    }

    vector<string_view> GetPossibleDomainPostfixes(string_view domain) {
        vector<string_view>result;
        size_t dot_pos = 0;
        while(dot_pos != string_view::npos) {
            result.push_back(domain);
            dot_pos = domain.find('.');
            domain.remove_prefix(dot_pos + 1);
        }
        return result;
    }

    unordered_set<string_view> banned_domains_view_;
    vector<string> banned_domains_;
};

int main() {
    DomainManager dm(cin);
    dm.RunGradeDomainLoop(cin);
    return 0;
}
#include <iomanip>
#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class ReadingManager {
public:
    ReadingManager() : pages(1001) {}

    void Read(int user_id, int page_count) {
        if (users_to_positions_.count(user_id)) {
            for (int i = users_to_positions_[user_id] + 1; i <= page_count; i++) {
                pages[i]--;
            }
        } else {
            for (int i = page_count + 1; i <= 1000; i++) {
                pages[i]++;
            }
        }
        users_to_positions_[user_id] = page_count;
    }

    double Cheer(int user_id) const {
        if (!users_to_positions_.count(user_id)) {
            return 0.0;
        }
        if (users_to_positions_.size() == 1) {
            return 1.0;
        }
        return pages[users_to_positions_.at(user_id)] * 1.0 / (users_to_positions_.size() - 1);
    }

private:
    vector<int> pages;
    map<int, int> users_to_positions_;
};


int main() {
    // Для ускорения чтения данных отключается синхронизация
    // cin и cout с stdio,
    // а также выполняется отвязка cin от cout
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        } else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << endl;
        }
    }

    return 0;
}
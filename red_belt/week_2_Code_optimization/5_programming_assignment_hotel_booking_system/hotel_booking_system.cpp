#include <queue>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

class HotelBookingSystem {
public:
    void Book(int64_t time, const string &hotel, int client_id, int rooms) {
        queries_.push(Query(time, hotel, client_id, rooms));
        hotel_to_clients_[hotel][client_id]++;
        hotel_to_rooms_[hotel] += rooms;
    }

    size_t Clients(const string &hotel) {
        Adjust();
        return hotel_to_clients_[hotel].size();
    }

    int Rooms(const string &hotel) {
        Adjust();
        return hotel_to_rooms_[hotel];
    }

private:

    struct Query {
        Query(int64_t time, const string &hotel_name, int client_id, int rooms) : time_(time), hotel_name_(hotel_name),
                                                                                  client_id_(client_id), rooms_(rooms) {}

        int64_t time_{};
        string hotel_name_;
        int client_id_{};
        int rooms_{};
    };

    void Adjust() {
        if (queries_.empty()) return;

        int64_t current_time = queries_.back().time_;
        while (current_time - queries_.front().time_ >= TIME_WINDOW) {
            auto query_to_delete = queries_.front();
            auto &hotel_clients = hotel_to_clients_[query_to_delete.hotel_name_];
            if (hotel_clients[query_to_delete.client_id_] > 1) hotel_clients[query_to_delete.client_id_]--;
            else hotel_clients.erase(query_to_delete.client_id_);
            hotel_to_rooms_[query_to_delete.hotel_name_] -= query_to_delete.rooms_;
            queries_.pop();
        }
    }

    static const int TIME_WINDOW = 86400;
    queue<Query> queries_;
    unordered_map<string, unordered_map<int, int>> hotel_to_clients_;
    unordered_map<string, int> hotel_to_rooms_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    HotelBookingSystem bookingSystem;
    int n;
    cin >> n;
    while (n--) {
        string command;
        cin >> command;
        if (command == "BOOK") {
            int64_t time;
            string hotel;
            int client_id;
            int rooms;
            cin >> time >> hotel >> client_id >> rooms;
            bookingSystem.Book(time, hotel, client_id, rooms);
        } else if (command == "CLIENTS") {
            string hotel;
            cin >> hotel;
            cout << bookingSystem.Clients(hotel) << endl;
        } else if (command == "ROOMS") {
            string hotel;
            cin >> hotel;
            cout << bookingSystem.Rooms(hotel) << endl;
        }
    }

    return 0;
}
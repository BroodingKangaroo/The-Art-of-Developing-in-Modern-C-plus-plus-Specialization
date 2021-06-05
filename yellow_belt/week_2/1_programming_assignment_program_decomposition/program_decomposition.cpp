#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
    string operation_code; is >> operation_code;
    if (operation_code == "NEW_BUS") {
        int stop_count;
        q.stops.clear();
        q.type = QueryType::NewBus;
        is >> q.bus >> stop_count;
        while (stop_count--) {
            string stop; is >> stop;
            q.stops.push_back(stop);
        }
    } else if (operation_code == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (operation_code == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    bool no_stop = false;
    vector<string>buses_for_stop;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.no_stop) {
        os << "No stop" << endl;
    } else {
        for (const string& bus : r.buses_for_stop) {
            os << bus << " ";
        }
        os << endl;
    }
    return os;
}

struct StopsForBusResponse {
    bool no_bus = false;
    const vector<string>* stops;
    const map<string, vector<string>>* stops_to_buses;
    string bus;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    if (r.no_bus) {
        cout << "No bus" << endl;
        return os;
    }
    for (const string& stop : *r.stops) {
        os << "Stop " << stop << ": ";
        if (r.stops_to_buses->at(stop).size() == 1) {
            os << "no interchange";
        } else {
            for (const string& other_bus : r.stops_to_buses->at(stop)) {
                if (r.bus != other_bus) {
                    os << other_bus << " ";
                }
            }
        }
        os << endl;
    }
    return os;
}

struct AllBusesResponse {
    bool no_buses = false;
    const map<string, vector<string>>* buses_to_stops;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    if (r.no_buses) {
        os << "No buses" << endl;
        return os;
    }
    for (const auto& bus_item : *r.buses_to_stops) {
        os << "Bus " << bus_item.first << ": ";
        for (const string& stop : bus_item.second) {
            os << stop << " ";
        }
        os << endl;
    }
    return os;
}

class BusManager {
    map<string, vector<string>> buses_to_stops, stops_to_buses;
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops[bus] = stops;
        for (const auto& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse response;
        if (stops_to_buses.count(stop) == 0) {
            response.no_stop = true;
        } else {
            response.buses_for_stop = stops_to_buses.at(stop);
        }
        return response;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse response;
        if (buses_to_stops.count(bus) == 0) {
            response.no_bus = true;
        } else {
            response.stops = &buses_to_stops.at(bus);
            response.stops_to_buses = &stops_to_buses;
            response.bus = bus;
        }
        return response;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse response;
        if (buses_to_stops.empty()) {
            response.no_buses = true;
        } else {
            response.buses_to_stops = &buses_to_stops;
        }
        return response;
    }
};

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }

    return 0;
}
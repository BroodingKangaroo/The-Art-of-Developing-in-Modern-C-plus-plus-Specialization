#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;


struct BusesForStopResponse {
    bool no_stop = false;
    vector<string>buses_for_stop;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
    bool no_bus = false;
    const vector<string>* stops;
    const map<string, vector<string>>* stops_to_buses;
    string bus;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
    bool no_buses = false;
    const map<string, vector<string>>* buses_to_stops;
};

ostream& operator << (ostream& os, const AllBusesResponse& r);
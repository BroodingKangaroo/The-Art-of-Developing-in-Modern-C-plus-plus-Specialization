#pragma once

#include <string>
#include <vector>
#include <map>

#include "responses.h"

using namespace std;

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
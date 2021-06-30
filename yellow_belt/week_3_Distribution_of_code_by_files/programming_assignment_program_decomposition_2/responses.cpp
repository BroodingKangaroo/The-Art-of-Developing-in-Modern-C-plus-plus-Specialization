#include "responses.h"

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
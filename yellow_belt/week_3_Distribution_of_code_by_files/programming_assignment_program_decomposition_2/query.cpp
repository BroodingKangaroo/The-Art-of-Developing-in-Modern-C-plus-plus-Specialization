#include <iostream>

#include "query.h"

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
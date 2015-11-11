#include "template.h"
#include "flight.h"
#include "schedule.h"
#include "evaluator.h"

char buff[1000111];
map< int, vector<Flight> > readFlightInfo(char* file_name) {
    cout << "Reading data from " << file_name << endl;
    // It's easier to read this file with scanf, so we're using FILE* instead of fstream

    FILE* f = fopen(file_name, "r");
    // Ignore header line
    fgets(buff, sizeof buff, f);

    map< int, vector<Flight> > result;

    int flight_count = 0;
    while (fgets(buff, sizeof buff, f)) {
        Flight flight;
        try {
            flight = Flight(buff);
            ++flight_count;
        }
        catch (int e) {
            DEBUG(buff);
        }

        if (flight_count == 1) {
            cout << flight.date.to_int() << endl;
            DEBUG(flight.airline_id);
            DEBUG(flight.origin_airport_id);
            DEBUG(flight.dest_airport_id);
            DEBUG(flight.dep_delay);
            DEBUG(flight.arr_delay);
            DEBUG(flight.cancelled);
        }

        int date_stamp = flight.date.to_int();
        if (result.count(date_stamp)) {
            result[date_stamp].push_back(flight);
        }
        else {
            result[date_stamp] = vector<Flight> {flight};
        }
    }
    fclose(f);

    cout << flight_count << " flights read" << endl;
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw 1;
    }
    auto flights = readFlightInfo(argv[1]);
    cout << "Number of date: " << flights.size() << endl;

    for (auto flight : flights) {
        DEBUG(flight.first);
        PartialSchedule rand_sched = getRandomSchedule(flight.second);
        PartialSchedule beam_sched = getBeamSearchSchedule(flight.second);

        DEBUG(rand_sched.countGate());
        DEBUG(beam_sched.countGate());
    }
}

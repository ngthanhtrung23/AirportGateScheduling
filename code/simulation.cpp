const int AIRPORT_ID = 12478;

#include "template.h"
#include "flight.h"

bool operator < (const Flight& a, const Flight& b) {
    int time_a = a.get_time(AIRPORT_ID);
    int time_b = b.get_time(AIRPORT_ID);
    return time_a < time_b;
}

#include "schedule.h"
#include "evaluator.h"

//const int AIRPORT_ID = 12892;

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

        if (flight.origin_airport_id != AIRPORT_ID
                && flight.dest_airport_id != AIRPORT_ID) continue;

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
        DEBUG(flight.second.size());
        PartialSchedule rand_sched = getRandomSchedule(AIRPORT_ID, flight.second);
        DEBUG(rand_sched.countGate());
        DEBUG(rand_sched.countCollisionDelay());

        PartialSchedule beam_sched = getBeamSearchSchedule(AIRPORT_ID, flight.second);
        DEBUG(beam_sched.countGate());
        DEBUG(beam_sched.countCollisionDelay());
        break;
    }
}

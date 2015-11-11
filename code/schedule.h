#ifndef SCHEDULE

#define SCHEDULE
#include "flight.h"
const int C = 1;

int sqr(int x) {
    return x*x;
}

int cost(int time1, int time2) {
    assert(time1 > time2);
    if (time2 + 60 < time1) return 0;
    return sqr(time1 - 60 - time2);
}

struct PartialSchedule {
    map<int, int> nGate;       // Airport --> # gate
    map< pair<int,int>, vector<Flight> > flights; // Airport + Gate --> flights

    vector<Flight> *allFlight; // Store a reference to all flights
    int gap = 0;

    int countCollisionDelay() {
        int res = 0;
        for(auto p : flights) {
            auto flight_list = p.second;
            for(int i = 0; i < (int) flight_list.size() - 1; ++i) {
                int j = i + 1;
                if (collides_delay(flight_list[i], flight_list[j],
                            AIRPORT_ID, 60)) {
                    ++res;
                }
            }
        }
        return res;
    }

    void addFlightToGate(int airport_id, int gate, Flight& f1) {
        if (gate < 0) {
            gate = nGate[airport_id];
            nGate[airport_id] += 1;
        }
        auto t = make_pair(airport_id, gate);
        if (flights.count(t)) {
            auto flight_list = flights[t];
            for(int i = 0; i < (int) flight_list.size() - 1; ++i) {
                gap -= cost(flight_list[i+1].get_time(AIRPORT_ID), flight_list[i].get_time(AIRPORT_ID));
            }

            flights[t].push_back(f1);
            sort(flights[t].begin(), flights[t].end());

            flight_list = flights[t];
            for(int i = 0; i < (int) flight_list.size() - 1; ++i) {
                gap += cost(flight_list[i+1].get_time(AIRPORT_ID), flight_list[i].get_time(AIRPORT_ID));
            }
        }
        else {
            flights[t] = vector<Flight> {f1};
        }
    }

    bool canAddFlightToGate(int airport_id, int gate, Flight& f1) {
        if (gate < 0) return true; // add new gate
        assert(f1.origin_airport_id == airport_id || f1.dest_airport_id == airport_id);
        for (auto f2 : flights[make_pair(airport_id, gate)]) {
            if (collides(f1, f2, airport_id, 30)) {
                return false;
            }
        }
        return true;
    }

    int countGate() {
        int sumGate = 0;
        for (auto g : nGate)
            sumGate += g.second;
        return sumGate;
    }

    int score() {
        assert(countGate() * C + gap * 100 >= 0);
        return countGate() * C + gap * 100;
    }
};

PartialSchedule getRandomSchedule(int AIRPORT_ID, vector<Flight> flights) {
    PartialSchedule res;
    for(auto flight : flights) {
        int from = flight.origin_airport_id;
        int to = flight.dest_airport_id;
        int nGate = 0;
        bool assigned = false;

        // assign gate to "from"
        if (from == AIRPORT_ID) {
            nGate = res.nGate[from];
            assigned = false;
            vector<int> gates;
            for(int gate = 0; gate < nGate; ++gate)
                gates.push_back(gate);
            for(int gate : gates) {
                if (res.canAddFlightToGate(from, gate, flight)) {
                    res.addFlightToGate(from, gate, flight);
                    assigned = true;
                    break;
                }
            }
            if (!assigned) res.addFlightToGate(from, -1, flight);
        }

        // assign gate to "to"
        if (to == AIRPORT_ID) {
            nGate = res.nGate[to];
            assigned = false;
            for(int gate = 0; gate < nGate; ++gate) {
                if (res.canAddFlightToGate(to, gate, flight)) {
                    res.addFlightToGate(to, gate, flight);
                    assigned = true;
                    break;
                }
            }
            if (!assigned) res.addFlightToGate(to, -1, flight);
        }
    }
    return res;
}

PartialSchedule getBeamSearchSchedule(int AIRPORT_ID, vector<Flight> flights) {
    // pair< score, Schedule >
    vector< pair<int, PartialSchedule> > res;

    PartialSchedule empty;
    empty.allFlight = &flights;
    res.push_back(make_pair(0, empty));
    DEBUG(res.size());
    
    int total_flights = flights.size();
    int processed_flights = 0;

    cout << "Processing " << total_flights << " total flights" << endl;

    for(auto flight : flights) {
        processed_flights++;
        if (processed_flights % 100 == 0) {
            cout << "processed " << processed_flights << " / " << total_flights << endl;
        }
        vector< pair<int, PartialSchedule> > next;
        // generate next set of scheduling

        int u = AIRPORT_ID;

        for (auto p : res) {
            PartialSchedule cur = p.second;

            // assign gate to from & to
            int cnt_solution = 0;
            for(int gate = -1; gate < cur.nGate[u]; ++gate) {
                if (cur.canAddFlightToGate(u, gate, flight)) {
                    PartialSchedule added = cur;
                    added.addFlightToGate(u, gate, flight);

                    next.push_back(make_pair(added.score(), added));
                    ++cnt_solution;
                }
            }
        }
        res = next;
        sort(res.begin(), res.end());
        while (res.size() > 1) res.pop_back();
    }

    assert(!res.empty());
    return res[0].second;
}

bool operator < (const PartialSchedule& a, const PartialSchedule& b) {
    return 0;
}

#endif

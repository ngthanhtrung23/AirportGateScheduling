#ifndef SCHEDULE

#define SCHEDULE
struct PartialSchedule {
    map<int, int> nGate;       // Airport --> # gate
    map< pair<int,int>, vector<int> > flights; // Airport + Gate --> flights

    vector<Flight> *allFlight; // Store a reference to all flights

    int countGate() {
        int sumGate = 0;
        for (auto g : nGate)
            sumGate += g.second;
        return sumGate;
    }

    int score() {
        int gap = 0;
        for (auto g : flights) {
            vector<int> flights = g.second;
            int airport_id = g.first.first;

            for (int i = 0; i < (int) flights.size() - 1; ++i) {
                Flight u = (*allFlight)[i];
                Flight v = (*allFlight)[i+1];

                int time_u = (u.origin_airport_id == airport_id) ? u.crs_dep_time.to_int() : u.crs_arr_time.to_int();
                int time_v = (v.origin_airport_id == airport_id) ? v.crs_dep_time.to_int() : v.crs_arr_time.to_int();

                gap += abs(time_u - time_v);
            }
        }
        return countGate() * 1000 + gap;
    }
};

PartialSchedule getRandomSchedule(vector<Flight> flights) {
    PartialSchedule res;
    return res;
}

PartialSchedule getBeamSearchSchedule(vector<Flight> flights) {
    // pair< score, Schedule >
    vector< pair<int, PartialSchedule> > res;

    PartialSchedule empty;
    empty.allFlight = &flights;

    assert(!res.empty());
    return res[0].second;
}

#endif

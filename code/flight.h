#ifndef FLIGHT

#define FLIGHT

#include "template.h"
#include "datetime.h"

struct Flight {
    Date date;
    Time crs_dep_time;
    Time crs_arr_time;

    int get_dep_time() const {
        return crs_dep_time.to_int();
    }
    int get_arr_time() const {
        return crs_arr_time.to_int();
    }

    int airline_id;
    int origin_airport_id;
    int dest_airport_id;
    int dep_delay;         // in minutes
    int arr_delay;         // in minutes
    int cancelled;

    int get_time(int AIRPORT_ID) const {
        if (origin_airport_id == AIRPORT_ID)
            return get_dep_time();
        else return get_arr_time();
    }

    Flight() {}
    Flight(char* buff) {
        // flight date
        int offset = 0, char_read = 0;
        assert(sscanf(buff + offset, "%d-%d-%d,%n", &date.year, &date.month, &date.date, &char_read) == 3);
        offset += char_read;

        // airline id
        assert(sscanf(buff + offset, "%d,%n", &airline_id, &char_read) == 1);
        offset += char_read;

        // origin & dest airport id
        assert(sscanf(buff + offset, "%d,%n", &origin_airport_id, &char_read) == 1);
        offset += char_read;
        assert(sscanf(buff + offset, "%d,%n", &dest_airport_id, &char_read) == 1);
        offset += char_read;

        int t;

        // departure time
        assert(sscanf(buff + offset, "\"%d\",%n", &t, &char_read) == 1);
        offset += char_read;
        crs_dep_time = Time(t);
        assert(sscanf(buff + offset, "%d.00,%n", &dep_delay, &char_read) == 1);
        offset += char_read;

        // arrival time
        assert(sscanf(buff + offset, "\"%d\",%n", &t, &char_read) == 1);
        offset += char_read;
        crs_arr_time = Time(t);
        assert(sscanf(buff + offset, "%d.00,%n", &arr_delay, &char_read) == 1);
        offset += char_read;

        // cancel
        assert(sscanf(buff + offset, "%d.00,%n", &cancelled, &char_read) == 1);
        offset += char_read;
    }
};

bool collides(Flight a, Flight b, int airport, int time_interval) {
    int time_a = (a.origin_airport_id == airport)
                ? a.crs_dep_time.to_int()
                : a.crs_arr_time.to_int();
    int time_b = (b.origin_airport_id == airport)
                ? b.crs_dep_time.to_int()
                : b.crs_arr_time.to_int();

    return abs(time_a - time_b) < time_interval;
}

bool collides_delay(Flight a, Flight b, int airport, int time_interval) {
    int a_from, a_to, b_from, b_to;
    if (a.origin_airport_id == airport) {
        a_from = a.get_dep_time();
        a_to = a.get_dep_time() + a.dep_delay;
    }
    else {
        a_from = a.get_arr_time();
        a_to = a.get_arr_time() + a.arr_delay;
    }
    if (a_from > a_to) swap(a_from, a_to);
    a_to += time_interval;

    if (b.origin_airport_id == airport) {
        b_from = b.get_dep_time();
        b_to = b.get_dep_time() + b.dep_delay;
    }
    else {
        b_from = b.get_arr_time();
        b_to = b.get_arr_time() + b.dep_delay;
    }
    if (b_from > b_to) swap(b_from, b_to);
    b_to += time_interval;

    if (a_to < b_from) return false;
    if (b_to < a_from) return false;
    return true;
}

#endif

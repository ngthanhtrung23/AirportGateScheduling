#ifndef FLIGHT

#define FLIGHT

#include "template.h"
#include "datetime.h"

struct Flight {
    Date date;
    Time crs_dep_time;
    Time crs_arr_time;

    int airline_id;
    int origin_airport_id;
    int dest_airport_id;
    int dep_delay;         // in minutes
    int arr_delay;         // in minutes
    int cancelled;

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

#endif

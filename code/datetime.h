#ifndef DATE_TIME

#define DATE_TIME

struct Date {
    int year, month, date;

    Date() {}
    Date(int year, int month, int date) : year(year), month(month), date(date) {}

    int to_int() { // return some unique int for different date
        return year * 400 + month * 32 + date;
    }
};

struct Time {
    int hour, min;

    Time() {}
    Time(int timestamp) { // in format hhmm, e.g. 1830
        hour = timestamp / 100;
        min = timestamp % 100;
    }

    int to_int() {
        return hour * 60 + min;
    }
};

#endif

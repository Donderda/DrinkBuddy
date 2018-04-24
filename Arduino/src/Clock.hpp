#ifndef CLOCK_H
#define CLOCK_H

#include "DateTime.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

class Clock {

public:
    Clock();

    void setTime(int h, int min, int s);

    void setDate(int day, int month, int year);

    void saveDateTime();

    void printDateTime();

    String getCurrentDateTimeString();

    DateTime getDateTime();

    int getDateTimeInMinutes();

private:
    tmElements_t tm;

    String addLeadingZeroIfNeeded(int number);

    bool getTime(const char *str);

    bool getDate(const char *str);

    const char *monthName[12] = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
};

#endif

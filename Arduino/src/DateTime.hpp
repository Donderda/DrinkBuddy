#ifndef DATETIME_H
#define DATETIME_H

#include <Arduino.h>

struct DateTime {
    int day, month, year, hour, minute, second;
    DateTime() : day(0), month(0), year(0), hour(0), minute(0), second(0) {}
};

#endif

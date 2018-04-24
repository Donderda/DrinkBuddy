#include <Time.h>
#include "Clock.hpp"

Clock::Clock() {
    delay(100);
    if (!RTC.read(tm)) {
        if (RTC.chipPresent()) {
            if (getDate(__DATE__) && getTime(__TIME__)) {
                saveDateTime();
                Serial.println("Time/Date Set!");
            }
        }
    }
}

bool Clock::getTime(const char *str) {
    int Hour, Min, Sec;
    if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
    tm.Hour = Hour;
    tm.Minute = Min;
    tm.Second = Sec;
    return true;
}

bool Clock::getDate(const char *str) {
    char Month[12];
    int Day, Year;
    unsigned int monthIndex;

    if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
    for (monthIndex = 0; monthIndex < 12; monthIndex++) {
        if (strcmp(Month, monthName[monthIndex]) == 0) break;
    }
    if (monthIndex >= 12) return false;
    tm.Day = Day;
    tm.Month = monthIndex + 1;
    tm.Year = CalendarYrToTm(Year);
    return true;
}

void Clock::setDate(int day, int month, int year) {
    this->tm.Day = day;
    this->tm.Month = month;
    this->tm.Year = year-1970;
    this->saveDateTime();
}

void Clock::setTime(int h, int min, int s) {
    this->tm.Hour = h;
    this->tm.Minute = min;
    this->tm.Second = s;
    this->saveDateTime();
}

void Clock::saveDateTime() {
    RTC.write(this->tm);
}

void Clock::printDateTime() {
    Serial.println(this->getCurrentDateTimeString());
}

String Clock::getCurrentDateTimeString() {
    RTC.read(tm);

    String timeString = String(tmYearToCalendar(tm.Year));
    timeString.concat("-");
    timeString.concat(addLeadingZeroIfNeeded(tm.Month));
    timeString.concat("-");
    timeString.concat(addLeadingZeroIfNeeded(tm.Day));
    timeString.concat(" ");
    timeString.concat(addLeadingZeroIfNeeded(tm.Hour));
    timeString.concat(":");
    timeString.concat(addLeadingZeroIfNeeded(tm.Minute));
    timeString.concat(":");
    timeString.concat(addLeadingZeroIfNeeded(tm.Second));
    return timeString;
}

DateTime Clock::getDateTime() {
    DateTime dateTime;
    RTC.read(tm);

    dateTime.year = tmYearToCalendar(tm.Year);
    dateTime.month = tm.Month;
    dateTime.day = tm.Day;
    dateTime.hour = tm.Hour;
    dateTime.minute = tm.Minute;
    dateTime.second = tm.Second;

    return dateTime;
}

String Clock::addLeadingZeroIfNeeded(int number) {
    String ret = "";
    if(number < 10){
        ret = "0";
    }
    ret += number;
    return ret;
}

int Clock::getDateTimeInMinutes() {
    DateTime now = getDateTime();
    return now.hour * 60 + now.minute;
}

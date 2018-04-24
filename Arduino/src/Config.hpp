#ifndef CONFIG_H
#define CONFIG_H

#include "Clock.hpp"
#include <Arduino.h>
#include <WString.h>
#include <OSFS.h>
#include "LED.hpp"
//#include <SoftwareSerial.h>
#include "Buzzer.hpp"

struct ConfigSaveStruct {
    char deviceName[128];
    int brightness;
    int reminderminutes;
    DateTime start;
    DateTime end;
    DateTime lastDrinkTime;
    float minimumLitre;
    float targetLitre;
    float totalDrinkAmount;
    float minimumLitreDynamic;
};

class Config {
private:
    Clock *clock;
    LED *led;
    String inputString = "";
    bool configMode = false;
 //   SoftwareSerial *bluetooth = nullptr;
    ConfigSaveStruct cfg;
    Buzzer *buzzer;

    bool loadConfig();

    void createConfig();

    String getConfigString();

    String addLeadingZeros(int number);

public:
    Config(LED *led, Clock *clock, /*SoftwareSerial *serial, */Buzzer *buzzer);
/*
    bool bluetoothAvailable();

    void setBluetooth(SoftwareSerial *bluetooth);

    SoftwareSerial *getBluetooth() const;
*/
    void handleInput(char inputChar);

    DateTime getStartTime() const {
        return cfg.start;
    }

    float setTotalDrinkAmount(float amount);

    DateTime getEndTime() const {
        return cfg.end;
    }

    float getMinimumlitre() const {
        return getMinimumlitre(false);
    }

    float getMinimumlitre(bool forceConfigValue) const {
        if (forceConfigValue) return cfg.minimumLitre;
        return cfg.minimumLitreDynamic;
    }

    void setMinimumlitre(float minimumlitre) {
        cfg.minimumLitreDynamic = minimumlitre;
    }

    float getTargetlitre() const {
        return cfg.targetLitre;
    }

    const char *getDeviceName() const {
        return cfg.deviceName;
    }

    bool isConfigMode() const;

    bool inTimeWindow();

    /*void println(const String &s);

    void println(const char *c);

    void println(char c);*/

    int getConfigBrightness();

    int getMinutesSinceStart();

    float getShouldDrunkMl();

    float getMLperMinute();

    bool isAfterTime();

    bool isAfterStart();

    void saveConfig();

    void setLastDrinkTime(DateTime time);

    float getDrunkAmount();

    int getLastDrinkTimeInMinutes();

    int getReminderMinutes();

    bool isAfterEnd();

    void saveConfig(bool setBrightness);
};

#endif

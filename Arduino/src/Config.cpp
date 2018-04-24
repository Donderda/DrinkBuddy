
#include "Config.hpp"

#define VERSION "0.2"

Config::Config(LED *led, Clock *clock, /*SoftwareSerial *serial, */Buzzer *buzzer) {
    // this->bluetooth = serial;
    this->led = led;
    this->clock = clock;
    this->buzzer = buzzer;
    if (!this->loadConfig()) {
        this->createConfig();
    }
    this->cfg.lastDrinkTime = clock->getDateTime();
}

bool Config::loadConfig() {
    OSFS::result r = OSFS::getFile("cfg", this->cfg);
    if (r == OSFS::result::NO_ERROR) {
        Serial.println(this->getConfigString());
        this->led->setBrightness(this->cfg.brightness);
        return true;
    } else {
        Serial.println((int) r);
    }
    Serial.println("ErrorCode: " + (int) r);
    return false;
}

void Config::createConfig() {
    Serial.println(F("Creating new config"));
    OSFS::format();
    String name = "DrinkBuddy";
    memcpy(this->cfg.deviceName, name.c_str(), name.length() + 1);
    this->cfg.start.hour = 8;
    this->cfg.start.minute = 0;
    this->cfg.end.hour = 18;
    this->cfg.end.minute = 30;
    this->cfg.minimumLitre = 1;
    this->cfg.minimumLitreDynamic = 1;
    this->cfg.targetLitre = 2.5;
    this->cfg.brightness = 255;
    this->cfg.reminderminutes = -1;
    DateTime empty;
    this->setLastDrinkTime(empty);
    this->saveConfig();
    this->loadConfig();
}

String Config::getConfigString() {
    String ret = ";NAME=";
    ret += this->cfg.deviceName;
    ret += ";STARTTIME=";
    ret += addLeadingZeros(this->cfg.start.hour);
    ret += ":";
    ret += addLeadingZeros(this->cfg.start.minute);
    ret += ";ENDTIME=";
    ret += addLeadingZeros(this->cfg.end.hour);
    ret += ":";
    ret += addLeadingZeros(this->cfg.end.minute);
    ret += ";TARGETLITRE=";
    ret += this->getTargetlitre();
    ret += ";MINIMUMLITRE=";
    ret += this->cfg.minimumLitre;
    ret += ";VERSION=";
    ret += VERSION;
    ret += ";";
    ret += "BRIGHT=";
    ret += this->cfg.brightness;
    ret += ";MINLDYN=";
    ret += this->cfg.minimumLitreDynamic;
    ret += ";RMNDRMNTS=";
    ret += this->cfg.reminderminutes;
    ret += ";";
    return ret;
}

void Config::saveConfig() {
    this->saveConfig(true);
}

void Config::saveConfig(bool setBrightness) {
    Serial.println(F("Saving Config"));
    if (setBrightness)
        this->led->setBrightness(this->cfg.brightness);
    OSFS::format();
    OSFS::newFile("cfg", this->cfg, true);
}

bool Config::isConfigMode() const {
    return configMode;
}

void Config::handleInput(char inputChar) {
    Serial.print(inputChar);
    if (inputChar == '\r' || inputChar == '\n') {
        String input = this->inputString;
        input.trim();
        if (input == "TIME") {
            Serial.println(this->clock->getCurrentDateTimeString());
        }

        if (input == "CONFIG START") {
            this->configMode = true;
        }

        if (this->configMode && input == "CONFIG RESET") {
            this->createConfig();
        }

        if (this->configMode && input == "CONFIG READ") {
            Serial.println(this->getConfigString());
        }

        if (this->configMode && input == "CONFIG LOAD") {
            Serial.println(this->loadConfig());
        }
        if (this->configMode && input == "CONFIG SAVE") {
            this->saveConfig();
        }

        if (this->configMode && input == "CONFIG FINISH") {
            this->configMode = false;
            this->saveConfig();
        }

        if (this->configMode && input.substring(0, 4) == "DATE") {
            int year = input.substring(5, 9).toInt();
            int month = input.substring(10, 12).toInt();
            int day = input.substring(13, 15).toInt();

            this->clock->setDate(day, month, year);
        }


        if (this->configMode && input.substring(0, 4) == "TIME") {
            int hour = input.substring(5, 7).toInt();
            int minute = input.substring(8, 10).toInt();
            int second = input.substring(11, 13).toInt();
            this->clock->setTime(hour, minute, second);
        }


        if (this->configMode && input.substring(0, 15) == "REMINDERMINUTES") {
            int minutes = input.substring(16).toInt();
            this->cfg.reminderminutes = minutes;
        }

        if (this->configMode && input.substring(0, 4) == "NAME") {
            String name = input.substring(5);
            memcpy(this->cfg.deviceName, name.c_str(), name.length() + 1);
        }


        if (this->configMode && input.substring(0, 11) == "TARGETLITRE") {
            this->cfg.targetLitre = input.substring(12).toFloat();
        }

        if (this->configMode && input.substring(0, 12) == "MINIMUMLITRE") {
            this->cfg.minimumLitre = input.substring(13).toFloat();
            this->cfg.minimumLitreDynamic = cfg.minimumLitre;
        }

        if (this->configMode && input.substring(0, 9) == "STARTTIME") {
            int hour = static_cast<int>(input.substring(10, 12).toInt());
            int minute = static_cast<int>(input.substring(13, 15).toInt());
            this->cfg.start.hour = hour;
            this->cfg.start.minute = minute;
        }

        if (this->configMode && input.substring(0, 7) == "ENDTIME") {
            int hour = static_cast<int>(input.substring(8, 10).toInt());
            int minute = static_cast<int>(input.substring(11, 13).toInt());
            this->cfg.end.hour = hour;
            this->cfg.end.minute = minute;
        }

        if (this->configMode && input.substring(0, 10) == "BRIGHTNESS") {
            int brightness = static_cast<int>(input.substring(11).toInt());
            this->cfg.brightness = brightness;
        }

        this->inputString = "";
        return;
    }
    this->inputString += inputChar;
}

int Config::getReminderMinutes() {
    return cfg.reminderminutes;
}

/*
void Config::setBluetooth(SoftwareSerial *bluetooth) {
    Config::bluetooth = bluetooth;
}

SoftwareSerial *Config::getBluetooth() const {
    return bluetooth;
}

bool Config::bluetoothAvailable() {
    if (this->bluetooth == nullptr) return false;
    return static_cast<bool>(bluetooth->available());
}

void Config::println(const String &s) {
    Serial.println(s);
    if (this->bluetoothAvailable())
        this->bluetooth->println(s);
}

void Config::println(const char c[]) {
    Serial.println(c);
    if (this->bluetoothAvailable())
        this->bluetooth->println(c);
}

void Config::println(char c) {
    Serial.println(c);
    if (this->bluetoothAvailable())
        this->bluetooth->println(c);
}
*/
String Config::addLeadingZeros(int number) {
    String ret = "";
    if (number < 10) {
        ret = "0";
    }
    ret += String(number);
    return ret;
}

bool Config::inTimeWindow() {
    DateTime now = clock->getDateTime();
    int nowInMinutes = now.hour * 60 + now.minute;
    int startInMinutes = cfg.start.hour * 60 + cfg.start.minute;
    int endInMinutes = cfg.end.hour * 60 + cfg.end.minute - 1;
    return nowInMinutes >= startInMinutes && nowInMinutes <= endInMinutes;
}

int Config::getMinutesSinceStart() {
    DateTime now = clock->getDateTime();
    int nowInMinutes = now.hour * 60 + now.minute;
    int startInMinutes = cfg.start.hour * 60 + cfg.start.minute;
    return nowInMinutes - startInMinutes;
}

float Config::getShouldDrunkMl() {
    return getMLperMinute() * getMinutesSinceStart();
}

float Config::getMLperMinute() {
    int startInMinutes = cfg.start.hour * 60 + cfg.start.minute;
    int endInMinutes = cfg.end.hour * 60 + cfg.end.minute - 1;
    int totalMinutes = endInMinutes - startInMinutes;

    float ml = this->cfg.minimumLitre * 1000;

    return ml / totalMinutes;
}

bool Config::isAfterTime() {
    DateTime now = clock->getDateTime();
    int nowInMinutes = now.hour * 60 + now.minute;
    int endInMinutes = cfg.end.hour * 60 + cfg.end.minute - 1;
    return endInMinutes < nowInMinutes;
}

int Config::getConfigBrightness() {
    return this->cfg.brightness;
}

bool Config::isAfterStart() {
    DateTime now = clock->getDateTime();
    int nowInMinutes = now.hour * 60 + now.minute;
    int startInMinutes = cfg.start.hour * 60 + cfg.start.minute;
    return (startInMinutes < nowInMinutes);
}

float Config::setTotalDrinkAmount(float amount) {
    this->cfg.totalDrinkAmount = amount;
    return amount;
}

void Config::setLastDrinkTime(DateTime time) {
    this->cfg.lastDrinkTime = time;
}

float Config::getDrunkAmount() {
    DateTime now = clock->getDateTime();
    if (now.year != cfg.lastDrinkTime.year ||
        now.month != cfg.lastDrinkTime.month ||
        now.day != cfg.lastDrinkTime.day) {
        return 0.0;
    }

    return cfg.totalDrinkAmount;
}

int Config::getLastDrinkTimeInMinutes() {
    return cfg.lastDrinkTime.hour * 60 + cfg.lastDrinkTime.minute - 1;

}

bool Config::isAfterEnd() {
    DateTime now = clock->getDateTime();
    int nowInMinutes = now.hour * 60 + now.minute;
    int endInMinutes = cfg.end.hour * 60 + cfg.end.minute;
    return (endInMinutes < nowInMinutes);
}

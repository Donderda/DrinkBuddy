#include <Arduino.h>
#include "Clock.hpp"
#include "DateTime.hpp"
#include "Config.hpp"
#include "OSFSSetup.h"
#include "LED.hpp"
#include "Scale.hpp"
//#include "SoftwareSerial.h"
#include "Buzzer.hpp"

#define SCALE_DT A1
#define SCALE_SCK A0
#define BTrxPin 8
#define BTtxPin 9
#define BuzzerPort 5

Clock *clock;
Config *config;
LED *led;
Scale *scale;
//SoftwareSerial BTSerial(BTrxPin, BTtxPin);

Buzzer *buzzer;

float lastValue = 0;
float totalDrunk = 0;
int tooMuchCounter = 0;

bool minimumAmountWasRecalculated = false;
bool bootup = false;
bool maintanenceWasDone = false;

void checkDrinkAmount();

void performMaintenance();

void checkLastDrinkTime();

void setup() {
    Serial.begin(19200);
    Serial.println(F("DrinkBuddy"));
    buzzer = new Buzzer(BuzzerPort);
    delay(100);
    led = new LED();
    scale = new Scale(SCALE_DT, SCALE_SCK);
    clock = new Clock();
    //BTSerial.begin(9600);
    config = new Config(led, clock, /*&BTSerial, */buzzer);
    totalDrunk = config->getDrunkAmount();
    led->animate(CRGB::Green, false);
    buzzer->playBootSound();
    bootup = true;
}

void loop() {
    /*if (BTSerial.available()) {
        char inputChar = (char) BTSerial.read();
        config->handleInput(inputChar);
    }*/

    if (config->isConfigMode()) {
        led->setBrightness(255);
        led->fill(CRGB::Blue);
        return;
    }

    if (bootup) {
        if (!config->inTimeWindow()) {
            performMaintenance();
            led->setBrightness(0);
            return;
        }

        if (config->isAfterStart() && !config->isAfterEnd() && minimumAmountWasRecalculated && !maintanenceWasDone) {
            minimumAmountWasRecalculated = false;
            maintanenceWasDone = true;
        }

        if (totalDrunk == 0) {
            led->setPercent(0);
        }

        led->setBrightness(config->getConfigBrightness());

        checkDrinkAmount();
        checkLastDrinkTime();
        delay(100);
    }
}

void checkLastDrinkTime() {
    if (config->inTimeWindow()) {
        maintanenceWasDone = false;
        int reminderMinutes = config->getReminderMinutes();
        if (reminderMinutes > 0) {
            if ((clock->getDateTimeInMinutes() - config->getLastDrinkTimeInMinutes()) > reminderMinutes) {
                buzzer->beep(1047, 200);
                led->animate(CRGB::Yellow, true);
                config->setLastDrinkTime(clock->getDateTime());
            }
        }
    }
}

void performMaintenance() {
    if (config->isAfterTime()) {
        if (!minimumAmountWasRecalculated) {
            if (totalDrunk > config->getMinimumlitre()) {
                float minimumLitre = config->getMinimumlitre();
                if (totalDrunk < config->getTargetlitre()) {
                    if (minimumLitre * 1.10 > config->getTargetlitre()) {
                        config->setMinimumlitre(config->getTargetlitre());
                    } else {
                        config->setMinimumlitre(minimumLitre * 1.10);
                    }
                }
                buzzer->playWinningSound();
                led->fillAnimated(CRGB::Green);
                led->fillAnimated(CRGB::Blue);
                led->fillAnimated(CRGB::Green);
            } else {
                float configuredMinimumLitre = config->getMinimumlitre(true);
                if (config->getMinimumlitre() * 0.9 < configuredMinimumLitre) {
                    config->setMinimumlitre(configuredMinimumLitre);
                } else {
                    config->setMinimumlitre(config->getMinimumlitre() * 0.9);
                }
                buzzer->playLosingSound();
                led->fillAnimated(CRGB::Red);
                led->fillAnimated(CRGB::Blue);
                led->fillAnimated(CRGB::Red);
            }
            DateTime empty;
            minimumAmountWasRecalculated = !minimumAmountWasRecalculated;
            config->setLastDrinkTime(empty);
            totalDrunk = config->setTotalDrinkAmount(0.0);
            config->saveConfig(false);
            led->setBrightness(0);
        }
    }
}

void checkDrinkAmount() {
    float readVal = scale->readValues();
    if ((int) readVal && readVal < lastValue) {
        float diff = (lastValue - readVal);

        // if the diff is greater than 20% of the lastValue, wait 3 times. Then accept the new value...
        if (diff / lastValue > 0.2f) {
            if (tooMuchCounter < 3) {
                delay(5000);
                tooMuchCounter++;
                return;
            }
        }
        tooMuchCounter = 0;
        lastValue = readVal;
        config->setLastDrinkTime(clock->getDateTime());
        totalDrunk = config->setTotalDrinkAmount(totalDrunk + diff);
        Serial.print(F("Total drunk: "));
        Serial.println(totalDrunk);
        config->saveConfig(false);
    } else if (readVal > lastValue) {
        Serial.print("New Scale value: ");
        Serial.println(readVal);
        lastValue = readVal;
    }
    led->setPercent(static_cast<int>(totalDrunk * 100 / config->getShouldDrunkMl()));
}

void serialEvent() {
    while (Serial.available()) {
        auto inputChar = (char) Serial.read();
        //BTSerial.write(inputChar);
        config->handleInput(inputChar);
    }
}

#ifndef LED_H
#define LED_H

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define NUM_LEDS 16
#define LED_PIN 2

class LED {
private:
    CRGB leds[NUM_LEDS];
    unsigned int brightness = 5;
    int percent = 0;
public:
    LED();

    void setPercent(int percent);

    void setBrightness(int brightness);

    void animate(CRGB color);

    void animate(CRGB color, bool keepOldColor);

    void fill(CRGB color);

    void fillAnimated(CRGB color);
};

#endif

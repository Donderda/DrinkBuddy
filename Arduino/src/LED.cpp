#include "LED.hpp"

LED::LED() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(this->leds, NUM_LEDS);
}

void LED::animate(CRGB color) {
    this->animate(color, false);
}

void LED::animate(CRGB color, bool keepOldColor) {
    if (!keepOldColor) {
        for (auto &led : leds) {
            led = CRGB::Black;
        }
        FastLED.delay(50);
    }
    for (auto &led : leds) {
        led = color;
        FastLED.delay(50);
        if (!keepOldColor) {
            led = CRGB::Black;
        }
    }
    FastLED.delay(50);
    if (!keepOldColor) {
        leds[NUM_LEDS] = CRGB::Black;
    }
    FastLED.setBrightness(this->brightness);
}

void LED::fill(CRGB color){
    fill_solid(this->leds, NUM_LEDS, color);
    FastLED.setBrightness(this->brightness);
    FastLED.show();
}

void LED::fillAnimated(CRGB color){
    for (auto &led : leds) {
        led = color;
        FastLED.delay(50);
    }
    FastLED.setBrightness(this->brightness);
    FastLED.show();
}

void LED::setPercent(int percent) {
    if(percent > 120) percent = 120;
    this->percent = percent;
    CHSV newColor(static_cast<unsigned int>(percent), 255, 255);
    //fill_solid(this->leds, NUM_LEDS, newColor);
    this->fillAnimated(newColor);
    FastLED.setBrightness(this->brightness);
    if(percent == 0){
        for (auto &led : leds) {
            led = CRGB::Red;
        }
    }
    FastLED.show();
}

/*

void LED::blink(CRGB color, int repetitions) {
    this->blink(color, repetitions, true);
}

void LED::blink(CRGB color, int repetitions, bool returnToOldColor) {
    for (auto &led : leds) {
        led = CRGB::Black;
    }
    FastLED.setBrightness(this->brightness);
    for (int i = 0; i < repetitions; i++) {
        for (auto &led : leds) {
            led = color;
        }
        FastLED.delay(100);
        for (auto &led : leds) {
            led = CRGB::Black;
        }
        FastLED.delay(100);
    }
    if(returnToOldColor) {
        this->setPercent();
    }
}

void LED::blink(CRGB color) {
    this->blink(color, 2);
}
*/

void LED::setBrightness(int brightness) {
    if(brightness == 0){
        this->fillAnimated(CRGB::Black);
    }
    this->brightness = brightness;
    FastLED.setBrightness(this->brightness);
}

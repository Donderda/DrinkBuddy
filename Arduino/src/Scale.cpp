#include "Scale.hpp"

Scale::Scale(int dt, int sck) {
    this->adc = new HX711(dt, sck);
    this->adc->set_scale(CALIBRATION_FACTOR);
    this->adc->tare();
}

int Scale::readValues() {
    float avgVal = 0.0;
    this->adc->power_up();
    avgVal = this->adc->get_units(20);
    this->adc->power_down();
    if (avgVal < 1) return 0;
    return (int) avgVal;
}

Scale::~Scale() {
    delete adc;
}

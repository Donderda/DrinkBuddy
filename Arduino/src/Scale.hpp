#ifndef SCALE_H
#define SCALE_H

#include <Arduino.h>
#include "HX711.h"

#define CALIBRATION_FACTOR 1220.f

class Scale {
private:
    HX711 *adc;
public:
    Scale(int DT, int SCK);
    ~Scale();
    int readValues();
};

#endif

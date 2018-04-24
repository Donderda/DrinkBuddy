#ifndef BUZZER_H
#define BUZZER_H


#include <Arduino.h>
#include "pitches.hpp"

class Buzzer {
private:
   int boot_note[7] = {
            NOTE_A4, NOTE_A4, NOTE_G4, NOTE_FS4,
            NOTE_G4, NOTE_E4, 0
    };
    int boot_duration[7] = {
            8, 16, 16, 8,
            4, 4, 2
    };

    int win_note[4] {
            NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5
    };

    int win_duration[4] = {
            4, 4, 4, 2
    };

    unsigned int pin;
public:
    Buzzer(unsigned int port);

    void beep();

    void playWinningSound();

    void playLosingSound();

    void playBootSound();

    void beep(unsigned int  freq, unsigned int  duration);

    void beep(unsigned int  freq);
};

#endif

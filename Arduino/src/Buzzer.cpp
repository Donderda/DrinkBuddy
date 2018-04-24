#include "Buzzer.hpp"

Buzzer::Buzzer(unsigned int port) {
    this->pin = port;
    pinMode(port, OUTPUT);
}

void Buzzer::beep(unsigned int freq, unsigned int duration) {
    tone(this->pin, freq, duration);
}

void Buzzer::beep(unsigned int freq) {
    this->beep(freq, 1000);
}

void Buzzer::beep() {
    this->beep(1000);
}

void Buzzer::playWinningSound() {
    for (int thisNote = 0; thisNote < (sizeof(win_note) / sizeof(unsigned int)); thisNote++) {

        int noteDuration = 1000 / win_duration[thisNote];
        tone(this->pin, win_note[thisNote], noteDuration);

        unsigned long pauseBetweenNotes = static_cast<unsigned long>(noteDuration * 1.8);
        delay(pauseBetweenNotes);
        noTone(this->pin); //stop music on pin 8
    }
}

void Buzzer::playLosingSound() {
    tone(this->pin, NOTE_G4);
    delay(250);
    tone(this->pin, NOTE_C4);
    delay(500);
    noTone(this->pin);
}

void Buzzer::playBootSound() {
    for (int thisNote = 0; thisNote < (sizeof(boot_note) / sizeof(unsigned int)); thisNote++) {

        int noteDuration = 1000 / boot_duration[thisNote];
        tone(this->pin, boot_note[thisNote], noteDuration);

        unsigned long pauseBetweenNotes = static_cast<unsigned long>(noteDuration * 1.8);
        delay(pauseBetweenNotes);
        noTone(this->pin); //stop music on pin 8
    }
}

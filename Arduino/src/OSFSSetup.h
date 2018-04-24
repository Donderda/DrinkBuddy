#include "OSFS.h"
#include "EEPROM.h"

uint16_t OSFS::startOfEEPROM = 1;
uint16_t OSFS::endOfEEPROM = 768;

void OSFS::readNBytes(uint16_t address, unsigned int num, void *output) {
    for (int i = address; i < address + num; i++) {
        *((byte *) output) = EEPROM.read(i);
        output++;
    }
}

void OSFS::writeNBytes(uint16_t address, unsigned int num, const void *input) {
    for (int i = address; i < address + num; i++) {
        EEPROM.update(i, *((byte *) input));
        input++;
    }
}

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#include <OSFS.h>
#include "EEPROM.h"

class FileSystem {
public:

    FileSystem() {

    }

    String loadString() {
        Serial.println(F("Looking for testStr..."));
        OSFS::result r;
        const OSFS::result noerr = OSFS::result::NO_ERROR;
        const OSFS::result notfound = OSFS::result::FILE_NOT_FOUND;

        char testStr[1024];
        unsigned int  filePtr, fileSize;
        r = OSFS::getFileInfo("config", filePtr, fileSize);

        if (r == notfound)
            Serial.println(F("Not found"));
        else if (r == noerr) {

            OSFS::readNBytes(filePtr, fileSize, testStr);
            Serial.println(testStr);

        } else {
            Serial.print(F("Error: "));
            Serial.println( (int) r );
        }

        String ret = String(testStr);
        Serial.println("Saved String:" + ret);
        return ret;

    }

    bool saveString(String &config) {
        Serial.println("Config2Save:");
        char *testStr = config.c_str();
        OSFS::newFile("testStr", config.c_str(), config.length(), true);

        return true;
    }
};

#endif
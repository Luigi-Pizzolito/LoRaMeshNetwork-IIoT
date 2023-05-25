#pragma once

#include <TinyGPSPlus.h>
#define GPS_BAUD 9600
#define GPS_LOCK_TIMEOUT 60000
#include <ESP32Time.h>

// todo: add the position info to flash or rtc memory to retain through sleep cycles.

class LowPowerGPS {
    public:
        LowPowerGPS();
        void update();
        double lat = 0;
        double lng = 0;
        ESP32Time rtc;
        void printDT();
    private:
        TinyGPSPlus _gps;
        TinyGPSTime _time;
        TinyGPSDate _date;
        void _init();
        void _fix();
        unsigned long _startT = 0;
        void _process();
        bool _locked = false;
        void _gpsOn();
        void _gpsOff();
        void _sendUBX(uint8_t *msg, uint8_t len);
        void _time2RTC();
};
#include "LowPowerGPS.h"

LowPowerGPS::LowPowerGPS()
{
    _init();
}

void LowPowerGPS::update()
{
    _locked = false;
    _fix();
}

void LowPowerGPS::_init()
{
    Serial2.begin(GPS_BAUD);
    _fix();
}

void LowPowerGPS::_fix()
{
    _gpsOn();
    delay(3000);
    _startT = millis();
    while (1 /*_locked == false && millis() < _startT + GPS_LOCK_TIMEOUT*/);
    {
        _process();
    }
    
    // while (!_locked)
    // {

    //     Serial.println("Searching for satellites.");
    //     _process();
    // }

    if (_locked == false && millis() >= _startT + GPS_LOCK_TIMEOUT)
    {
        Serial.println("GPS Lock Timeout. Position not updated.");
        // while (1)
        //     delay(10);
    }
}

void LowPowerGPS::_process()
{
    while (Serial2.available() > 0 /*&& _locked == false && millis() < _startT + GPS_LOCK_TIMEOUT*/)
    {
        if (_gps.encode(Serial2.read()))
        {
            // Recieved GPS packet
            if (_gps.location.isValid()/* && _gps.time.isValid() && _gps.date.isValid()*/)
            {
                // Location fixed
                _locked = true;
                lat = _gps.location.lat();
                lng = _gps.location.lng();
                _date = _gps.date;
                _time = _gps.time;
                _time2RTC();

                Serial.print("GPS Location & Time Locked: ");
                printDT();

                // _gpsOff();
                // delay(3000);
            }
            else
            {
                Serial.println("bad pkt");
            }
            Serial.println("Looking for GPS stelites to lock onto.");
        }
        Serial.println("m");
    }

    if (millis() > 5000 && _gps.charsProcessed() < 10 && _locked == false)
    {
        Serial.println("GPS Init Failed. Check wiring, no packets recieved.");
        while (true)
            delay(10);
    }
}

void LowPowerGPS::_sendUBX(uint8_t *msg, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        Serial2.write(msg[i]);
    }
    Serial2.println();
}

void LowPowerGPS::_gpsOff()
{
    // ---- Set GPS to backup mode (sets it to never wake up on its own)
    uint8_t GPSoff[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B};
    _sendUBX(GPSoff, sizeof(GPSoff) / sizeof(uint8_t));
    Serial.println(F("GPS power OFF"));
    //! still draws around 5mA
}

void LowPowerGPS::_gpsOn()
{
    // ---- Restart GPS
    uint8_t GPSon[] = {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4C, 0x37};
    _sendUBX(GPSon, sizeof(GPSon) / sizeof(uint8_t));
    Serial.println(F("GPS power ON"));
    //! draws around 65mA
}

void LowPowerGPS::_time2RTC()
{
    rtc.setTime(_time.second(), _time.minute(), _time.hour(), _date.day(), _date.month(), _date.year(), _time.centisecond() * 10000);
}

void LowPowerGPS::printDT()
{
    Serial.print(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
    Serial.print(" ");
    Serial.print(lat, 6);
    Serial.print(", ");
    Serial.println(lng, 6);
}
#pragma once
// Sensors
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
// Data Packing
#include <ArduinoJson.h>

#define JSON_PKT_MAX_SIZE 128

class NodeSensors {
    public:
        NodeSensors(uint8_t NodeID);
        String data();
        String printData();

    private:
        void _getSensorData();

        Adafruit_AHTX0 aht;
        void _AHT20_setup();
        Adafruit_BMP280 bmp;
        void _BMP280_setup();

        StaticJsonDocument<JSON_PKT_MAX_SIZE> _pkt;
        uint8_t NodeID;
};
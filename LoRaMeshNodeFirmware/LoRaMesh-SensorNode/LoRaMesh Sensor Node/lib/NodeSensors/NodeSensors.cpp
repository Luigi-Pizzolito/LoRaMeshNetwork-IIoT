#include "NodeSensors.h"

// **** Constructor ***

NodeSensors::NodeSensors(uint8_t NodeID)
{
    // save NodeID for packet generation
    NodeID = NodeID;
    // init sensors
    _AHT20_setup();
    _BMP280_setup();
}

// **** Serialise Data ****

String NodeSensors::data()
{
    // Get sensor data
    _getSensorData();
    // Serialise JSON document to MsgPack
    String out;
    serializeMsgPack(_pkt, out);
    return out;
}

String NodeSensors::printData()
{
    // Get sensor data
    _getSensorData();
    // Serialise JSON document to JSON
    String out;
    serializeJson(_pkt, out);
    return out;
}

// **** Get Data ****

void NodeSensors::_getSensorData()
{
    float tempA, temp1, temp2, baro, humi;
    // AHT20
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    temp1 = temp.temperature;
    humi = humidity.relative_humidity;
    // BMP280
    temp2 = bmp.readTemperature();
    baro = bmp.readPressure();
    // Temp Fusion
    tempA = (temp1 + temp2) / 2.0f;
    // Push data to JSON document
    JsonObject data = _pkt.createNestedObject(NodeID);
    data["t"] = tempA;
    data["p"] = baro;
    data["h"] = humi;
}

// **** Init Sensors ****

void NodeSensors::_AHT20_setup()
{
    // init AHT20
    if (!aht.begin())
    {
        Serial.println(F("Could not find AHT? Check wiring,"));
        while (1)
            delay(10);
    }
}

void NodeSensors::_BMP280_setup()
{
    // init BMP280
    unsigned status = bmp.begin();
    if (!status)
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                         "try a different address!"));
        Serial.print(F("SensorID was: 0x"));
        Serial.println(bmp.sensorID(), 16);
        Serial.print(F("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n"));
        Serial.print(F("        ID of 0x56-0x58 represents a BMP 280,\n"));
        Serial.print(F("        ID of 0x60 represents a BME 280.\n"));
        Serial.print(F("        ID of 0x61 represents a BME 680.\n"));
        while (1)
            delay(10);
    }
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,   /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X16,  /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,  /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,    /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_1); /* Standby time. */
}
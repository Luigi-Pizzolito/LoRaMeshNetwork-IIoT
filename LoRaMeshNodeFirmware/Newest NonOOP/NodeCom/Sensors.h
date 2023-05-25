#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

//for sensor AHT20+BMP280
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

//usefull c++ libraries
#include <string>
#include <cstdint>
#include <cstring>


class Sensors
{
    private:
    Adafruit_AHTX0 aht;
    Adafruit_BMP280 bmp;


    public:
    Sensors();
    int init();
    std::string getData(char a = 'A'); // 'A' for all, 'T' for temperature, 
                                       //'H' for humidity, 'P' for pressure
    


};















#endif
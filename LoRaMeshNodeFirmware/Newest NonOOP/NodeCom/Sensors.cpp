#include "Sensors.h"


Sensors::Sensors()
{

}

int Sensors::init()  // return 0 if no problem, 1 if AHT20 dont work, 2 if BMP280 dont work
{                    // and 3 if both dont work
    
    //AHT20 and BMP280 test
    if(!aht.begin() && !bmp.begin()) 
        return 3;
    else if(!aht.begin())
        return 1;
    else
        return 2;
    
    //BMP280 setup
    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    return 0;
}



std::string Sensors::getData(char a ) // 'A' for all, 'T' for temperature, 
{                                     //'H' for humidity, 'P' for pressure
    std::string json;
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data


    if(a == 'A')
    {
        json = "{\"T\":" + std::to_string(temp.temperature) + 
               ",\"H\":" + std::to_string(humidity.relative_humidity) +
               ",\"P\":" + std::to_string(bmp.readPressure()) +"}";   
        return json;                  
    }

    else if(a == 'T')
    {
        json = "{\"T\":" + std::to_string(temp.temperature) + "}";   
        return json; 
    }

    else if(a == 'H')
    {
        json = "{\"H\":" + std::to_string(humidity.relative_humidity) +"}";   
        return json; 
    }

    else if(a == 'P')
    {
        json = "{\"P\":" + std::to_string(bmp.readPressure()) +"}";   
        return json; 
    } 

}   
                                
    

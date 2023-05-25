#pragma once
/*
SensorDataDB Module

created 27 Mar 2023
by Luigi Pizzolito
*/

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

class SensorDataDB
{
public:
    SensorDataDB();
    void addPacket(uint8_t *pktbuff);
    void clearDB();
    // Exports
    String exportMsgPack();
    String exportJSON();

private:
    // DB memory object
    StaticJsonDocument<600> _db;
    // Initisliase DB as JSON array
    JsonArray _data = _db.to<JsonArray>();
};
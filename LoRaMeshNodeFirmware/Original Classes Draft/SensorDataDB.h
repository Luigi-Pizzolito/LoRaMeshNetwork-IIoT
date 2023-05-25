#ifndef SensorDataDB_h
#define SensorDataDB_h
/*
SensorDataDB Module

created 27 Mar 2023
by Luigi Pizzolito
*/

#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

class SensorDataDB
#ifndef ESP32
: public Printable 
#endif
{
public:
  SensorDataDB();
  void addPacket(uint8_t* pktbuff);
  void clearDB(); 
  // Exports
  String exportMsgPack();
  String exportJSON();
  #ifndef ESP32
  size_t printTo(Print&) const;
  #endif
private:
  // DB memory object
  StaticJsonDocument<600> _db;
  // Initisliase DB as JSON array
  JsonArray _data = _db.to<JsonArray>();
  
};

SensorDataDB::SensorDataDB() {
  // test MsgPack parse
  uint8_t input[] = { 131, 166, 115, 101, 110, 115, 111, 114, 163, 103, 112, 115,
                      164, 116, 105, 109, 101, 206, 80, 147, 50, 248, 164, 100,
                      97, 116, 97, 146, 203, 64, 72, 96, 199, 58, 188, 148,
                      112, 203, 64, 2, 106, 146, 230, 33, 49, 169 };
  addPacket(input);
  delay(1000);
  // iterate over DB objects
  for (JsonObject pkt : _data) {
    // if not null / duplicate
    if (pkt) {
      // Serial.println(pkt["time"].as<int>());
      serializeJson(pkt, Serial);
      Serial.println("-");
    }
  }
  delay(250);
  // serializeJson(_data, Serial);
  Serial.println(exportJSON());// Serial.println(*this);
  // Serial.println(_exportMsgPack());
  delay(250);
  // clearDB();
  Serial.println(exportJSON());// Serial.println(*this);
  delay(250);
  // addPacket(input);
  // StaticJsonDocument<200> pkt;
  // deserializeJson(pkt, "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.75608,2.302038]}");
  // _data.add(pkt);
  StaticJsonDocument<256> pkt2;
  deserializeJson(pkt2, "{\"sensor\":\"spg\",\"time\":135182320,\"data\":[48.00,2.543]}");
  _data.add(pkt2);
  deserializeJson(pkt2, "{\"sensor\":\"gps\",\"time\":45671,\"data\":[48.00,46.32]}");
  _data.add(pkt2);
  delay(250);
  // iterate over DB objects
  for (JsonObject pkt : _data) {
    // if not null / duplicate
    if (pkt) {
      // Serial.println(pkt["time"].as<int>());
      serializeJson(pkt, Serial);
      Serial.println("-");
    }
  }
  Serial.println(exportJSON());// Serial.println(*this);
}

void SensorDataDB::addPacket(uint8_t* pktbuff) {
  // Deserialise into pkt
  StaticJsonDocument<256> pkt;
  DeserializationError error = deserializeMsgPack(pkt, pktbuff);
  // Test if parsing succeeded.
  if (error) {
    Serial.print("deserializeMsgPack() failed: ");
    Serial.println(error.f_str());
    return;
  }
  // push to array
  _data.add(pkt);
}


String SensorDataDB::exportJSON() {
  String out = "";
  serializeJson(_data, out);
  return out;  
}

String SensorDataDB::exportMsgPack() {
  String out = "";
  serializeMsgPack(_data, out);
  return out;
}

void SensorDataDB::clearDB() {
  _db.clear();
  _data = _db.to<JsonArray>();
}

#ifndef ESP32
// return JSON string if class is printed
size_t SensorDataDB::printTo(Print& p) const {
  return p.print(exportJSON());
}
#endif

#endif
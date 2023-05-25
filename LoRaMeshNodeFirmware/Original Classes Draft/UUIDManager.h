#ifndef UUIDManager_h
#define UUIDManager_h
/*
UUID Manager Module
Generates UUID if not existing, stores to EEPROM. Consisten UUIDs per device.

created 27 Mar 2023
by Luigi Pizzolito
*/

#include <UUID.h>

#ifndef ESP32
  #define ADC_PIN A0
#endif

// #define CLR  // Uncomment this and run to clear flag and generate new UUID

#ifdef ESP32

#include <Preferences.h>
Preferences preferences;

class UUIDMan {
public:
  UUIDMan();
  String uuid;
  uint8_t intID();
  uint8_t intChipID();
private:
  UUID _uuidgen;
};

UUIDMan::UUIDMan() {
  preferences.begin("uuid-man", false);
// Clear last UUID flag if requested
#ifdef CLR
  preferences.putString("uuid", NULL);
#endif
  // check if UUID already exists in EEPROM
  uuid = preferences.getString("uuid");
  if (uuid == NULL) {
    // Generate new UUID, and store it
    _uuidgen.setRandomMode();
    _uuidgen.seed(esp_random());  // seed RNG with ESP32 hardware RNG
    _uuidgen.generate();
    uuid = _uuidgen.toCharArray();
    preferences.putString("uuid", uuid);
  }
}

uint8_t UUIDMan::intID() {
  // take first two hex chars of UUID and convert to uint8_t for a UUID between 0 and 255
  return (int)strtol(uuid.substring(0,2).c_str(), 0, 16);
}

uint8_t UUIDMan::intChipID() {
  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}
  return (chipId & 0x000000ff);
}

#else

#include <EEPROM.h>

class UUIDMan : public Printable {
public:
  UUIDMan();
  size_t printTo(Print&) const;
  String uuid;
private:
  UUID _uuidgen;
  void _Str2EEPROM(char add, String data);
  String _EEPROM2Str(char add);
};

UUIDMan::UUIDMan() {
// clear last UUID flag if requested
#ifdef CLR
  EEPROM.write(0, 0x00);
#endif
  // check if UUID already exists in EEPROM
  // by reading if first memory address == GEN
  // Serial.println(EEPROM.read(0), HEX);
  if (EEPROM.read(0) != 0x00) {
    // UUID already generated, load it
    uuid = _EEPROM2Str(0);
  } else {
    // Generate new UUID, and store it
    _uuidgen.setRandomMode();
    _uuidgen.seed(analogRead(ADC_PIN));  // seed RNG with analog read
    _uuidgen.generate();
    uuid = _uuidgen.toCharArray();
    _Str2EEPROM(0, uuid);
  }
}

// write string to EEPROM
void UUIDMan::_Str2EEPROM(char add, String data) {
  int _size = data.length();
  int i;
  for (i = 0; i < _size; i++) {
    EEPROM.write(add + i, data[i]);
  }
  EEPROM.write(add + _size, '\0');  //Add termination null character for String Data
}

// read string from EEPROM
String UUIDMan::_EEPROM2Str(char add) {
  int i;
  char data[100];  //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500)  //Read until null character
  {
    k = EEPROM.read(add + len);
    data[len] = k;
    len++;
  }
  data[len] = '\0';
  return String(data);
}

// return uuid if class is printed
size_t UUIDMan::printTo(Print& p) const {
  return p.print(uuid);
}

#endif

#endif
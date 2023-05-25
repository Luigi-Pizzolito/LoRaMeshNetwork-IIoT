#include "RadioSensor.h"

// **** Log Functions ****
namespace Logger
{
    void log(String val)
    {
        char buf[val.length()+16];
        sprintf(buf, "[   %02X] %s", NODE_ID, val);
        Serial.println(buf);
        delete[] buf;
    }

    void logPkt(uint8_t from, int16_t rssi, int snr, uint8_t* strbuf) {
        char buf[sizeof(strbuf)+32];
        sprintf(buf, "[%02X<-%02X] [%d/%d] %s", NODE_ID, from, rssi, snr, (char*)strbuf);
        Serial.println(buf);
        delete[] buf;
    }

    void logPkt(uint8_t to, uint8_t* strbuf) {
        char buf[sizeof(strbuf)+32];
        sprintf(buf, "[%02X->%02X] [---/---] %s", NODE_ID, to, (char*)strbuf);
        Serial.println(buf);
        delete[] buf;
    }

    void logPkt(uint8_t to, String str) {
        char buf[str.length()+32];
        sprintf(buf, "[%02X->%02X] [---/---] %s", NODE_ID, to, str);
        Serial.println(buf);
        delete[] buf;
    }
}

// **** Constructor ****
RadioSensor::RadioSensor() {
    _sx1278_setup();
}

// **** Init Radio Module ****
void RadioSensor::_sx1278_setup() {
    Logger::log("Initialising Radio Node.");
    if (!_mesh.init()) {
        Logger::log("Init Failed.");
        while (1)
            delay(10);
    }

    Logger::log("Configuring SX1278.");
    _rf95.setTxPower(16, false); //* from +2 to +20, warning 20dBm gets VERY HOT! Default is 13dBm.
    _rf95.setFrequency(433.0);
    _rf95.setCADTimeout(500);
    // Predefined configurations( bandwidth, coding rate, spread factor ):
    // Bw125Cr45Sf128     Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
    // Bw500Cr45Sf128     Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
    // Bw31_25Cr48Sf512   Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
    // Bw125Cr48Sf4096    Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
    // Bw125Cr45Sf2048    Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
    if (!_rf95.setModemConfig(RH_RF95::Bw125Cr45Sf2048)) {
        Logger::log("Config Failed.");
        while (1)
            delay(10);
    }

    Logger::log("RF95 Ready.");
}

// **** Application Functions ****
void RadioSensor::ReplyToRecievedMessageExample(String data) {
    uint8_t len = sizeof(_incoming_buffer);
    uint8_t from;
    if(_mesh.recvfromAck(_incoming_buffer, &len, &from)) {
        // print recieved message
        Logger::logPkt(from, _rf95.lastRssi(), _rf95.lastSNR(), _incoming_buffer);

        // reply
        uint8_t res;
        uint8_t databuf[data.length()+1];
        res = _mesh.sendtoWait(databuf, data.length()+1, from);
        if ( res != RH_ROUTER_ERROR_NONE) {
            Logger::logPkt(from, String("Sending packet failed."));
        } else {
            Logger::logPkt(from, databuf);
        }
        delete[] databuf;
    }
}
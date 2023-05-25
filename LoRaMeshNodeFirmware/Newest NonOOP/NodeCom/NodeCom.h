#ifndef NODECOM_H
#define NODECOM_H

#include <Arduino.h>

//Lora module controle libraries
#include <RHMesh.h>
#include <RH_RF95.h>
#include <SPI.h>


//usefull c++ libraries
#include <string>
#include <cstdint>
#include <cstring>

//Sensors
#include "Sensors.h"





struct ModemConfig
{
    uint8_t 	reg_1d;      //Value for register RH_RF95_REG_1D_MODEM_CONFIG1.
    uint8_t 	reg_1e;      //Value for register RH_RF95_REG_1E_MODEM_CONFIG2.
    uint8_t 	reg_26;  	//Value for register RH_RF95_REG_26_MODEM_CONFIG3. 
};


class NodeCom
{
  
  public:
  enum ModemConfigChoice {  Bw125Cr45Sf128 = 0, 
                            Bw500Cr45Sf128, 
                            Bw31_25Cr48Sf512, 
                            Bw125Cr48Sf4096, 
                            Bw125Cr45Sf2048 };
  enum RequestMessages {  ERROR = 0,
                          IP_REQUEST,
                          GET_STATUS,
                          GET_MASTER_IP, 
                          GET_ALL_SENSORS_DATA, 
                          GET_SENSORS_DATA };
  NodeCom(bool is_sensor_node = false);
  NodeCom(int ssn, int dio0, bool is_sensor_node = false, uint8_t ip=254);
  bool init(bool longRange = false);
  bool setFrequency(float centre);
  void setTxPower(int8_t power, bool useRFO=false);
  void setCADTimeout (unsigned long cad_timeout);
  void setModemRegisters (const ModemConfig *config);
  bool setModemConfig (ModemConfigChoice index);
  void setIsaRouter (bool isa_router);

  uint8_t sendtoWait (uint8_t *buf, uint8_t len, uint8_t dest, uint8_t flags=0);
  uint8_t sendtoWait (std::string *buf, uint8_t dest, uint8_t flags=0);
  bool recvfromAck (uint8_t *buf, uint8_t *len, uint8_t *source=NULL, uint8_t *dest=NULL, uint8_t *id=NULL, uint8_t *flags=NULL, uint8_t *hops=NULL);
  bool recvfromAck (std::string *buf, uint8_t *len, uint8_t *source=NULL, uint8_t *dest=NULL, uint8_t *id=NULL, uint8_t *flags=NULL, uint8_t *hops=NULL);
  bool recvfromAckTimeout (uint8_t *buf, uint8_t *len, uint16_t timeout, uint8_t *source=NULL, uint8_t *dest=NULL, uint8_t *id=NULL, uint8_t *flags=NULL, uint8_t *hops=NULL);
  bool recvfromAckTimeout (std::string *buf, uint8_t *len, uint16_t timeout, uint8_t *source=NULL, uint8_t *dest=NULL, uint8_t *id=NULL, uint8_t *flags=NULL, uint8_t *hops=NULL);
  int16_t lastRssi();
  bool sendRequest(uint8_t dest, RequestMessages message, uint8_t *buf, uint8_t *len, uint16_t timeout);
  bool waitForResquest();
  bool setAsMasterIp();
  bool findMasterIp();
  bool getIp();



  private:
  enum RequestMessages;
  RH_RF95 rf95;
  RHMesh manager;
  bool isSensorNode;
  Sensors sensor;
  uint8_t ip;
  uint8_t master_ip;
  bool respondRequest(uint8_t* dest, RequestMessages message);
  bool giveIp();

};


#endif
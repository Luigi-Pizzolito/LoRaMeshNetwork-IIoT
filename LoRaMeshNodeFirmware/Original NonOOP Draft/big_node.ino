//node

#include <Wire.h>
#include <SPI.h>
//for sensor AHT20+BMP280
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
//for lora module
#include <RHMesh.h>
#include <RH_RF95.h>

//for sensor AHT20+BMP280

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; // I2C

void AHT20_setup(){
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
}

void BMP280_setup(){
  unsigned status;
  status = bmp.begin();
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

//Return sensor data in the following format: Temperature + ";" +
//                                            Humidity + ";" +
//                                            Pressure + ";" + 
//                                            Altitude + ";"
String getSensorData(){
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  String data = String( temp.temperature ) + ";" + 
                String( humidity.relative_humidity ) + ";" +
                String( bmp.readPressure() ) + ";" + 
                String( bmp.readAltitude(1013.25) ) + ";";
  return data;
}


//Communication part
const String ip = "M1";
#define NODE_ADDRESS 3    // address of this node
#define BRIDGE_ADDRESS 1  // address of the bridge ( we send our data to, hopefully the bridge knows what to do with our data )
#define RH_MESH_MAX_MESSAGE_LEN 50

RH_RF95 rf95;
RHMesh manager(rf95, NODE_ADDRESS);

#define TXINTERVAL 3000  // delay between successive transmissions
unsigned long nextTxTime;

void sx1278_setup(){
  Serial.print(F("initializing node "));
  Serial.print(NODE_ADDRESS);
  if (!manager.init())
    {Serial.println(" init failed");} 
  else
    {Serial.println(" done");}  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36 
  rf95.setTxPower(18, false); // with false output is on PA_BOOST, power from 2 to 20 dBm, use this setting for high power demos/real usage
  rf95.setFrequency(433.0);
  rf95.setCADTimeout(500);
  // long range configuration requires for on-air time
  boolean longRange = false;
  if (longRange) 
    {
    // custom configuration
    RH_RF95::ModemConfig modem_config = {
      0x78, // Reg 0x1D: BW=125kHz, Coding=4/8, Header=explicit
      0xC4, // Reg 0x1E: Spread=4096chips/symbol, CRC=enable
      0x08  // Reg 0x26: LowDataRate=On, Agc=Off.  0x0C is LowDataRate=ON, ACG=ON
      };
    rf95.setModemRegisters(&modem_config);
    }
  else
    {
    // Predefined configurations( bandwidth, coding rate, spread factor ):
    // Bw125Cr45Sf128     Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range
    // Bw500Cr45Sf128     Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range
    // Bw31_25Cr48Sf512   Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range
    // Bw125Cr48Sf4096    Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, low data rate, CRC on. Slow+long range
    // Bw125Cr45Sf2048    Bw = 125 kHz, Cr = 4/5, Sf = 2048chips/symbol, CRC on. Slow+long range
    if (!rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128))
      {Serial.println(F("set config failed"));}
    }
  Serial.println("RF95 ready");
  nextTxTime = millis();
}

uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t res;

//IMPORTANT!!! Don't forget to delete the generated table at the end.
//How to use: uint8_t* charArray = generateByteArray(inputString);
//How to delete: delete[] charArray;
uint8_t* generateByteArray(String inputString) {
  int stringLength = inputString.length();
  uint8_t* byteArray = new uint8_t[stringLength];
  for (int i = 0; i < stringLength; i++) {
    byteArray[i] = (uint8_t)inputString.charAt(i);
  }
  return byteArray;
}


void sendMessageExemple(){
  // send message every TXINTERVAL millisecs
  if (millis() > nextTxTime)
    {
    nextTxTime += TXINTERVAL;
    Serial.print("Sending to bridge n.");
    Serial.print(BRIDGE_ADDRESS);
    Serial.print(" res=");
    
    // Send a message to a rf95_mesh_server
    // A route to the destination will be automatically discovered.
    res = manager.sendtoWait(data, sizeof(data), BRIDGE_ADDRESS);
    Serial.println(res);
    if (res == RH_ROUTER_ERROR_NONE)
      {
      // Data has been reliably delivered to the next node.
      // now we do... 
      }
    else
      {
      // Data not delivered to the next node.
      Serial.println("sendtoWait failed. Are the bridge/intermediate mesh nodes running?");
      }
    }
  
  // radio needs to stay always in receive mode ( to process/forward messages )
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager.recvfromAck(buf, &len, &from))
    {
    Serial.print("message from node n.");
    Serial.print(from);
    Serial.print(": ");
    Serial.print((char*)buf);
    Serial.print(" rssi: ");
    Serial.println(rf95.lastRssi()); 
    }
}






void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AHT20_setup();
  BMP280_setup();
  sx1278_setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  sendMessageExemple();
}

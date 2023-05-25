/*
LoRa Mesh Node Firmware
Runs both large radio forward nodes and smaller sensor nodes.
Sensors have automatic UUID and reply only by direct request from radio node.
Sensor data is encoded using MessagePack JSON representation.

Circuit:
LoRa Module on SPI () and DIO1 to pin XX.
For sensor node, see connections in Sensors.h
For radio node, a 9800 baud GPS modules is connected to pins 17(RX) and 16(TX).
---

created 27 Mar 2023
by Luigi Pizzolito
*/

// -------- Settings --------
// Which type of node, only one at a tiem.
#define RADIO_NODE
// #define SENSOR_NODE
// --------------------------
#ifdef RADIO_NODE
  #include "RadioNode.h"
#endif
#ifdef SENSOR_NODE
  #include "SensorNode.h"
#endif

#include "UUIDManager.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\r\nUUID TEST");
  UUIDMan uuid;
  Serial.println(uuid.uuid);
  Serial.println(uuid.intID());
  Serial.println("BD TEST");
  SensorDataDB db;
  delay(1000);
  Serial.print("Final DB: ");
  // Serial.println(db);
  Serial.println(db.exportJSON());
  delay(1000);
  RadioNode rn;
  rn.connectWiFi();
  delay(500);
  String jsonString = "{\"id\":0,\"ts\":1678841238,\"data\":{\"temperature\":27.66,\"humidity\":62.98,\"soil_moisture\":20.61}}";
  rn.sendPOSTForm(jsonString);
  
  // // Connect to Wi-Fi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");
  // String jsonString = "{\"id\":0,\"ts\":1678841238,\"data\":{\"temperature\":27.66,\"humidity\":62.98,\"soil_moisture\":20.61}}";
  // HTTPClient http;
  // http.begin(serverUrl);
  // http.addHeader("Content-Type", "multipart/form-data;");
  // String requestBody = "Content-Disposition: form-data; name=\"json\"\r\n\r\n";
  // requestBody+=jsonString;
  // http.POST(requestBody);
  // Serial.println(http.getString());
  // http.end();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

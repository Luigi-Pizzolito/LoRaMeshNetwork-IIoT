#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  //LoRa.setPins(8, 3, 4);
  Serial.println("LoRa Client");
  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1);
  }
}

void loop() {
  String payload = "Request data";
  Serial.print("Sending message: ");
  Serial.print(payload);
  Serial.print("\n**************\n");
  LoRa.beginPacket();
  LoRa.print("Request data");
  LoRa.endPacket();
  
  
  unsigned int timer = 0;
  int packetSize = LoRa.parsePacket();
  while(packetSize==0 && timer < 15000){packetSize = LoRa.parsePacket();timer++;}
  if (packetSize) {
    Serial.print("Received data: ");
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.println();
  }

  delay(5000);
}

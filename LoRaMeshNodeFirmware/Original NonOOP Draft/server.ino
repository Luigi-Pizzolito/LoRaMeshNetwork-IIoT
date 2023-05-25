#include <SPI.h>
#include <LoRa.h>


/* For the DHT11  */
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 1
//DHT dht(DHTPIN, DHTTYPE);

/* For the display */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define OLED_ADDR   0x3C

void show(String txt)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(txt);
  display.display();
}

void setup_screen()
{
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome");
  display.display();
  delay(3000);
}




void setup() {
  setup_screen();  //screen setup
  
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("LoRa Server");
  show("LoRa Server");
  
  //LoRa.setPins(8, 3, 4);

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed.");
    show("LoRa initialization failed.");
    while (1);
  }

}

//the request message code
String request_message = "Request data";

void loop() {
  
  //Serial.println();
  Serial.println("Waiting for the request");
  show("Waiting for the \nrequest");
  int packetSize = LoRa.parsePacket();
  while(!packetSize)packetSize = LoRa.parsePacket();
  if (packetSize) {
    String request;
    while (LoRa.available()) {
      request += (char)LoRa.read();
    }
    int RSSI_received = LoRa.packetRssi();
  
    if (request == request_message) {
      DHT.read(DHT11_PIN);
      float humidity = DHT.temperature;
      float temperature = DHT.humidity;

      Serial.println("\nSending back requested data ");
      
      String response = "\nHumidity: " + String(humidity) + " % \nTemp: " + String(temperature) + " °C\n";
      LoRa.beginPacket();
      LoRa.print(response);
      LoRa.endPacket();
      Serial.print(response);
      show("Response: " + response + "RSSI: " + String(RSSI_received));
      // print RSSI of packet
      Serial.print("With RSSI ");
      Serial.println(RSSI_received);
      delay(2000);
    }
  }
}

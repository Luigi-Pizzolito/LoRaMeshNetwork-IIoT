#include <Arduino.h>

#include "pins_ws_rp2040_zero.h"
#include <utils.h>
#include <NodeSensors.h>
#include <RadioSensor.h>

NodeSensors sensors(NODE_ID);
// RadioSensor radio;
unsigned long time_now = 0;

String data;

void setup()
{
	Serial.begin(115200);
	delay(5000);
	Serial.println("HELLO WORLD");
}

void loop()
{
	if (millis() >= time_now + 1000) {
		// every 1000ms
		time_now += 1000;
		Serial.println(sensors.printData());
		data = sensors.data();
		printHEX(data);
	}
	
	// radio.ReplyToRecievedMessageExample(String("TEST STRING!"));
}
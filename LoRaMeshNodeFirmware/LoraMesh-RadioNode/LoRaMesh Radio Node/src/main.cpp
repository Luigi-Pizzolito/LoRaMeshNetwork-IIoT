#include <Arduino.h>

// #include <SensorDataDB.h>
// #include <GatewayOps.h>
#include <LowPowerGPS.h>


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // SensorDataDB db;
  LowPowerGPS gps;
  Serial.println("Setup finished.");
}

void loop()
{
  // put your main codke here, to run repeatedly:
}
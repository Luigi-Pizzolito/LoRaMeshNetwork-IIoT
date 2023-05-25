#pragma once

/*
GatewayOps Module

created 27 Mar 2023
by Luigi Pizzolito
*/

// #include "SensorDataDB.h"
// #include "UUIDManager.h"
// #include <TinyGPSPlus.h>

#include "WiFiCredentials.h"
#include <WiFi.h>
#include <HTTPClient.h>

class GatewayOps
{
public:
    GatewayOps();
    static void connectWiFi();
    void sendPOSTForm(String jsonString);
};
#include "GatewayOps.h"

GatewayOps::GatewayOps()
{
}

void GatewayOps::connectWiFi()
{
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void GatewayOps::sendPOSTForm(String jsonString)
{
    // Make the POST request
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "multipart/form-data;");
    String requestBody = "Content-Disposition: form-data; name=\"json\"\r\n\r\n";
    requestBody += jsonString;
    http.POST(requestBody);
    Serial.println(http.getString());
    http.end();
    // TODO: change the server code to accept an array.
    // TODO: clear the DB buffer after successful send + ACK.
}

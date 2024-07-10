#include "LCD.h"

void wifiAP(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_OFF);
    delay(1000);
    Serial.print("Setting AP (Access Point)…");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    delay(1000);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void wifiSTA(const char *netssid, const char *netpassword, const char *ssid, const char *password)
{
    WiFi.mode(WIFI_OFF); // Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(netssid);
    WiFi.begin(netssid, netpassword);

    displayWifiConnecting(netssid);
    delay(1000);

    uint32_t periodToConnect = 30000L;
    for (uint32_t StartToConnect = millis(); (millis() - StartToConnect) < periodToConnect;)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        else
        {
            break;
        }
    }
    if (WiFi.isConnected())
    {
        displayWifiConnected(WiFi.localIP());
    }
    else
    {
        wifiAP(ssid, password);
    }
    delay(1000);
}

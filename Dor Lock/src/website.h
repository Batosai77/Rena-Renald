#include <HTTPClient.h>
#include <WiFi.h>
#include "relay.h"

extern WiFiClient client;

int confirmEnrollment(String Link, String URL, const char *finger_uuid)
{
    int isTrue = 0;
    if (WiFi.isConnected())
    {
        HTTPClient http;

        Link = URL + "attendance" + "/" + finger_uuid;
        http.begin(client, Link);

        JsonDocument doc;
        doc["type"] = "attendance";
        doc["_method"] = "PUT";

        String body;
        serializeJson(doc, body);

        http.addHeader("Content-Type", "application/json");

        int httpCode = http.POST(body);
        Serial.println(httpCode);
        Serial.println(Link);
        delay(1000);
        if (httpCode == 200)
        {
            display.clearDisplay();
            display.setTextSize(1.5);    // Normal 1:1 pixel scale
            display.setTextColor(WHITE); // Draw white text
            display.setCursor(0, 0);     // Start at top-left corner
            display.display();
            isTrue = 1;
            delay(2000);
        }
        else
        {
            isTrue = 0;
            Serial.println("Error Confirm Adding!");
        }
        http.end();
    }
    return isTrue;
}

int sendDelete(String Link, String URL, const char *finger_uuid)
{
    String finger_uuid_value = String(finger_uuid);
    if (WiFi.isConnected())
    {
        int isTrue = 0;
        HTTPClient http;
        Link = URL + "attendance" + "/" + finger_uuid_value;

        http.begin(client, Link);

        JsonDocument doc;
        doc["_method"] = "DELETE";

        String body;
        serializeJson(doc, body);

        http.addHeader("Content-Type", "application/json");

        int httpCode = http.POST(body);
        Serial.println(Link);
        Serial.println(body);
        Serial.println(httpCode);
        delay(1000);
        if (httpCode == 200)
        {
            isTrue = 1;
            // timer.enable(timerCheckMode);
            Serial.println("Deleted Success!");
            delay(1000);
        }
        return isTrue;
        http.end();
    }
}

int sendAttendance(bool useKeypad, String Link, String URL, const char *finger_uuid, String device_token, int finger_id, String password_1)
{
    Serial.println("Sending the Fingerprint ID");
    if (WiFi.isConnected())
    {
        int isTrue = 0;
        HTTPClient http;

        Link = URL + "add-record" + "/" + "fingerprint";
        http.begin(client, Link);

        JsonDocument doc;
        doc["finger_id"] = finger_id;

        String body;
        serializeJson(doc, body);

        http.addHeader("Content-Type", "application/json");
        http.addHeader("token", device_token);

        int httpCode = http.POST(body);
        Serial.println(httpCode);
        Serial.println(body);
        Serial.println(Link);
        Serial.println(device_token);

        if (httpCode == 200)
        {
            Welcome();
            ActiveRelay();
            isTrue = 1;
            // display.print(user_name);
        }
        else if (httpCode == 403)
        {
            String payload = http.getString();
            JsonDocument responseDoc;
            deserializeJson(responseDoc, payload);

            int code = responseDoc["code"];
            const char *expiredAt = responseDoc["expired_at"];
            password_1 = String(code);
            // timer.disable(timerCheckMode);
            // timer.disable(t2);

            useKeypad = true;
            Serial.println(code);
            Serial.println(expiredAt);
            display.clearDisplay();
            display.setTextSize(2);      // Smaller text size for more info
            display.setTextColor(WHITE); // Draw white text
            display.setCursor(30, 0);    // Start at top-left corner
            display.print(F("Code"));
            display.setCursor(0, 20);
            display.print(code);
            display.setCursor(0, 40);
            display.print(F("Exp: "));
            display.print(expiredAt);
            display.display();
        }
        else
        {
            display.clearDisplay();
            display.setTextSize(2);      // Normal 2:2 pixel scale
            display.setTextColor(WHITE); // Draw white text
            display.setCursor(15, 0);    // Start at top-left corner
            display.print(F("Gak Boleh"));
            display.setCursor(0, 20);
            display.display();
        }
        return isTrue;
        http.end();
    }
}
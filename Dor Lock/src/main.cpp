// #include <Arduino.h>
#include <SimpleTimer.h> //https://github.com/jfturcot/SimpleTimer
#include <HardwareSerial.h>
#include <WebServer.h>
#include <WiFi.h>

// OLED-----------------------------
#include <Adafruit_Fingerprint.h> //https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
#include <ArduinoJson.h>          //json
#include <string.h>
#include <Keypad.h> //keypad

#include "wifiFunction.h"
#include "WebPage.h"
#include "website.h"
#include "Fingerprint.h"

// #include "LCD.h"
//*******************************Keypad********************************
#define ROW_NUM 4    // keypad four rows
#define COLUMN_NUM 4 // keypad three columns
#define USE_KEYPAD false

char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte pin_rows[ROW_NUM] = {27, 14, 12, 13};      // ESP32 pin: GPIO12, GPIO14, GPIO27, GPIO26 connected to the row pins
byte pin_column[COLUMN_NUM] = {32, 33, 25, 26}; // ESP32 pin: GPIO25, GPIO33, GPIO32 connected to the column pins

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String password_1 = "1234";         // change your password here
const String password_2 = "56789";  // change your password here
const String password_3 = "901234"; // change your password here
String input_password;

//***********************Fingerprint*****************************
// Fingerprint scanner Pins
#define Finger_Rx 16 // Rx2
#define Finger_Tx 17 // Tx2

//***********************RELAY*****************************
int currentState; // the current reading from the input pin

//************************************************************************
WiFiClient client;
SimpleTimer timer;
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
WebServer server(80);

//************************************************************************
String getData, Link, URL;
const char *finger_uuid;
String device_token;
//************************************************************************
int FingerID, t2, timerCheckMode, finger_id, isTrue; // The Fingerprint ID from the scanner
bool device_Mode = false;                            // Default Mode Enrollment
bool useKeypad = false, emergency = false;
unsigned long previousMillis = 0;
//************************************************************************

//***********************WIFI*****************************
/* Set these to your desired credentials. */
const char *ssid = "ESP32-WiFi";
const char *password = "password";

const char *netSsid = "";
const char *netPassword = "";
bool firstConnect = true;
//***********************Wrapping*****************************

void EnrollModeTimer()
{
  int fingerEnroll = Enrollment(finger, finger_id);
  if (fingerEnroll > 0)
  {
    isTrue = confirmEnrollment(Link, URL, finger_uuid);
  }
}

void DeleteModeTimer()
{
  int deleteId = deleteFingerId(finger, finger_id);
  Serial.println(deleteId);
  if (deleteId > 0)
  {
    isTrue = sendDelete(Link, URL, finger_uuid);
  }
}

void AttendanceMode()
{
  Serial.println("attendande");
  Serial.println(FingerID);
  if (FingerID > 0)
  {
    isTrue = sendAttendance(useKeypad, Link, URL, finger_uuid, device_token, FingerID, password_1);
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    display.display();
    delay(2000);
  }
  else if (FingerID == 0)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    display.display();
  }
  //---------------------------------------------
  // Didn't find a match
  else if (FingerID == -1)
  {
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
    display.display();
  }
  //---------------------------------------------
  // Didn't find the scanner or there an error
  else if (FingerID == -2)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE);
    display.display();
  }
}

void emergencyEvent()
{
  FingerID = getFingerId(finger);
  // Serial.println(FingerID);
  if (FingerID > 0)
  {
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    display.display();
    delay(2000);
  }
  //---------------------------------------------
  // No finger detected
  else if (FingerID == 0)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    display.display();
  }
  //---------------------------------------------
  // Didn't find a match
  else if (FingerID == -1)
  {
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
    display.display();
  }
  //---------------------------------------------
  // Didn't find the scanner or there an error
  else if (FingerID == -2)
  {
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE);
    display.display();
  }
}

void CheckMode()
{
  Serial.println("Checking mode");
  if (WiFi.isConnected())
  {
    HTTPClient http;

    Link = URL + "get-mode" + "/attendance";
    http.begin(client, Link);  // initiate HTTP request,
    int httpCode = http.GET(); // Send the request

    Serial.println(Link);
    Serial.println(httpCode);
    delay(1000);
    if (httpCode == 200)
    {
      String payload = http.getString(); // Get the response payload
      Serial.println(payload);
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);

      const char *mode = doc["mode"];
      finger_uuid = doc["finger_uuid"]; // Ensure correct assignment
      finger_id = doc["finger_id"];     // Ensure correct assignment
      if (strcmp(mode, "register") == 0)
      {
        Serial.println(finger_uuid);
        Serial.println("Deivce Mode: Enrollment");
        EnrollModeTimer();
        timer.disable(timerCheckMode);
      }
      else if (strcmp(mode, "delete") == 0)
      {
        DeleteModeTimer();
        timer.disable(timerCheckMode);
        Serial.println("Deivce Mode: Delete Finger ID");
      }
      else
      {
        // AttendanceMode();
        Serial.println("Deivce Mode: Attendance");
      }
      http.end();
    }
    else
    {
      emergency = true;
      timer.disable(timerCheckMode);
      Serial.println("Error Http Request");
    }
    http.end(); // Close connection
  }
  else
  {
    emergency = true;
    timer.disable(timerCheckMode);
    Serial.println("Wifi Not Connected");
  }
}

void CheckModeTimer()
{
  CheckMode();
}
//***********************Handle POST*****************************

void handlePost()
{
  String newSsid = server.arg("ssid");
  String newPassword = server.arg("password");
  String newToken = server.arg("token");
  String newUrl = server.arg("url");
  // Validasi input
  if (newSsid.isEmpty() || newPassword.isEmpty() || newToken.isEmpty() || newUrl.isEmpty())
  {
    server.send(400, "text/plain", "Invalid input. All fields are required.");
    return;
  }
  netSsid = newSsid.c_str();
  netPassword = newPassword.c_str();
  device_token = newToken.c_str();
  URL = newUrl;

  server.send(200, "text/plain", "success!");
  delay(500);
  firstConnect = false;
  wifiSTA(netSsid, netPassword, ssid, password);
  Serial.println("Setup Wifi Mode");
  CheckModeTimer();
}

//***********************KEYPAD*****************************

void UseKeypad()
{
  char key = keypad.getKey();

  if (key)
  {
    Serial.println(key);

    if (key == '*')
    {
      input_password = ""; // reset the input password
    }
    else if (key == '#')
    {
      if (input_password == password_1 || input_password == password_2 || input_password == password_3)
      {
        useKeypad = false;
        Serial.println("The password is correct => unlock");
        display.clearDisplay();
        display.setTextSize(2);      // Normal 2:2 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(15, 0);    // Start at top-left corner
        display.print(F("Welcome"));
        display.setCursor(0, 20);
        display.display();
        delay(2000);
        display.clearDisplay();
        display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
        display.display();

        digitalWrite(RELAY_PIN, HIGH);
        delay(5000);
        digitalWrite(RELAY_PIN, LOW);
      }
      else
      {
        Serial.println("The password is incorrect, try again");
      }

      input_password = ""; // reset the input password
    }
    else
    {
      input_password += key; // append new character to input password string
    }
  }
}

//***********************SETUP*****************************

void setup()
{
  Serial.begin(9600);
  // pinMode(BUTTON_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT); // initialize pin as an output.
  //----------------------Relay-----------------------
  digitalWrite(RELAY_PIN, LOW);
  delay(500);

  setupOLED();

  logoLion();
  delay(5000);
  // put your setup code here, to run once:

  // mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);
  Serial.println("\n\nAdafruit finger detect test");

  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    display.display();
    delay(1000);
    noFinger();
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, WHITE);
    display.display();
    while (1)
    {
      delay(1);
    }
  }
  //----------------------Keypad-----------------------
  input_password.reserve(32); // maximum password size is 32, change if needed
  //---------------------------------------------
  finger.getTemplateCount();
  Serial.print("Sensor contains ");
  Serial.print(finger.templateCount);
  Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

  wifiAP(ssid, password);
  server.on("/", []()
            { server.send_P(200, "text/html", WebPage); });
  server.on("/save", HTTP_POST, handlePost);
  server.begin();
  timerCheckMode = timer.setInterval(25000L, CheckModeTimer);
}

void loop()
{
  timer.run();
  currentState = digitalRead(BUTTON_PIN);

  if (emergency)
  {
    wifiAP(ssid, password);
    emergency = false;
  }
  if (WiFi.getMode() == WIFI_MODE_AP)
  {
    server.handleClient();
    emergencyEvent();
    UseKeypad();
    timer.disable(timerCheckMode);
  }
  else
  {
    FingerID = getFingerId(finger);
    AttendanceMode();
    if (isTrue == 1)
    {
      timer.enable(timerCheckMode);
    }
    delay(10);
  }

  if (currentState == LOW)
  {
    Serial.println("Button pressed!");
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
  }
}

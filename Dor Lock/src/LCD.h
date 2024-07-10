#ifndef LCD_H
#define LCD_H

#include <Wire.h>                //https://github.com/adafruit/Adafruit_SSD1306
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>    //https://github.com/adafruit/Adafruit-GFX-Library
#include <SPI.h>
#include "icons.h"

//***********************OLED*****************************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    0 // Reset pin # (or -1 if sharing Arduino reset pin)

extern Adafruit_SSD1306 display;

extern void setupOLED();
extern void displayWifiConnected(IPAddress IPAddr);
extern void displayWifiConnecting(const char *ssid);
extern void logoLion();
extern void fingerScanning();
extern void imageTaken();
extern void noFinger();
extern void invalidImage();
extern void Welcome();

#endif

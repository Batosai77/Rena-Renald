#include "LCD.h"
#include "icons.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupOLED()
{
    //-----------initiate OLED display-------------
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
}

void displayWifiConnected(IPAddress IPAddr)
{
    Serial.println("");
    Serial.println("Connected");
    Serial.print("IP address: ");
    Serial.println(IPAddr);
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(8, 0);     // Start at top-left corner
    display.print(F("Connected \n"));
    display.drawBitmap(33, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, WHITE);
    display.display();
}

void displayWifiConnecting(const char *ssid)
{
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print(F("Connecting to \n"));
    display.setCursor(0, 50);
    display.setTextSize(2);
    display.print(ssid);
    display.drawBitmap(73, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, WHITE);
    display.display();
}

void logoLion()
{
    display.clearDisplay();
    display.drawBitmap(0, 0, lion_of_informatics, 128, 64, WHITE);
    display.display();
}

void fingerScanning()
{
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_scan_bits, FinPr_scan_width, FinPr_scan_height, WHITE);
    display.display();
}

void imageTaken()
{
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
    display.display();
}
void noFinger()
{
    display.clearDisplay();
    display.drawBitmap(32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
    display.display();
}

void invalidImage()
{
    display.clearDisplay();
    display.drawBitmap(34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
    display.display();
}

void Welcome()
{
    display.clearDisplay();
    display.setTextSize(2);      // Normal 2:2 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(15, 0);    // Start at top-left corner
    display.print(F("Welcome"));
    display.setCursor(0, 20);
    display.display();
}

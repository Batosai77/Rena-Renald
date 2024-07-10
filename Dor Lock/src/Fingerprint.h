#include "LCD.h"

int Enrollment(Adafruit_Fingerprint finger, int finger_id)
{
    Serial.println("enroll");
    int p = -1;

    while (p != FINGERPRINT_OK)
    {
        fingerScanning();
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            Serial.println("Image taken");
            imageTaken();
        case FINGERPRINT_NOFINGER:
            // Serial.println(".");
            // noFinger();
            Serial.println("No Finger");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            invalidImage();
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            break;
        default:
            Serial.println("Unknown error");
            break;
        }
    }

    // OK success!
    p = finger.image2Tz(1);
    switch (p)
    {
    case FINGERPRINT_OK:
        imageTaken();
        break;
    case FINGERPRINT_IMAGEMESS:
        invalidImage();
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }
    display.clearDisplay();
    display.setTextSize(2);      // Normal 2:2 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print(F("Remove"));
    display.setCursor(0, 20);
    display.print(F("finger"));
    display.display();
    // Serial.println("Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger.getImage();
    }
    Serial.print("ID ");
    Serial.println(finger_id);
    p = -1;
    fingerScanning();
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            // Serial.println("Image taken");
            imageTaken();
            break;
        case FINGERPRINT_NOFINGER:
            // Serial.println(".");
            noFinger();
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            Serial.println("Communication error");
            break;
        case FINGERPRINT_IMAGEFAIL:
            Serial.println("Imaging error");
            break;
        default:
            Serial.println("Unknown error");
            break;
        }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p)
    {
    case FINGERPRINT_OK:
        // Serial.println("Image converted");
        imageTaken();
        break;
    case FINGERPRINT_IMAGEMESS:
        // Serial.println("Image too messy");
        invalidImage();
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
    default:
        Serial.println("Unknown error");
        return p;
    }

    // OK converted!
    Serial.print("Creating model for #");
    Serial.println(finger_id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Prints matched!");
        imageTaken();
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication error");
        return p;
    }
    else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
        Serial.println("Fingerprints did not match");
        invalidImage();
        return p;
    }
    else
    {
        Serial.println("Unknown error");
        return p;
    }

    Serial.print("ID ");
    Serial.println(finger_id);
    p = finger.storeModel(finger_id);
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Stored!");
        imageTaken();
        return finger_id;
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        Serial.println("Communication error");
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        Serial.println("Could not store in that location");
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        Serial.println("Error writing to flash");
        return p;
    }
    else
    {
        Serial.println("Unknown error");
        return p;
    }
}

int getFingerId(Adafruit_Fingerprint finger)

{
    uint8_t p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
    case FINGERPRINT_NOFINGER:
        // Serial.println("No finger detected");
        return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
        // Serial.println("Communication error");
        return -2;
    case FINGERPRINT_IMAGEFAIL:
        // Serial.println("Imaging error");
        return -2;
    default:
        // Serial.println("Unknown error");
        return -2;
    }
    // OK success!
    p = finger.image2Tz();
    switch (p)
    {
    case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
    case FINGERPRINT_IMAGEMESS:
        // Serial.println("Image too messy");
        return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
        // Serial.println("Communication error");
        return -2;
    case FINGERPRINT_FEATUREFAIL:
        // Serial.println("Could not find fingerprint features");
        return -2;
    case FINGERPRINT_INVALIDIMAGE:
        // Serial.println("Could not find fingerprint features");
        return -2;
    default:
        // Serial.println("Unknown error");
        return -2;
    }
    // OK converted!
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK)
    {
        Serial.println("Found a print match!");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        // Serial.println("Communication error");
        return -2;
    }
    else if (p == FINGERPRINT_NOTFOUND)
    {
        // Serial.println("Did not find a match");
        return -1;
    }
    else
    {
        // Serial.println("Unknown error");
        return -2;
    }

    // found a match!
    display.display();
    Serial.print("Found ID #");
    Serial.print(finger.fingerID);
    Serial.print(" with confidence of ");
    Serial.println(finger.confidence);

    return finger.fingerID;
}

int deleteFingerId(Adafruit_Fingerprint finger, int finger_id)
{
    uint8_t p = -1;
    int ID = 0;
    p = finger.deleteModel(finger_id);

    if (p == FINGERPRINT_OK)
    {
        // Serial.println("Deleted!");
        display.clearDisplay();
        display.setTextSize(2);      // Normal 2:2 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(0, 0);     // Start at top-left corner
        display.print(F("Deleted!\n"));
        display.display();
        // SendDeleteFingerID();
        ID = 1;
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        // Serial.println("Communication error");
        display.clearDisplay();
        display.setTextSize(1);      // Normal 1:1 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(0, 0);     // Start at top-left corner
        display.print(F("Communication error!\n"));
        display.display();
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        // Serial.println("Could not delete in that location");
        display.clearDisplay();
        display.setTextSize(1);      // Normal 1:1 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(0, 0);     // Start at top-left corner
        display.print(F("Could not delete in that location!\n"));
        display.display();
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        // Serial.println("Error writing to flash");
        display.clearDisplay();
        display.setTextSize(1);      // Normal 1:1 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(0, 0);     // Start at top-left corner
        display.print(F("Error writing to flash!\n"));
        display.display();
        return p;
    }
    else
    {
        // Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
        display.clearDisplay();
        display.setTextSize(2);      // Normal 2:2 pixel scale
        display.setTextColor(WHITE); // Draw white text
        display.setCursor(0, 0);     // Start at top-left corner
        display.print(F("Unknown error:\n"));
        display.display();
        return p;
    }
    return ID;
}
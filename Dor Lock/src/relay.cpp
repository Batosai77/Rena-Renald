#include <Arduino.h>
#include "relay.h"

void ActiveRelay()
{
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
}
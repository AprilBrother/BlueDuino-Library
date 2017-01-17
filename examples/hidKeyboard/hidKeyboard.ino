#include "AB_BLE.h"

#define BAUD_RATE 57600

AB_BLE ble(&Serial1);

void setup() {
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE);

  Serial.println(F("ZeroKeyboard Example"));
  Serial.println(F("---------------------------------------"));
}

void loop() {
  
}


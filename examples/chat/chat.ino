#include <AB_BLE.h>

#define BAUD_RATE 9600

AB_BLE ble(&Serial1);
String tmp; 

void setup() {
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Hello BlueDuino!");
}

void loop() {

  while (ble.available() > 0)  {
    tmp += char(ble.read());
    delay(2);
  }

  if(tmp.length() > 0) {
    Serial.println(tmp);
    tmp = "";
  }

  if (Serial.available()) {
    ble.write(Serial.read());
  }
}

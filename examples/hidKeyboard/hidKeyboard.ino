#include "AB_BLE.h"

#define BAUD_RATE 57600

AB_BLE ble(&Serial1);

void setup() {
  Serial.begin(BAUD_RATE);
  Serial1.begin(BAUD_RATE);

  while(!Serial) {
    //
  }

  Serial.println(F("ZeroKeyboard Example"));
  Serial.println(F("---------------------------------------"));
  Serial.println();
  Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
  Serial.println(F("then open an application that accepts keyboard input"));

  Serial.println();
  Serial.println(F("Enter the character(s) to send:"));

  ble.verbose(true);
}

void loop(void)
{
  // Display prompt
  Serial.print(F("keyboard > "));

  // Check for user input and echo it back if anything was found
  char keys[AB_BUF_SIZE];
  getUserInput(keys, AB_BUF_SIZE - 1);

  Serial.print("\nSending ");
  Serial.println(keys);

  ble.print("AT+KEY=");
  ble.println(keys);

  if( ble.waitOk() )
  {
    Serial.println( F("OK!") );
  }else
  {
    Serial.println( F("FAILED!") );
  }
}

void getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);
  while( Serial.available() == 0 ) {
    delay(1);
  }

  uint8_t count=0;

  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && !(Serial.available() == 0) );
}

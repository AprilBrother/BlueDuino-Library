#define BAUD_RATE 9600

void setup() {
    Serial.begin(BAUD_RATE);

    // Note: The serial function will not work if the device go to sleep.
    // You need give high level to pin P0.1 to wakeup the device.
    // https://wiki.aprbrother.com/en/ZeroBeacon.html
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    // BLE module CC2540 Pins - RXI/TXO - Serial1
    // https://wiki.aprbrother.com/en/BlueDuino_Rev2_Hookup_Guide.html
    Serial1.begin(BAUD_RATE); 
    while (!Serial) {
        ; // wait for serial port to connect. Needed for Leonardo only
    }

    Serial.println("Hello BlueDuino!");
}

void loop() {
    if(Serial1.available() > 0)
        Serial.write(Serial1.read());

    if (Serial.available()) 
        Serial1.write(Serial.read());
}

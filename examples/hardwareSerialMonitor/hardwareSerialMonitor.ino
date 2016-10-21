#define BAUD_RATE 9600

void setup() {
    Serial.begin(BAUD_RATE);
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

#include "AB_BLE.h"

AB_BLE::AB_BLE(Stream *s) : stream(s) {};

int AB_BLE::available() {
    return stream->available();
}

size_t AB_BLE::write(uint8_t c) {
    return stream->write(c);
}

void AB_BLE::writeBytes(unsigned char *data, unsigned char len) {
    delay(10);
    if(len > 0) {
        stream->write(data, len);
    }
}

void AB_BLE::println(const char data[]) {
    stream->println(data);
}

size_t AB_BLE::println(int n, int base) {
    stream->println(n, base);
}

int AB_BLE::read() {
    delay(10);	
    return stream->read();
}


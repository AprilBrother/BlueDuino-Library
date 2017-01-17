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

void AB_BLE::println(const __FlashStringHelper *data) {
    stream->println(data);
}

size_t AB_BLE::println(int n, int base) {
    stream->println(n, base);
}

int AB_BLE::read() {
    return stream->read();
}

bool AB_BLE::waitOk(void) {
    String resp = stream->readStringUntil('\n');
    if (resp.startsWith("OK")) {
        return 1;
    }
    strcpy(buffer, resp.c_str());
    return 0;
}

bool AB_BLE::at(const __FlashStringHelper *cmd) {
    println(cmd);
    return waitOk();
}

bool AB_BLE::at(const char cmd[]) {
    println(cmd);
    return waitOk();
}


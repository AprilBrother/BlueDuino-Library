#ifndef  _AB_BLE_H
#define _AB_BLE_H

#include <Arduino.h>

class AB_BLE : public Print {

    public:
        AB_BLE(Stream *s = &Serial1);
        void writeBytes(unsigned char *data, unsigned char len);
        void println(const char data[]);

        int available();
        int read();

        // Class Print virtual function Interface
        virtual size_t write(uint8_t c);
        virtual size_t println(int, int = DEC);


        // pull in write(str) and write(buf, size) from Print
        using Print::write;

    private:
        Stream    *stream;     // -> BLE module, e.g. SoftwareSerial or Serial1

};

#endif

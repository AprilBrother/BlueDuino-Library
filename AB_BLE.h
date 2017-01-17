#ifndef  _AB_BLE_H
#define _AB_BLE_H

#include <Arduino.h>

#define AB_BUF_SIZE 65

class AB_BLE : public Print {

    protected:
        bool     _verbose;

    private:
        Stream    *stream;     // -> BLE module, e.g. SoftwareSerial or Serial1

        bool at(const __FlashStringHelper *cmd);
        bool at(const char cmd[]);

    public:
        char buffer[AB_BUF_SIZE];

        AB_BLE(Stream *s = &Serial1);
        void writeBytes(unsigned char *data, unsigned char len);
        void println(const char data[]);
        void println(const __FlashStringHelper *data);

        void verbose(bool enable) { _verbose = enable; }

        int available();
        int read();

        bool waitOk(void);
        bool sendCmdUntilOk(const __FlashStringHelper *cmd) { return this->at(cmd); }
        bool sendCmdUntilOk(const char cmd[]) { return this->at(cmd); }

        // Class Print virtual function Interface
        virtual size_t write(uint8_t c);
        virtual size_t println(int, int = DEC);

        // pull in write(str) and write(buf, size) from Print
        using Print::write;

};

#endif

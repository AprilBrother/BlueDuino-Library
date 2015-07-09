/*
YourDuino.com Example Software Sketch
DHT11 Humidity and Temperature Sensor test
Credits: Rob Tillaart
http://arduino-direct.com/sunshop/index.php?l=product_detail&p=162
terry@yourduino.com

Modified: Yanc
*/

/*-----( Import needed libraries )-----*/
#include <dht11.h>
#include <AB_BLE.h>

/*-----( Declare objects )-----*/
dht11 DHT11;
AB_BLE ble(&Serial1);

/*-----( Declare Constants, Pin Numbers )-----*/
#define DHT11PIN 21

// GROUP
#define MAJOR 100
#define MINOR 1

int flag = 0;

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
    Serial.begin(9600);
    Serial1.begin(9600);

    //while(!Serial);

    Serial.println("DHT11 TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT11LIB_VERSION);
    Serial.println();

    ble.println("AT+ROLE0");
    delay(100);
    ble.println("AT+RESET");
    delay(400);

    // Set proximity UUID for sensors
    ble.println("AT+PUID06742A694C2C4359BBFE122D05B6DCBF");
    delay(100);
    // Set sensor id
    ble.write("AT+MAJR");
    ble.write(MAJOR);
    delay(100);
    ble.write("AT+MINR");
    ble.write(MINOR);
    delay(100);
}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
    Serial.println("\n");

    int chk = DHT11.read(DHT11PIN);

    Serial.print("Read sensor: ");
    switch (chk)
    {
        case 0: Serial.println("OK"); break;
        case -1: Serial.println("Checksum error"); break;
        case -2: Serial.println("Time out error"); break;
        default: Serial.println("Unknown error"); break;
    }

    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);

    Serial.print("Temperature (oC): ");
    Serial.println((float)DHT11.temperature, 2);

    Serial.print("Temperature (oF): ");
    Serial.println(Fahrenheit(DHT11.temperature), 2);

    Serial.print("Temperature (K): ");
    Serial.println(Kelvin(DHT11.temperature), 2);

    Serial.print("Dew Point (oC): ");
    Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));

    Serial.print("Dew PointFast (oC): ");
    Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));


    // Reverse the flag for advertising iBeacon data or temperature/humidity data
    flag = !flag;

    if (flag) {
        int minor = DHT11.humidity;
        minor = (minor << 8) + DHT11.temperature;
        ble.write("AT+MINR");
        ble.println(minor, DEC);
        delay(100);
        ble.println("AT+PUIDE2C56DB5DFFB48D2B060D0F5A71096E1");
    } else {
        ble.println("AT+PUID06742A694C2C4359BBFE122D05B6DCBF");
        // Set sensor id
        delay(100);
        ble.write("AT+MINR");
        ble.write(MINOR);
    }

    delay(100);
    ble.println("AT+RESET");

    delay(2000);
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
//
//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
    return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
    return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm
double dewPoint(double celsius, double humidity)
{
    double A0 = 373.15 / (273.15 + celsius);
    double SUM = -7.90298 * (A0 - 1);
    SUM += 5.02808 * log10(A0);
    SUM += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / A0))) - 1) ;
    SUM += 8.1328e-3 * (pow(10, (-3.49149 * (A0 - 1))) - 1) ;
    SUM += log10(1013.246);
    double VP = pow(10, SUM - 3) * humidity;
    double T = log(VP / 0.61078); // temp var
    return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
    double a = 17.271;
    double b = 237.7;
    double temp = (a * celsius) / (b + celsius) + log(humidity / 100);
    double Td = (b * temp) / (a - temp);
    return Td;
}

/* ( THE END ) */


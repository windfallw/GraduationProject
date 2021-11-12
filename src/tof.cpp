#include <Arduino.h>
#include "tof.h"

const char AutoMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xF2, 0xAA};
const char startMeasure[] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xAA};

SKPTOFLIDAR::SKPTOFLIDAR(HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx)
{
    uart = u;
    uart->begin(baudrate, SERIAL_8N1, rx, tx);
}

SKPTOFLIDAR::~SKPTOFLIDAR()
{
}

void SKPTOFLIDAR::start()
{
    uart->write(startMeasure);
}

void SKPTOFLIDAR::read()
{
    if (uart->available())
    {
        uint8_t buf[8] = {};
        uart->readBytes(buf, 8);

        if (buf[0] == 0x55 && buf[7] == 0xAA)
        {
            for (int i = 0; i < 8; i++)
            {
                Serial.print(buf[i], HEX);
            }
            Serial.println();
        }
    }
    // if (uart->available())
    // {
    //     Serial.println(uart->read());
    // }
}

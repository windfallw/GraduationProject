#include "tof.h"

hw_timer_t *tim1;

bool buzzer_Open = false;
bool buzzer_req_off = false;

void IRAM_ATTR onTim1()
{
    buzzer_req_off = true;
}

shinelight::shinelight(uint8_t timerNum, uint8_t pin, uint8_t channel, uint8_t resolution)
{
    this->pin = pin;
    this->channel = channel;
    this->resolution = resolution;

    tim1 = timerBegin(timerNum, 80, true);
    timerAttachInterrupt(tim1, &onTim1, true);

    ledcSetup(channel, cg.alarm.freq, resolution);
    ledcAttachPin(pin, channel);
    ledcWrite(channel, 0);
}

shinelight::~shinelight()
{
    ledcDetachPin(pin);
}

void shinelight::open()
{
    if (buzzer_Open)
        timerWrite(tim1, 0);

    else
    {
        ledcWrite(channel, cg.alarm.dutyCycle);
        buzzer_Open = true;
        timerAlarmWrite(tim1, cg.alarm.ms * 1000, false); // value in microseconds
        timerWrite(tim1, 0);
        timerAlarmEnable(tim1);
    }
}

void shinelight::close()
{
    ledcWrite(channel, 0);
    buzzer_Open = false;
    timerAlarmDisable(tim1);
    buzzer_req_off = false;
}

void shinelight::check()
{
    if (buzzer_req_off)
        close();
}

void shinelight::writeCycle(uint32_t cycle)
{
    if (ledcRead(channel) != cycle)
    {
        cg.alarm.dutyCycle = cycle;
        ledcWrite(channel, cg.alarm.dutyCycle);
    }
}

void shinelight::writeFreq(uint32_t fq)
{
    if (ledcReadFreq(channel) != fq)
    {
        cg.alarm.freq = fq;
        ledcSetup(channel, cg.alarm.freq, resolution);
    }
}

void shinelight::fade_test()
{
    uint32_t dutyCycle = 0;
    while (dutyCycle < 0xff)
    {
        dutyCycle++;
        ledcWrite(channel, dutyCycle);
    }

    while (dutyCycle > 0)
    {
        dutyCycle--;
        ledcWrite(channel, dutyCycle);
    }
}

// below are ToF LiDAR Part

constexpr const char
    crcTable[] = {0, 49, 98, 83, 196, 245, 166, 151, 185, 136, 219, 234, 125, 76, 31, 46, 67, 114, 33, 16, 135, 182, 229, 212,
                  250, 203, 152, 169, 62, 15, 92, 109, 134, 183, 228, 213, 66, 115, 32, 17, 63, 14, 93, 108, 251, 202, 153, 168,
                  197, 244, 167, 150, 1, 48, 99, 82, 124, 77, 30, 47, 184, 137, 218, 235, 61, 12, 95, 110, 249, 200, 155, 170,
                  132, 181, 230, 215, 64, 113, 34, 19, 126, 79, 28, 45, 186, 139, 216, 233, 199, 246, 165, 148, 3, 50, 97, 80,
                  187, 138, 217, 232, 127, 78, 29, 44, 2, 51, 96, 81, 198, 247, 164, 149, 248, 201, 154, 171, 60, 13, 94, 111,
                  65, 112, 35, 18, 133, 180, 231, 214, 122, 75, 24, 41, 190, 143, 220, 237, 195, 242, 161, 144, 7, 54, 101, 84,
                  57, 8, 91, 106, 253, 204, 159, 174, 128, 177, 226, 211, 68, 117, 38, 23, 252, 205, 158, 175, 56, 9, 90, 107,
                  69, 116, 39, 22, 129, 176, 227, 210, 191, 142, 221, 236, 123, 74, 25, 40, 6, 55, 100, 85, 194, 243, 160, 145,
                  71, 118, 37, 20, 131, 178, 225, 208, 254, 207, 156, 173, 58, 11, 88, 105, 4, 53, 102, 87, 192, 241, 162, 147,
                  189, 140, 223, 238, 121, 72, 27, 42, 193, 240, 163, 146, 5, 52, 103, 86, 120, 73, 26, 43, 188, 141, 222, 239,
                  130, 179, 224, 209, 70, 119, 36, 21, 59, 10, 89, 104, 255, 206, 157, 172};

constexpr const char startMeasure[] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xAA};
constexpr const char stopMeasure[] = {0x55, 0x06, 0x00, 0x00, 0x00, 0x00, 0x88, 0xAA};

constexpr const char singleMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x01, 0xC3, 0xAA};
constexpr const char AutoMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xF2, 0xAA};
constexpr const char autoButCloseMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x02, 0x90, 0xAA};

constexpr const char deviceInfo[] = {0x55, 0x01, 0x00, 0x00, 0x00, 0x00, 0xD3, 0xAA};
constexpr const char byteOutPut[] = {0x55, 0x04, 0x00, 0x00, 0x00, 0x01, 0x2E, 0xAA};
constexpr const char pixhawkOutPut[] = {0x55, 0x04, 0x00, 0x00, 0x00, 0x02, 0x7D, 0xAA};

SKPTOFLIDAR::SKPTOFLIDAR(uint8_t id, HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx)
{
    _id = id;
    uart = u;
    uart->begin(baudrate, SERIAL_8N1, rx, tx);
    uart->write(AutoMode, sizeof(AutoMode));
}

SKPTOFLIDAR::~SKPTOFLIDAR()
{
}

void SKPTOFLIDAR::start()
{
    uart->write(startMeasure, sizeof(startMeasure));
}

void SKPTOFLIDAR::stop()
{
    uart->write(stopMeasure, sizeof(stopMeasure));
}

void SKPTOFLIDAR::print_buffs()
{
    for (int i = 0; i < sizeof(buffs); i++)
    {
        Serial.printf("%#.2X ", buffs[i]);
    }
    Serial.println();
}

uint8_t SKPTOFLIDAR::read_tof()
{
    if (uart->available() > 8)
    {
        uart->readBytes(buffs, 8);
        // print_buffs();
        if (buffs[0] == 0x55 && buffs[7] == 0xAA && digest(buffs))
        {
            // checksum fail stop at here.
            if (buffs[1] == 0x07 && buffs[2] == 0x00)
            {
                // further assert.
                uint8_t tmp[4] = {buffs[5], buffs[4], buffs[3], buffs[2]};
                memcpy(&distance, &tmp, 4);
                // Serial.println(distance);
                return true;
            }
        }
    }
    return false;
}

uint8_t SKPTOFLIDAR::handler()
{
    if (!read_tof())
        return false;

    if (_id == 1 && distance < cg.alarm.tof1)
        return true;

    if (_id == 2 && distance < cg.alarm.tof2)
        return true;

    return false;
}

uint8_t digest(uint8_t buffer[])
{
    uint8_t crc = 0x00;
    for (int i = 1; i < 6; i++)
    {
        crc ^= buffer[i];
        crc = crcTable[crc];
    }
    if (crc == buffer[6])
        return true;
    return false;
}
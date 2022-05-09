#include "tof.h"
#include "file.h"

alarm_log_t alog[TOFDEVICENUMBER + 1];

hw_timer_t *shinelight::timer = NULL;
uint8_t shinelight::IsOn = false;
uint8_t shinelight::ReqOff = false;

void shinelight::onTimerOut()
{
    ReqOff = true;
}

shinelight::shinelight(uint8_t timerNum, uint8_t pin, uint8_t channel, uint8_t resolution)
{
    this->pin = pin;
    this->channel = channel;
    this->resolution = resolution;

    timer = timerBegin(timerNum, 80, true);
    timerAttachInterrupt(timer, &onTimerOut, true);
    ledcAttachPin(pin, channel);

    ledcSetup(channel, syscg.alarm.freq, resolution);
    ledcWrite(channel, 0);
}

shinelight::~shinelight()
{
    ledcDetachPin(pin);
}

void shinelight::open()
{
    if (IsOn)
    {
        timerWrite(timer, 0);
        return;
    }

    if (!syscg.alarm.ms)
        return;

    ledcSetup(channel, syscg.alarm.freq, resolution);
    ledcWrite(channel, syscg.alarm.dutyCycle);

    IsOn = true;
    timerAlarmWrite(timer, syscg.alarm.ms * 1000, false); // value in microseconds
    timerWrite(timer, 0);
    timerAlarmEnable(timer);
}

void shinelight::close()
{
    ledcWrite(channel, 0);
    IsOn = false;
    timerAlarmDisable(timer);
    ReqOff = false;
}

void shinelight::writeFreq(uint32_t fq, bool autoload)
{
    // ledcReadFreq(channel)
    ledcSetup(channel, fq, resolution);

    if (autoload)
        ledcWrite(channel, syscg.alarm.dutyCycle);
}

void shinelight::writeCycle(uint32_t cycle)
{
    // ledcRead(channel)
    ledcWrite(channel, cycle);
}

void shinelight::fade_test()
{
    uint32_t dutyCycle = 0;
    while (dutyCycle < MAXDUTYCYCLE)
    {
        dutyCycle++;
        ledcWrite(channel, dutyCycle);
        delay(1);
    }

    while (dutyCycle > 0)
    {
        dutyCycle--;
        ledcWrite(channel, dutyCycle);
        delay(1);
    }
}

// below are ToF LiDAR Part

static const uint8_t
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

static const uint8_t startMeasure[] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xAA};
static const uint8_t stopMeasure[] = {0x55, 0x06, 0x00, 0x00, 0x00, 0x00, 0x88, 0xAA};

static const uint8_t singleMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x01, 0xC3, 0xAA};
static const uint8_t AutoMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x00, 0xF2, 0xAA};
static const uint8_t autoButCloseMode[] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x02, 0x90, 0xAA};

static const uint8_t deviceInfo[] = {0x55, 0x01, 0x00, 0x00, 0x00, 0x00, 0xD3, 0xAA};
static const uint8_t byteOutPut[] = {0x55, 0x04, 0x00, 0x00, 0x00, 0x01, 0x2E, 0xAA};
static const uint8_t pixhawkOutPut[] = {0x55, 0x04, 0x00, 0x00, 0x00, 0x02, 0x7D, 0xAA};

static uint8_t digest(uint8_t buffer[]);

SKPTOFLIDAR::SKPTOFLIDAR(uint8_t tid, HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx)
{
    this->tid = tid;
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
    // 0x55 0x07 0x00 0x00 0x05 0xAD 0x9C 0xAA
    //  frame header[55] Key[07] system status[00] Distance[00 05 AD] CRC[9C] frame end[AA]

    if (uart->available() < 8)
        return false;

    if (uart->peek() == 0x55)
    {
        uart->readBytes(buffs, 8);
        if (buffs[1] == 0x07 && buffs[2] == 0x00 && buffs[7] == 0xAA && digest(buffs))
        {
            uint8_t tmp[4] = {buffs[5], buffs[4], buffs[3], buffs[2]};
            memcpy(&distance, &tmp, 4);
            // Serial.println(distance);

            return true;
        }
        else
            // format error
            print_buffs();
    }
    else
        uart->read(); // read first byte of incoming serial data

    return false;
}

uint8_t SKPTOFLIDAR::handler(uint32_t limit)
{
    if (!read_tof())
        // didn't receive new data
        return false;

    if (pause)
        // pause alert
        return false;

    if (distance < limit)
    {
        if (tid > TOFDEVICENUMBER)
            tid = 0;

        if (!alog[tid].onpub)
        {
            alog[tid].distance = distance;
            alog[tid].limit = limit;
            alog[tid].onpub = true;
        }
        return true;
    }

    return false;
}

static uint8_t digest(uint8_t buffer[])
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
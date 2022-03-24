#ifndef TOFH
#define TOFH

#include <Arduino.h>
#include "file.h"

#define TOFDEVICENUMBER 2

// use npn mosfet  ctrl light & buzzer
class shinelight
{
public:
    static hw_timer_t *timer;
    static uint8_t IsOn;
    static uint8_t ReqOff;
    static void IRAM_ATTR onTimerOut();

    uint8_t pin;
    uint8_t channel;
    uint8_t resolution;

    shinelight(uint8_t timerNum, uint8_t pin, uint8_t channel, uint8_t resolution = 8);
    ~shinelight();

    void open();
    void close();

    void writeCycle(uint32_t cycle);
    void writeFreq(uint32_t fq);

    void fade_test();
};

// RX1->15 TX1->14   RX2->16 TX2->17
class SKPTOFLIDAR
{
public:
    uint8_t tid;
    HardwareSerial *uart;
    uint8_t buffs[8] = {};
    uint32_t distance = 0;

    SKPTOFLIDAR(uint8_t tid, HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx);
    ~SKPTOFLIDAR();

    void start();
    void stop();
    uint8_t read_tof();
    uint8_t handler(uint32_t limit);

    void print_buffs();
};

struct alarmlog_t
{
    bool onpub = false;
    uint32_t distance = 0;
    uint32_t limit = 0;
};

extern struct alarmlog_t alog[TOFDEVICENUMBER + 1];

uint8_t digest(uint8_t buffer[]);

#endif
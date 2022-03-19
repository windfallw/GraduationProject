#ifndef TOFH
#define TOFH

#include <Arduino.h>
#include "file.h"

extern bool is_timeout;
extern hw_timer_t *timer;

// use npn mosfet  ctrl light & buzzer
class shinelight
{
public:
    uint8_t pin;
    uint8_t channel;
    uint8_t resolution;
    double freq;
    uint32_t dutyCycle;

    shinelight(uint8_t timerNum, uint8_t pin, uint8_t channel, double freq, uint8_t resolution = 8);
    ~shinelight();

    void writeCycle(uint32_t cycle);
    void writeFreq(uint32_t fq);

    void fade_test();
};

// RX1->15 TX1->14   RX2->16 TX2->17
class SKPTOFLIDAR
{
private:
    uint8_t _id;

public:
    HardwareSerial *uart;
    uint8_t buffs[8] = {};
    uint32_t distance = 0;

    SKPTOFLIDAR(uint8_t id, HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx);
    ~SKPTOFLIDAR();

    void start();
    void stop();
    void print_buffs();
    uint8_t read_tof();
    uint8_t handler();
};

uint8_t digest(uint8_t buffer[]);

#endif
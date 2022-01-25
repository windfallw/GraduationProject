#ifndef TOFH
#define TOFH

// use npn mosfet  ctrl light & buzzer
class shinelight
{
public:
    uint8_t pin;
    uint8_t channel;
    uint8_t resolution;
    double freq;

    shinelight(uint8_t pin, uint8_t channel, uint8_t resolution, double freq);
    ~shinelight();

    void set_up();
    void write(uint16_t dutyCycle);

    void fade_test();
};

// RX1->15 TX1->14   RX2->16 TX2->17
class SKPTOFLIDAR
{
private:
public:
    HardwareSerial *uart;
    uint8_t buffs[8] = {};
    uint32_t distance = 0;

    SKPTOFLIDAR(HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx);
    ~SKPTOFLIDAR();

    void start();
    void stop();
    void print_buffs();
    void read_handler();
};

int digest(uint8_t buffer[]);

#endif
#ifndef TOFH
#define TOFH

// #define RX1 15
// #define TX1 14

class SKPTOFLIDAR
{
private:
public:
    SKPTOFLIDAR(HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx);
    ~SKPTOFLIDAR();

    HardwareSerial *uart;

    uint8_t buffs[8] = {};
    uint32_t distance = 0;

    void start();
    void print_buffs();
    void read_handler();
};

int digest(uint8_t buffer[]);

#endif
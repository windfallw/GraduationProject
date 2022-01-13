#ifndef TOFH
#define TOFH

// #define RX1 15
// #define TX1 14
// #define RX2 16
// #define TX2 17

// ledcSetup(0, 100, 8);
// ledcAttachPin(13, 0);
// ledcWrite(0, 30);

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
    void stop();
    void print_buffs();
    void read_handler();
};

int digest(uint8_t buffer[]);

#endif
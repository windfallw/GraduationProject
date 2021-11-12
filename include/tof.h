#ifndef TOFH
#define TOFH

// #define RX1 14
// #define TX1 15

class SKPTOFLIDAR
{
public:
    SKPTOFLIDAR(HardwareSerial *u, u_long baudrate, int8_t rx, int8_t tx);
    ~SKPTOFLIDAR();

    HardwareSerial *uart;

    void read();
    void start();
};

#endif
#ifndef TOFH
#define TOFH

// #define RX1 15
// #define TX1 14
// #define RX2 21
// #define TX2 13

// Serial2.begin(115200, SERIAL_8N1, 16, 17);
// if (Serial2.available())
// {
//     Serial.print(Serial2.readString());
// }

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
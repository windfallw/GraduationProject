# GraduationProject

my undergraduate graduation project

**Alarm System for Intrusion Detection Based on ToF LiDAR**



## Hardware

I used self-designed PCB, but you can make some modify to run on your projects.

- ESP32-WROOM-32E
- SKP20 ToF LiDAR * 2
- 2.8 inch ST7789V TFT
- rotary encoder
- NPN MOSFET with buzzer and LED
- IP5108 Power SoC
- Li-ion battery (2000-5000 mAh)
- A switch to toggle the power connection

## Compile

1. use git clone this repo

2. download vscode and platformio extension

3. if you only want to have a look on the lvgl ui, just open the `lvgl_simulator` folder (only verified on macOS)

4. if you want the firmware run on ESP32 open `WirelessAlarmSystem` folder
   - wait for platformio download dependent librarys complete
   
   - use git restore the files [here](https://github.com/windfallw/GraduationProject/tree/main/WirelessAlarmSystem/libdeps/esp32dev) (`TFT_eSPI` and `lvgl`)
   
   - for upload firmware via serial your ESP32 board need auto program circuit
   
## Usage

1. scan the qrcode on TFT screen connect to the ESP32 AP or use password `12345678`
2. waiting for the captive portal response or just visiting `192.168.4.1`
3. setting up ESP32 Wi-Fi connection, MQTT broker domain(IP) and port
3. All the settings are saved on LittleFS filesystem, named `config.json`, you don't need to set up again on next boot
3. There is a web page for devices management hosting on cloud server, but the code I wrote was so bad, so I didn't open soure it 😢 (the mqtt broker uses emqx)

## Acknowledgements

- [arduino-esp32](https://github.com/espressif/arduino-esp32)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [async-mqtt-client](https://github.com/marvinroger/async-mqtt-client)
- [AsyncElegantOTA](https://github.com/ayushsharma82/AsyncElegantOTA)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [littlefs](https://github.com/littlefs-project/littlefs)
- [lvgl](https://github.com/lvgl)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [ESP32Encoder](https://github.com/madhephaestus/ESP32Encoder)

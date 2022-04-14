#include "file.h"

#define FORMAT_LITTLEFS_IF_FAILED true
#define CONFIG_LITTLEFS_BASE_PATH "/littlefs"
#define CONFIG_LITTLEFS_MAX_OPEN_FILES 10
#define CONFIG_LITTLEFS_PARTITION_LABEL "littlefs"

#define CONFIG_LITTLEFS_SPIFFS_COMPAT false
#define CONFIG_LITTLEFS_CACHE_SIZE 512

constexpr const char *config_file_path = "/config.json";

struct cg_t init_config()
{
    uint8_t mac[6];
    char *apssid = (char *)malloc(10); // use free() to release
    char *macAddr = (char *)malloc(20);

    esp_efuse_mac_get_default(mac);
    sprintf(apssid, "ESP%02X%02X", mac[4], mac[5]);
    sprintf(macAddr, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    struct cg_t config = {
        {{"example1", "password1"},
         {"example2", "password2"},
         {"example3", "password3"}},
        {apssid, "12345678"},
        {20000, 1000, 1000, 10000, 500, 10, 30},
        {macAddr, "public", "public", "server-publish", "client-submit", "example.com", 1883}};

    return config;
}

struct cg_t cg = init_config();
struct conn_t CONN_SIGN;

void set_littlefs()
{
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED, CONFIG_LITTLEFS_BASE_PATH, CONFIG_LITTLEFS_MAX_OPEN_FILES, CONFIG_LITTLEFS_PARTITION_LABEL))
    {
        Serial.println(F("LITTLEFS Mount Failed"));
        return;
    }
    readConfigFile();
}

void listDir(const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);
    File root = LITTLEFS.open(dirname);
    if (!root)
    {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println(" - not a directory");
        return;
    }
    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.print(file.name());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.print(file.size());
            time_t t = file.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
        }
        file = root.openNextFile();
    }
}

void readFile(const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = LITTLEFS.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    Serial.println();

    file.close();
}

void writeFile(const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = LITTLEFS.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

void readConfigFile()
{
    if (!LITTLEFS.exists(config_file_path))
    {
        // first time run config file not exist
        writeConfigFile();
    }

    File file = LITTLEFS.open(config_file_path, "r");
    if (!file)
    {
        // a big problem occur remove config file and restart
        Serial.println(F("[E]config file open failed"));
        resetConfigFile(); // -> this function will restart the mcu
        return;
    }

    /* don’t use a global JsonDocument
    https://arduinojson.org/v6/issues/memory-leak/*/
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        // config file was corrupted remove config file and restart
        Serial.print(F("[E]deserializeJson() failed: "));
        Serial.println(error.c_str());
        resetConfigFile();
        return;
    }

    JsonObject wifi;
    JsonArray wifi_sta;
    JsonObject wifi_ap;
    if (doc["wifi"]["sta"].is<JsonArray>() && doc["wifi"]["ap"].is<JsonObject>())
    {
        wifi = doc["wifi"];
        wifi_sta = doc["wifi"]["sta"];
        wifi_ap = doc["wifi"]["ap"];
    }
    else
        resetConfigFile();

    JsonObject alarm;
    if (doc["alarm"].is<JsonObject>())
        alarm = doc["alarm"];
    else
        resetConfigFile();

    JsonObject mqtt;
    if (doc["mqtt"].is<JsonObject>())
        mqtt = doc["mqtt"];
    else
        resetConfigFile();

    // read the temp doc and put it into the config doc
    uint8_t index = 0;
    for (JsonObject wifi_sta_item : doc["wifi"]["sta"].as<JsonArray>())
    {
        cg.sta[index].ssid = const_cast<char *>((const char *)wifi_sta_item["ssid"]);
        cg.sta[index].pwd = const_cast<char *>((const char *)wifi_sta_item["pwd"]);
        index++;
        if (index >= 3)
            break;
    }

    /* may be useful https://github.com/bblanchon/ArduinoJson/issues/71#issuecomment-99364015
    const char *gatewayString = root["gateway"];
    char *octetoAux = const_cast<char *>(gatewayString);*/
    cg.ap.ssid = const_cast<char *>((const char *)doc["wifi"]["ap"]["ssid"]);
    cg.ap.pwd = const_cast<char *>((const char *)doc["wifi"]["ap"]["pwd"]);

    // cg.alarm.tofMax = alarm["tofMax"];
    cg.alarm.tof1 = alarm["tof1"];
    cg.alarm.tof2 = alarm["tof2"];
    // cg.alarm.msMax = alarm["msMax"];
    cg.alarm.ms = alarm["ms"];
    cg.alarm.freq = alarm["freq"];
    cg.alarm.dutyCycle = alarm["dutyCycle"];

    // cg.mqtt.macddr = const_cast<char *>((const char *)mqtt["macddr"]);
    cg.mqtt.user = const_cast<char *>((const char *)mqtt["user"]);
    cg.mqtt.pwd = const_cast<char *>((const char *)mqtt["pwd"]);
    cg.mqtt.subscribe = const_cast<char *>((const char *)mqtt["subscribe"]);
    cg.mqtt.publish = const_cast<char *>((const char *)mqtt["publish"]);
    cg.mqtt.server = const_cast<char *>((const char *)mqtt["server"]);
    cg.mqtt.port = mqtt["port"];

    // serializeJsonPretty(doc, Serial);
    // Serial.println();

    /* for the first time initial the config file
     for the second time can correct the corrupted config file*/
    writeConfigFile();
}

void writeConfigFile()
{
    File file = LITTLEFS.open(config_file_path, "w");
    if (!file)
    {
        Serial.println(F("[E] Failed to open config file for writing"));
        resetConfigFile();
        return;
    }

    StaticJsonDocument<1024> doc;
    JsonObject wifi = doc.createNestedObject("wifi");
    JsonArray wifi_sta = wifi.createNestedArray("sta");
    JsonObject wifi_ap = wifi.createNestedObject("ap");
    JsonObject alarm = doc.createNestedObject("alarm");
    JsonObject mqtt = doc.createNestedObject("mqtt");

    for (int i = 0; i < 3; i++)
    {
        JsonObject wifi_sta_obj = wifi_sta.createNestedObject();
        wifi_sta_obj["ssid"] = cg.sta[i].ssid;
        wifi_sta_obj["pwd"] = cg.sta[i].pwd;
    }

    wifi_ap["ssid"] = cg.ap.ssid;
    wifi_ap["pwd"] = cg.ap.pwd;

    alarm["tofMax"] = cg.alarm.tofMax;
    alarm["tof1"] = cg.alarm.tof1;
    alarm["tof2"] = cg.alarm.tof2;
    alarm["msMax"] = cg.alarm.msMax;
    alarm["ms"] = cg.alarm.ms;
    alarm["freq"] = cg.alarm.freq;
    alarm["dutyCycle"] = cg.alarm.dutyCycle;

    mqtt["macddr"] = cg.mqtt.macddr;
    mqtt["user"] = cg.mqtt.user;
    mqtt["pwd"] = cg.mqtt.pwd;
    mqtt["subscribe"] = cg.mqtt.subscribe;
    mqtt["publish"] = cg.mqtt.publish;
    mqtt["server"] = cg.mqtt.server;
    mqtt["port"] = cg.mqtt.port;

    cg.stream.clear();
    if (!serializeJson(doc, file) || !serializeJson(doc, cg.stream))
    {
        Serial.println(F("[E] serializeJson Failed"));
        file.close();
        resetConfigFile();
    }
}

void resetConfigFile()
{
    if (LITTLEFS.remove(config_file_path))
        Serial.println(F("config file removed"));
    else
        Serial.println(F("config file remove failed"));

    ESP.restart();
}

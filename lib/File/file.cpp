#include "file.h"
#include "ArduinoJson.h"

#define FORMAT_LITTLEFS_IF_FAILED true
#define CONFIG_LITTLEFS_SPIFFS_COMPAT false
#define CONFIG_LITTLEFS_CACHE_SIZE 512

const char *config_file_path = "/config.json";

void set_littlefs()
{
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
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
    // only use once in system setup.
    if (!LITTLEFS.exists(config_file_path))
    {
        writeConfigFile();
        Serial.printf("%s created\r\n", config_file_path);
        return;
    }

    StaticJsonDocument<1024> doc;
    File file = LITTLEFS.open(config_file_path, "r");
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.printf("Failed to read %s, deleting...\r\n", config_file_path);
        if (LITTLEFS.remove(config_file_path))
        {
            Serial.println(F("file deleted, rebooting...\r\n"));
        }
        else
        {
            Serial.println(F("delete failed, rebooting...\r\n"));
        }
        yield();
        delay(1000);
        yield();
        ESP.restart();
        return;
    }

    config.limit = doc["limit"] | 1000;
    strlcpy(config.hostname,                 // <- destination
            doc["hostname"] | "example.com", // <- source
            sizeof(config.hostname));        // <- destination's capacity

    serializeJsonPretty(doc, Serial);
    // Serial.printf("%s %d \r\n", config.hostname, config.limit);
    // listDir("/", 3);
}

void writeConfigFile()
{
    File file = LITTLEFS.open(config_file_path, "w");
    if (!file)
    {
        Serial.println(F("Failed to open config file for writing"));
        return;
    }

    StaticJsonDocument<1024> doc;
    doc["limit"] = config.limit;
    doc["hostname"] = config.hostname;

    if (!serializeJson(doc, file))
    {
        Serial.println(F("serializeJson Failed"));
    }

    file.close();
    return;
}

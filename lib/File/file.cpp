#include "file.h"

#include "ArduinoJson.h"

#define FORMAT_LITTLEFS_IF_FAILED true
#define CONFIG_FILE "/config.json"

void set_littlefs()
{
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        Serial.println("LITTLEFS Mount Failed");
    }
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

bool readConfigFile()
{
    if (!LITTLEFS.exists(CONFIG_FILE))
        return false;

    File file = LITTLEFS.open(CONFIG_FILE, "r");

    StaticJsonDocument<512> doc;

    DeserializationError error = deserializeJson(doc, file);

    file.close();

    if (error)
    {
        Serial.println(F("Failed to read file, using default configuration"));
        return false;
    }

    config.limit = doc["limit"];

    strlcpy(config.hostname,                 // <- destination
            doc["hostname"] | "example.com", // <- source
            sizeof(config.hostname));        // <- destination's capacity

    return true;
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

bool writeConfigFile()
{
    Serial.println("Saving config file");

    File file = LITTLEFS.open(CONFIG_FILE, "w");

    if (!file)
    {
        Serial.println(F("Failed to open config file for writing"));
        return false;
    }

    StaticJsonDocument<512> doc;

    doc["limit"] = config.limit;
    doc["hostname"] = config.hostname;

    if (serializeJson(doc, file) == 0)
    {
        Serial.println(F("Failed to write to file"));
    }

    file.close();

    return true;
}

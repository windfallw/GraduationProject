#include "netsrv.h"
#include "file.h"

const char *sta_ssid = "";
const char *sta_password = "";

const char *ap_ssid = "ESP32_AP";
const char *ap_password = "12345678";

const char *hostname = "esptest";

AsyncWebServer server(80);
AsyncElegantOtaClass AsyncElegantOTA;

void get_baseinfo()
{
    uint32_t chipId = 0;
    for (int i = 0; i < 17; i = i + 8)
    {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
    Serial.printf("This chip has %d cores\n", ESP.getChipCores());
    Serial.print("Chip ID: ");
    Serial.println(chipId);

    Serial.println(ArduinoOTA.getHostname());
}

void set_OTA()
{
    ArduinoOTA.onStart(
        []()
        {
            String type = "unknown";
            switch (ArduinoOTA.getCommand())
            {
            case U_FLASH:
                type = "sketch";
                break;
            case U_SPIFFS:
                type = "filesystem";
                LITTLEFS.end();
                break;
            default:
                break;
            }
            Serial.println("Start updating " + type);
        });

    ArduinoOTA.onEnd(
        []()
        {
            Serial.println("Update End");
        });

    ArduinoOTA.onProgress(
        [](unsigned int progress, unsigned int total)
        {
            // printf("Progress: %u%%\n", (progress / (total / 100)));
        });

    ArduinoOTA.onError(
        [](ota_error_t error)
        {
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Recieve Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.setPort(3232);
    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.begin();

    MDNS.addService("http", "tcp", 80);
}

void set_netsrv()
{
    WiFi.mode(WIFI_STA); // WIFI_STA WIFI_OFF
    // WiFi.softAP(ap_ssid, ap_password);
    WiFi.begin(sta_ssid, sta_password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("STA: Failed!\n");
        return;
    }

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    set_OTA();

    server.addHandler(new SPIFFSEditor(LITTLEFS, "", ""));

    AsyncElegantOTA.begin(&server);

    server.serveStatic("/", LITTLEFS, "/").setDefaultFile("index.html");

    server.onNotFound(
        [](AsyncWebServerRequest *request)
        {
            Serial.printf("NOT_FOUND: ");
            if (request->method() == HTTP_GET)
                Serial.printf("GET");
            else if (request->method() == HTTP_POST)
                Serial.printf("POST");
            else if (request->method() == HTTP_DELETE)
                Serial.printf("DELETE");
            else if (request->method() == HTTP_PUT)
                Serial.printf("PUT");
            else if (request->method() == HTTP_PATCH)
                Serial.printf("PATCH");
            else if (request->method() == HTTP_HEAD)
                Serial.printf("HEAD");
            else if (request->method() == HTTP_OPTIONS)
                Serial.printf("OPTIONS");
            else
                Serial.printf("UNKNOWN");
            Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

            if (request->contentLength())
            {
                Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
                Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
            }

            int headers = request->headers();
            int i;
            for (i = 0; i < headers; i++)
            {
                AsyncWebHeader *h = request->getHeader(i);
                Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
            }

            int params = request->params();
            for (i = 0; i < params; i++)
            {
                AsyncWebParameter *p = request->getParam(i);
                if (p->isFile())
                {
                    Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
                }
                else if (p->isPost())
                {
                    Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
                else
                {
                    Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
                }
            }

            request->send(404);
        });

    server.onFileUpload(
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
        {
            if (!index)
                Serial.printf("UploadStart: %s\n", filename.c_str());
            Serial.printf("%s", (const char *)data);
            if (final)
                Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
        });

    server.onRequestBody(
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            if (!index)
                Serial.printf("BodyStart: %u\n", total);
            Serial.printf("%s", (const char *)data);
            if (index + len == total)
                Serial.printf("BodyEnd: %u\n", total);
        });

    server.begin();

    Serial.println("HTTP server started");
}
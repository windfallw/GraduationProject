#ifndef NETSRVH
#define NETSRVH

#include <DNSServer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <Update.h>

#include "file.h"

#include "AsyncJson.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFSEditor.h"

#include "elegantWebpage.h"

#include <AsyncMqttClient.h>

class AsyncElegantOtaClass
{
private:
    AsyncWebServer *_server;

    String _id = getID();
    String _username = "";
    String _password = "";
    bool _authRequired = false;

public:
    String getID()
    {
        String id = "";
        id = String((uint32_t)ESP.getEfuseMac(), HEX);
        id.toUpperCase();
        return id;
    }

    void setID(const char *id)
    {
        _id = id;
    }

    void restart()
    {
        yield();
        delay(1000);
        yield();
        ESP.restart();
    }

    void begin(AsyncWebServer *server, const char *username = "", const char *password = "")
    {
        _server = server;

        if (strlen(username) > 0)
        {
            _authRequired = true;
            _username = username;
            _password = password;
        }
        else
        {
            _authRequired = false;
            _username = "";
            _password = "";
        }

        _server->on("/update/identity", HTTP_GET,
                    [&](AsyncWebServerRequest *request)
                    {
                        if (_authRequired)
                        {
                            if (!request->authenticate(_username.c_str(), _password.c_str()))
                            {
                                return request->requestAuthentication();
                            }
                        }
                        request->send(200, "application/json", "{\"id\": \"" + _id + "\", \"hardware\": \"ESP32\"}");
                    });

        _server->on("/update", HTTP_GET,
                    [&](AsyncWebServerRequest *request)
                    {
                        if (_authRequired)
                        {
                            if (!request->authenticate(_username.c_str(), _password.c_str()))
                            {
                                return request->requestAuthentication();
                            }
                        }
                        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", ELEGANT_HTML, ELEGANT_HTML_SIZE);
                        response->addHeader("Content-Encoding", "gzip");
                        request->send(response);
                    });

        _server->on(
            "/update", HTTP_POST,
            [&](AsyncWebServerRequest *request)
            {
                if (_authRequired)
                {
                    if (!request->authenticate(_username.c_str(), _password.c_str()))
                    {
                        return request->requestAuthentication();
                    }
                }
                // the request handler is triggered after the upload has finished...
                // create the response, add header, and send response
                AsyncWebServerResponse *response = request->beginResponse((Update.hasError()) ? 500 : 200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
                response->addHeader("Connection", "close");
                response->addHeader("Access-Control-Allow-Origin", "*");
                request->send(response);
                restart();
            },
            [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
            {
                // Upload handler chunks in data
                if (_authRequired)
                {
                    if (!request->authenticate(_username.c_str(), _password.c_str()))
                    {
                        return request->requestAuthentication();
                    }
                }

                if (!index)
                {
                    if (!request->hasParam("MD5", true))
                    {
                        return request->send(400, "text/plain", "MD5 parameter missing");
                    }

                    if (!Update.setMD5(request->getParam("MD5", true)->value().c_str()))
                    {
                        return request->send(400, "text/plain", "MD5 parameter invalid");
                    }

                    disableCore0WDT();
                    int cmd = (filename == "filesystem") ? U_SPIFFS : U_FLASH;
                    if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
                    {
                        // Start with max available size
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }

                // Write chunked data to the free sketch space
                if (len)
                {
                    if (Update.write(data, len) != len)
                    {
                        return request->send(400, "text/plain", "OTA could not begin");
                    }
                }

                if (final)
                { // if the final flag is set then this is the last frame of data
                    enableCore0WDT();
                    if (!Update.end(true))
                    { // true to set the size to the current progress
                        Update.printError(Serial);
                        return request->send(400, "text/plain", "Could not end OTA");
                    }
                }
                else
                {
                    return;
                }
            });
    }
};

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}
    bool canHandle(AsyncWebServerRequest *request)
    {
        // request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        String href = "http://" + WiFi.softAPIP().toString();
        request->redirect(href);
    }
};

extern DNSServer dnsServer;
extern AsyncWebServer server;
extern AsyncMqttClient mqttClient;

extern bool conn_wifi(bool scan = false);
extern bool conn_wifi(String ssid, String pwd);

extern void set_netsrv();
extern void set_mqtt();
extern void set_checkTimer(uint8_t timerNum);

#endif
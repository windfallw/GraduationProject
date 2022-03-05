#include "netsrv.h"

const char *sta_ssid = "";
const char *sta_password = "";

AsyncWebServer server(80);
AsyncElegantOtaClass AsyncElegantOTA;

constexpr const char *TEXT_MIMETYPE = "text/plain";

void set_OTA(char *hostname)
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
    WiFi.mode(WIFI_AP_STA); // WIFI_STA WIFI_OFF

    WiFi.softAP(cg.ap.ssid.c_str(), cg.ap.pwd.c_str());

    WiFi.scanNetworks();

    WiFi.begin(sta_ssid, sta_password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("STA: Failed!\n");
        return;
    }

    Serial.printf("IP address: %s\r\n", WiFi.localIP().toString().c_str());

    set_OTA(cg.convert(cg.ap.ssid));

    server.on("/machine", HTTP_POST,
              [](AsyncWebServerRequest *request)
              {
                  AsyncWebParameter *p = request->getParam(0);
                  if (p == nullptr)
                      return request->send(400, TEXT_MIMETYPE, "no param");
                  if (p->value() == "reset")
                  {
                      AsyncWebServerResponse *response = request->beginResponse(200, TEXT_MIMETYPE, p->value());
                      response->addHeader("Server", "ESP32WebServer");
                      request->send(response);
                      AsyncElegantOTA.restart();
                  }
                  else if (p->value() == "scan")
                  {
                      WiFi.scanNetworks(true);
                      AsyncWebServerResponse *response = request->beginResponse(200, TEXT_MIMETYPE, p->value());
                      response->addHeader("Server", "ESP32WebServer");
                      request->send(response);
                  }
                  else
                      return request->send(400, TEXT_MIMETYPE, "unknown param");
              });

    server.on("/networkinfo", HTTP_GET,
              [](AsyncWebServerRequest *request)
              {
                  AsyncJsonResponse *response = new AsyncJsonResponse(false, 4096);
                  response->addHeader("Server", "ESP32WebServer");
                  JsonVariant &root = response->getRoot();

                  root["heap"] = ESP.getFreeHeap();

                  JsonObject sta = root.createNestedObject("sta");
                  JsonObject sta_network = sta.createNestedObject("network");
                  sta_network["ip"] = WiFi.localIP().toString();
                  sta_network["subnetMask"] = WiFi.subnetMask().toString();
                  sta_network["gateway"] = WiFi.gatewayIP().toString();
                  sta_network["dns"] = WiFi.dnsIP().toString();

                  sta["ssid"] = WiFi.SSID();
                  sta["rssi"] = WiFi.RSSI();
                  sta["status"] = WiFi.status();
                  sta["mac"] = WiFi.macAddress();
                  sta["isconnect"] = WiFi.isConnected();

                  JsonObject ap = root.createNestedObject("ap");
                  JsonObject ap_network = ap.createNestedObject("network");
                  ap_network["ip"] = WiFi.softAPIP().toString();
                  ap_network["SubnetCIDR"] = WiFi.softAPSubnetCIDR();

                  ap["APNetworkID"] = WiFi.softAPNetworkID().toString();
                  ap["BroadcastIP"] = WiFi.softAPBroadcastIP().toString();
                  ap["ssid"] = WiFi.softAPSSID();
                  ap["StationNum"] = WiFi.softAPgetStationNum();
                  ap["hostname"] = WiFi.softAPgetHostname();
                  ap["mac"] = WiFi.softAPmacAddress();

                  JsonArray scan = root.createNestedArray("scan");
                  int n = WiFi.scanComplete();

                  for (int i = 0; i < n; i++)
                  {
                      JsonObject scan_item = scan.createNestedObject();
                      scan_item["rssi"] = String(WiFi.RSSI(i));
                      scan_item["ssid"] = WiFi.SSID(i);
                      scan_item["bssid"] = WiFi.BSSIDstr(i);
                      scan_item["channel"] = String(WiFi.channel(i));
                      scan_item["secure"] = String(WiFi.encryptionType(i));
                  }

                  response->setLength();
                  request->send(response);
              });

    server.on("/sysinfo", HTTP_GET,
              [](AsyncWebServerRequest *request)
              {
                  return request->send(200, JSON_MIMETYPE, cg.stream);
              });

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

    Serial.print(F("HTTP server started -> "));
    Serial.printf("http://%s.local\r\n", cg.ap.ssid.c_str());
}

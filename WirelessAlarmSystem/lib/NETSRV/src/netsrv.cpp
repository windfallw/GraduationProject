#include "netsrv.h"

static constexpr const char *TEXT_MIMETYPE = "text/plain";

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncElegantOtaClass AsyncElegantOTA;

AsyncMqttClient mqttClient;

static hw_timer_t *timer;

static void IRAM_ATTR onTimerOut()
{
    CONN_SIGN.WiFi = true;
}

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

/* connect to the known wifi in config file
 scan = true to scan for wifi before matching the known wifi*/
bool conn_wifi(bool scan)
{
    int num;
    if (scan)
        num = WiFi.scanNetworks();
    else
        num = WiFi.scanComplete();

    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < 3; j++)
        {
            if (WiFi.SSID(i).equals(cg.sta[j].ssid))
            {
                if (conn_wifi(cg.sta[j].ssid, cg.sta[j].pwd))
                    return true;
            }
        }
    }
    return false;
}

/*just connect to the wifi with variable ssid and pwd
if success save the ssid and pwd to config file
the maximum number of known wifi in config file is 3*/
bool conn_wifi(String ssid, String pwd)
{
    WiFi.disconnect();
    WiFi._setStatus(WL_DISCONNECTED);
    WiFi.begin(ssid.c_str(), pwd.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("STA: Connect to SSID: %s with PWD: %s Failed!\n", ssid.c_str(), pwd.c_str());
        return false;
    }
    else
    {
        Serial.printf("STA: Connection successful to %s\nIP address: %s\n", ssid.c_str(), WiFi.localIP().toString().c_str());

        for (int i = 0; i < 3; i++)
            // if ssid exists in config file
            if (cg.sta[i].ssid == ssid)
            {
                if (cg.sta[i].pwd != pwd)
                {
                    // if pwd not the same save else return
                    cg.sta[i].pwd = pwd;
                    writeConfigFile();
                }
                return true;
            }

        cg.sta[2].ssid = cg.sta[1].ssid;
        cg.sta[2].pwd = cg.sta[1].pwd;
        cg.sta[1].ssid = cg.sta[0].ssid;
        cg.sta[1].pwd = cg.sta[0].pwd;
        cg.sta[0].ssid = ssid;
        cg.sta[0].pwd = pwd;
        writeConfigFile();

        return true;
    }
}

void set_netsrv()
{
    WiFi.mode(WIFI_AP_STA); // WIFI_STA WIFI_OFF

    if (conn_wifi(true)) // scan before connect
    {
        Serial.printf("http://%s\n", WiFi.localIP().toString().c_str());
        Serial.printf("http://%s.local\n", cg.ap.ssid.c_str());
    }

    set_OTA(cg.convert(cg.ap.ssid));

    server.on("/postwifi", HTTP_POST,
              [](AsyncWebServerRequest *request)
              {
                  String ssid = request->arg("ssid");
                  String pwd = request->arg("pwd");
                  if (CONN_SIGN.WEB)
                  {
                      request->send(200, TEXT_MIMETYPE, "PLZ WAITING...");
                  }
                  else
                  {
                      request->send(200, TEXT_MIMETYPE, "CMD RECEIVED! Executing...");
                      CONN_SIGN.ssid = ssid;
                      CONN_SIGN.pwd = pwd;
                      CONN_SIGN.WEB = true;
                  }
              });

    server.on("/postwarn", HTTP_POST,
              [](AsyncWebServerRequest *request)
              {
                  int warnNum = request->arg("warn").toInt();
                  if (warnNum >= 0 && warnNum <= 20)
                  {
                      warnNum = warnNum * 1000; // meter to mm
                      cg.alarm.tof1 = warnNum;
                      cg.alarm.tof2 = warnNum;
                      writeConfigFile();
                      request->send(200, TEXT_MIMETYPE, "Done!");
                  }
                  return request->send(200, TEXT_MIMETYPE, "unknown param");
              });

    server.on("/postwarnsec", HTTP_POST,
              [](AsyncWebServerRequest *request)
              {
                  int warnSec = request->arg("warnSec").toInt();
                  if (warnSec >= 0 && warnSec <= 5000)
                  {
                      cg.alarm.ms = warnSec;
                      writeConfigFile();
                      request->send(200, TEXT_MIMETYPE, "Done!");
                  }
                  return request->send(200, TEXT_MIMETYPE, "unknown param");
              });

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
                      if (!CONN_SIGN.WEB)
                          // avoid scan and conn at the same time!
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

    WiFi.softAP(cg.ap.ssid.c_str(), cg.ap.pwd.c_str()); // WiFi.softAPdisconnect(true);
    WiFi.softAPsetHostname(cg.ap.ssid.c_str());

    dnsServer.start(53, "*", WiFi.softAPIP());
    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP

    Serial.printf("AP started SSID -> %s\n", WiFi.softAPSSID().c_str());
    Serial.printf("http://%s\n", WiFi.softAPIP().toString().c_str());

    server.begin();
}

void onMqttConnect(bool sessionPresent)
{
    Serial.printf("Connected to MQTT Session present: %d\n", sessionPresent);
    mqttClient.subscribe(cg.mqtt.subscribe.c_str(), 0);
    // mqttClient.publish(cg.mqtt.publish.c_str(), 0, false, "{}");
    // dup flag -> duplicate message; retain flag -> tell mqtt broker to save message for future user
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("Disconnected from MQTT");
    CONN_SIGN.MQTT = true;
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
    String output;
    StaticJsonDocument<64> doc;
    // not a log message
    doc["log"] = false;
    doc["mac"] = cg.mqtt.macddr;
    serializeJson(doc, output);
    // registering device to server mqtt subscriber
    mqttClient.publish(cg.mqtt.publish.c_str(), 0, false, output.c_str());

    Serial.printf("Subscribe acknowledged packetId: %d qos: %d\n", packetId, qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
    Serial.printf("Unsubscribe acknowledged packetId: %d\n", packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    // Serial.printf("topic: %s qos: %d dup: %d\n", topic, properties.qos, properties.dup);
    // Serial.printf("retain: %d len: %d index: %d total %d\n", properties.retain, len, index, total);

    if (String(topic) == cg.mqtt.subscribe)
    {
        // Serial.println(payload);
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
        {
            Serial.print(F("[E]deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }

        String msg;
        if (doc["mac"] == cg.mqtt.macddr && doc["init"].is<bool>())
        {
            if (doc["init"] == false)
            {
                if (doc["tof1"].is<unsigned int>() && doc["tof2"].is<unsigned int>() && doc["ms"].is<unsigned int>())
                {
                    if (doc["tof1"] <= cg.alarm.tofMax)
                        cg.alarm.tof1 = doc["tof1"];

                    if (doc["tof2"] <= cg.alarm.tofMax)
                        cg.alarm.tof2 = doc["tof2"];

                    if (doc["ms"] <= cg.alarm.msMax)
                        cg.alarm.ms = doc["ms"];

                    writeConfigFile();

                    doc["status"] = true;
                }
                else
                {
                    // modify failed
                    doc["status"] = false;
                }
            }

            /* doc["init"] = true
            response to get config request
            ["status"] key is not needed */
            doc["tof1"] = cg.alarm.tof1;
            doc["tof2"] = cg.alarm.tof2;
            doc["ms"] = cg.alarm.ms;

            serializeJson(doc, msg);
            // Serial.println(msg);
            mqttClient.publish(cg.mqtt.publish.c_str(), 0, false, msg.c_str());
        }
    }
}

void onMqttPublish(uint16_t packetId)
{
    Serial.printf("Publish acknowledged packetId: %d\n", packetId);
}

void set_mqtt()
{
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.setServer(cg.mqtt.server.c_str(), cg.mqtt.port);
    mqttClient.setCredentials(cg.mqtt.user.c_str(), cg.mqtt.pwd.c_str());
    mqttClient.setKeepAlive(30);
    if (WiFi.isConnected())
        mqttClient.connect();
    else
        CONN_SIGN.MQTT = true;
}

void set_checkTimer(uint8_t timerNum)
{
    timer = timerBegin(timerNum, 80, true);
    timerAttachInterrupt(timer, &onTimerOut, true);
    /* autoreload = true
    value in microseconds so 10,000,000 = 10s */
    timerAlarmWrite(timer, CONN_SIGN.checkMicroSec, true);
    timerAlarmEnable(timer);
}
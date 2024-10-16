#pragma once

#include <Adafruit_NeoPixel.h>
#include "MatrixManager.h"
#include "Application.h"
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "static_files.h"
#include <Arduino_JSON.h>

#define NUMPIXELS 144
#define PIN D4
#define TPS 30

struct InternalApp
{
    Application *(*createFunction)();
    String name;
};

struct boot_code_result
{
    String emoji;
    uint32_t color;
};

class SystemManager
{
public:
    SystemManager()
    {
    }
    void init()
    {
        // init system
        pixels = new Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
        pixels->begin();
        pixels->setBrightness(150);
        pixels->clear();
        pixels->show();
        Serial.begin(9600);
        mm = new MatrixManager(pixels);
        cm = new ControlManager();
        mm->set_tps(TPS);
        current_application = applications[activeApplication].createFunction();
        current_application->init(mm);

        // generate boot code between 0 255
        boot_code = random(0, 255);
        start_server();
    }
    void loop()
    {
        if ((millis() - frame_timer) > (1000 / 1))
        {
            frame_timer = millis();
            current_application->draw(mm);
            // system_draw();
            pixels->show();
        }

        if ((float)(millis() - game_loop_timer) > 1000.0f / mm->get_current_tps())
        {
            game_loop_timer = millis();
            current_application->game_loop(mm, cm);
        }

        if ((float)(millis() - ws_timer) > 1000)
        {
            ws_timer = millis();
            ws->cleanupClients();
            ws->pingAll();
        }

        

        dnsServer->processNextRequest();
    }
    void register_application(Application *(*app)(), String name)
    {
        applications.push_back({app,
                                name});
    }
    void switch_project(int id)
    {
        if (id >= applications.size())
        {
            Serial.println("Out of range\n");
            return;
        }

        current_application->clean_up(mm);
        delete current_application;
        activeApplication = id;
        this->mm->set_tps(TPS);
        current_application = applications[activeApplication].createFunction();
        current_application->init(mm);
        Serial.println("Switched to " + applications[activeApplication].name);
    }

    void handleWebsocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, unsigned char *data, size_t len)
    {
        switch (type)
        {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            this->handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
        }
    }

private:
    void start_server()
    {
        IPAddress APIP(172, 0, 0, 1); // Gateway

        WiFi.hostname("matrix");

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

        // Setting currentSSID -> SSID in EEPROM or default one.

        WiFi.softAP(build_ssid());
        webServer = new AsyncWebServer(80);
        dnsServer = new DNSServer();
        ws = new AsyncWebSocket("/ws");

        ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, unsigned char *data, size_t len) {
            this->handleWebsocketEvent(server,client,type,arg,data,len);
        });
        webServer->addHandler(ws);

        // Start webserver
        dnsServer->start(53, "*", APIP); // DNS spoofing (Only for HTTP)

        webServer->onNotFound([](AsyncWebServerRequest *request)
                              {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", static_files::f_index_html_contents, static_files::f_index_html_size);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response); });

        webServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                      {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", static_files::f_index_html_contents, static_files::f_index_html_size);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response); });

        // Create a route handler for each of the build artifacts
        for (int i = 0; i < static_files::num_of_files; i++)
        {
            webServer->on(static_files::files[i].path, HTTP_GET, [i](AsyncWebServerRequest *request)
                          {
      AsyncWebServerResponse *response = request->beginResponse_P(200, static_files::files[i].type, static_files::files[i].contents, static_files::files[i].size);
      response->addHeader("Content-Encoding", "gzip");
      request->send(response); });
        }

        webServer->begin();
    }

    String build_ssid()
    {
        String ssid = "Matrix ";
        ssid += get_boot_code_emoji(boot_code & 0x03).emoji;
        ssid += get_boot_code_emoji((boot_code >> 2) & 0x03).emoji;
        ssid += get_boot_code_emoji((boot_code >> 4) & 0x03).emoji;
        ssid += get_boot_code_emoji((boot_code >> 6) & 0x03).emoji;
        return ssid;
    }

    void system_draw()
    {
        if (WiFi.softAPgetStationNum() == 0)
        {
            mm->set(8, 0, get_boot_code_emoji(boot_code & 0x03).color);
            mm->set(9, 0, get_boot_code_emoji((boot_code >> 2) & 0x03).color);
            mm->set(10, 0, get_boot_code_emoji((boot_code >> 4) & 0x03).color);
            mm->set(11, 0, get_boot_code_emoji((boot_code >> 6) & 0x03).color);
        }
    }

    boot_code_result get_boot_code_emoji(uint8_t boot_code)
    {
        if (boot_code == 0x00)
            return {"🟥", 0xFF0000};
        if (boot_code == 0x01)
            return {"🟩", 0x00FF00};
        if (boot_code == 0x02)
            return {"🟦", 0x0000FF};
        if (boot_code == 0x03)
            return {"🟨", 0xFFFF00};
        return {"", 0};
    }

    void handleWebSocketMessage(void *arg, unsigned char *data, size_t len)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            String message = String((char *)data);
            JSONVar json = JSON.parse(message);
            Serial.println("JSON parsed successfully");
            if (json.hasOwnProperty("command"))
            {
                String command = json["command"];
                if (command == "switch_project")
                {
                    int project = json["project"];
                    switch_project(project);
                }
                else if (command == "action")
                {
                    String action = json["action"];

                    if (action == "up")
                    {
                        this->current_application->on_event(Event::UP);
                    }

                    if (action == "down")
                    {
                        this->current_application->on_event(Event::DOWN);
                    }

                    if (action == "left")
                    {
                        this->current_application->on_event(Event::LEFT);
                    }

                    if (action == "right")
                    {
                        this->current_application->on_event(Event::RIGHT);
                    }

                    if (action == "middle")
                    {
                        this->current_application->on_event(Event::MIDDLE);
                    }

                    if (action == "a")
                    {
                        this->current_application->on_event(Event::A);
                    }

                    if (action == "b")
                    {
                        this->current_application->on_event(Event::B);
                    }

                    if (action == "c")
                    {
                        this->current_application->on_event(Event::C);
                    }
                }

                // notifyClients(sensorReadings);
            }
        }
    }

    Adafruit_NeoPixel *pixels;
    MatrixManager *mm;
    std::vector<InternalApp> applications;
    Application *current_application = nullptr;
    ControlManager *cm = nullptr;
    int activeApplication = 0;
    long long frame_timer = 0;
    long long game_loop_timer = 0;
    long long ws_timer = 0;

    uint8_t boot_code = -1;
    std::vector<String> boot_code_emoji_translation = {"🟥", "🟩", "🟦", "🟨"};
    AsyncWebServer *webServer;
    DNSServer *dnsServer;
    AsyncWebSocket *ws;
};
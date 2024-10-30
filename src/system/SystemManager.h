#pragma once
#define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
#define ELEGANTOTA_DEBUG 1
#define UPDATE_DEBUG 1
#include <Adafruit_NeoPixel.h>
#include "MatrixManager.h"
#include "Application.h"
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "static_files.h"
#include <Arduino_JSON.h>
#include <ElegantOTA.h>
#include <ws2812_i2s.h>

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
/**
 * Controls the system and schedules the application.
 */
class SystemManager
{
public:
    SystemManager()
    {
    }
    void init()
    {

        Serial.begin(9600);
        // init system
        pixels = new Pixel_t[144];
        ledstrip = new WS2812();
        ledstrip->init(144);
        ledstrip->show(pixels, 2.0f);

        mm = new MatrixManager(pixels);
        cm = new ControlManager([this]()
                                {
            if((millis()-this->last_ws_update) > 500) {
                this->send_ws_update();
            } });
        mm->set_tps(TPS);
        current_application = applications[activeApplication].createFunction();
        current_application->init(mm, cm);

        for (uint64_t i = 0; i < applications.size(); i++)
        {
            this->json_apps[i] = applications[i].name;
        }

        // generate boot code between 0 255
        randomSeed(ESP.getCycleCount());
        boot_code = random(0, 255);
        start_server();
    }
    void loop()
    {

        yield();

        dnsServer->processNextRequest();
        ElegantOTA.loop();

        if ((millis() - frame_timer) > (1000 / 50))
        {
            frame_timer = millis();
            if (!ota_update)
            {
                current_application->draw(mm, cm);
                if (cm->is_animation_running())
                {
                    long long start = cm->__internal_get_animation_start();
                    float duration = cm->__internal_get_animation_duration();
                    long long time_running = millis() - start;

                    bool result = cm->__internal_get_animation()->run(((time_running) / duration), mm);

                    if (result && ((time_running) / (duration + cm->__interal_get_animation_keep_time())) > 1)
                    {
                        delete cm->__internal_get_animation();
                        cm->__internal_set_animation(nullptr);
                    }
                }
            }
            system_draw();
            ledstrip->show(pixels, 2.0f);
        }
        if (ota_update)
            return;

        if ((millis() - game_loop_timer) > (1000 / mm->get_current_tps()))
        {
            game_loop_timer = millis();
            current_application->game_loop(mm, cm);
        }

        if ((float)(millis() - ws_timer) > 300)
        {
            ws_timer = millis();
            ws->cleanupClients();
            if ((millis() - last_ws_update) > 300)
            {
                send_ws_update();
            }

            // Serial.println(ESP.getFreeHeap());
        }

        yield();
    }
    /**
     * Register an application to be visible
     * to the user in the drop down menu.
     *
     * @param app A references to a function which instantiates the application
     * @param name The name of the application
     * @param author The author of the application
     */
    void register_application(Application *(*app)(), String name, String author)
    {
        applications.push_back({app,
                                String(name + " by " + author)});
    }

    /**
     * Programmatically switch to a different application.
     * @param id The index of the application to switch to
     */
    void switch_project(int id)
    {
        if ((unsigned)id >= applications.size())
        {
            Serial.println("Out of range\n");
            return;
        }

        current_application->clean_up(mm);
        delete current_application;
        cm->reset();
        activeApplication = id;
        this->mm->set_tps(TPS);
        cm->__internal_set_animation(nullptr);
        mm->clear();
        current_application = applications[activeApplication].createFunction();
        current_application->init(mm, cm);
        Serial.println("Switched to " + applications[activeApplication].name);
        send_ws_update();
    }

    void handleWebsocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, unsigned char *data, size_t len)
    {
        switch (type)
        {
        case WS_EVT_CONNECT:
            if (client->id() > 5)
            {
                client->close(1000, "Too many clients");
                return;
            }

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

    bool ota_update = false;
    bool ota_error = false;
    float ota_progress = 0;

private:
    void send_ws_update()
    {
        last_ws_update = millis();
        JSONVar package;
        package["status"] = cm->get_status();
        package["controls"] = cm->get_controls();
        package["apps"] = this->json_apps;
        package["active_id"] = this->activeApplication;

        if (ws->count() > 0)
        {
            ws->textAll(JSON.stringify(package));
        }
    }
    void start_server()
    {
        IPAddress APIP(172, 0, 0, 1); // Gateway

        WiFi.hostname("matrix");

        //  WiFi.mode(WIFI_AP);
        delay(100);
        WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

        WiFi.scanNetworks();

        int best_channel_id = 3;
        for (int i = 0; i < 14; i++)
        {
            if (WiFi.channel(i) == 0)
            {
                best_channel_id = i;
                break;
            }
        }

        WiFi.softAP(build_ssid(), "", best_channel_id, 0, 4, 100);
        WiFi.setSleepMode(WIFI_NONE_SLEEP);
        WiFi.printDiag(Serial);

        webServer = new AsyncWebServer(80);
        dnsServer = new DNSServer();
        ws = new AsyncWebSocket("/ws");

        ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, unsigned char *data, size_t len)
                    { this->handleWebsocketEvent(server, client, type, arg, data, len); });
        webServer->addHandler(ws);

        ElegantOTA.begin(webServer);
        ElegantOTA.onStart([this]()
                           {
                               Serial.println("OTA update started!");
                               this->ota_update = true;
                               this->ota_error = false;
                               this->ota_progress = 0; 
                               this->mm->fill(0xFFFF00); });
        ElegantOTA.onProgress([this](size_t current, size_t final)
                              { this->ota_progress = (float)current / (float) final; });
        ElegantOTA.onEnd([this](bool success)
                         {
            if(success) {
                Serial.println("OTA update finished!");
                this->ota_error = false;
                this->ota_progress = 1;
            } else {
                Serial.println("OTA update failed!");
                this->ota_update = false;
                this->ota_error = true;
                this->ota_progress = 0;
            } });

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
        ssid += " ID: #" + String(random(100, 999));
        return ssid;
    }

    void system_draw()
    {

        if (ota_update)
        {
            mm->clear();
            if (ota_progress > 0)
            {
                mm->line(0, 0, 0, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (1 / 12.0f))
            {
                mm->line(1, 0, 1, 11, MatrixManager::Color(0, 255, 0));
            }

            if (ota_progress > (2 / 12.0f))
            {
                mm->line(2, 0, 2, 11, MatrixManager::Color(0, 255, 0));
            }

            if (ota_progress > (3 / 12.0f))
            {
                mm->line(3, 0, 3, 11, MatrixManager::Color(0, 255, 0));
            }

            if (ota_progress > (4 / 12.0f))
            {
                mm->line(4, 0, 4, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (5 / 12.0f))
            {
                mm->line(5, 0, 5, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (6 / 12.0f))
            {
                mm->line(6, 0, 6, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (7 / 12.0f))
            {
                mm->line(7, 0, 7, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (8 / 12.0f))
            {
                mm->line(8, 0, 8, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (9 / 12.0f))
            {
                mm->line(9, 0, 9, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (10 / 12.0f))
            {
                mm->line(10, 0, 10, 11, MatrixManager::Color(0, 255, 0));
            }
            if (ota_progress > (11 / 12.0f))
            {
                mm->line(11, 0, 11, 11, MatrixManager::Color(0, 255, 0));
            }

            mm->number(1, 4, floor(ota_progress * 100.0f), MatrixManager::Color(255, 0, 0));

            return;
        }

        if (WiFi.softAPgetStationNum() == 0 || ws->count() == 0)
        {
            if (!is_code_hidden)
            {
                mm->set(8, 0, get_boot_code_emoji(boot_code & 0x03).color);
                mm->set(9, 0, get_boot_code_emoji((boot_code >> 2) & 0x03).color);
                mm->set(10, 0, get_boot_code_emoji((boot_code >> 4) & 0x03).color);
                mm->set(11, 0, get_boot_code_emoji((boot_code >> 6) & 0x03).color);

                if ((millis() - hide_connnect_code_timer) > (1000 * 30))
                {
                    hide_connnect_code_timer = millis();
                    is_code_hidden = true;
                }
            }
            else
            {
                if ((millis() - hide_connnect_code_timer) > (1000 * 4 * 60))
                {
                    hide_connnect_code_timer = millis();
                    is_code_hidden = false;
                }
            }
        }
        else
        {
            is_code_hidden = false;
            hide_connnect_code_timer = millis();
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
            if (json.hasOwnProperty("command"))
            {
                String command = json["command"];
                if (command == "switch_project")
                {
                    int project = atoi(json["project"]);
                    Serial.println(message);
                    switch_project(project);
                }
                else if (command == "action")
                {
                    String action = json["action"];

                    if (action == "up")
                    {
                        this->current_application->on_event(Event::UP, mm, cm);
                    }

                    if (action == "down")
                    {
                        this->current_application->on_event(Event::DOWN, mm, cm);
                    }

                    if (action == "left")
                    {
                        this->current_application->on_event(Event::LEFT, mm, cm);
                    }

                    if (action == "right")
                    {
                        this->current_application->on_event(Event::RIGHT, mm, cm);
                    }

                    if (action == "middle")
                    {
                        this->current_application->on_event(Event::MIDDLE, mm, cm);
                    }

                    if (action == "a")
                    {
                        this->current_application->on_event(Event::A, mm, cm);
                    }

                    if (action == "b")
                    {
                        this->current_application->on_event(Event::B, mm, cm);
                    }

                    if (action == "c")
                    {
                        this->current_application->on_event(Event::C, mm, cm);
                    }
                }

                // notifyClients(sensorReadings);
            }
        }
    }

    MatrixManager *mm;
    std::vector<InternalApp> applications;
    Application *current_application = nullptr;
    ControlManager *cm = nullptr;
    int activeApplication = 0;
    long long frame_timer = 0;
    long long game_loop_timer = 0;
    long long ws_timer = 0;
    long long last_ws_update = 0;
    long long hide_connnect_code_timer = 0;
    bool is_code_hidden = false;

    uint8_t boot_code = -1;
    std::vector<String> boot_code_emoji_translation = {"🟥", "🟩", "🟦", "🟨"};
    AsyncWebServer *webServer;
    DNSServer *dnsServer;
    AsyncWebSocket *ws;
    JSONVar json_apps;
    // pixel buffer
    Pixel_t *pixels;
    int brightness = 100;
    WS2812 *ledstrip;
};
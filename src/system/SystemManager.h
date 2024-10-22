#pragma once
#define DEBUG_ESP_WIFI
#define DEBUG_WIFI
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
        // init system
        pixels = new Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
        pixels->begin();
        pixels->setBrightness(100);
        pixels->clear();
        pixels->show();
        Serial.begin(9600);
        mm = new MatrixManager(pixels);
        cm = new ControlManager([this](){
            if((millis()-this->last_ws_update) > 500) {
                this->send_ws_update();
            }
        });
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

        
        if ((millis() - frame_timer) > (1000 / 30))
        {
            frame_timer = millis();
            current_application->draw(mm,cm);
            if(cm->is_animation_running()) {
                long long start = cm->__internal_get_animation_start();
                float duration = cm->__internal_get_animation_duration();
                long long time_running = millis()-start;
            
                bool result = cm->__internal_get_animation()->run(((time_running)/duration),mm);
                
                if(result && ((time_running)/(duration+cm->__interal_get_animation_keep_time())) > 1) {
                    delete cm->__internal_get_animation();
                    cm->__internal_set_animation(nullptr);
                }
            }
            system_draw();
            pixels->show();
        }

        if ((millis() - game_loop_timer) > (1000 / mm->get_current_tps()))
        {
            game_loop_timer = millis();
            current_application->game_loop(mm, cm);
        }

        if ((float)(millis() - ws_timer) > 700)
        {
            ws_timer = millis();
            ws->cleanupClients();
            if((millis() - last_ws_update) > 700) {
                send_ws_update();
            }

           // Serial.println(ESP.getFreeHeap());
        }

        yield();

        dnsServer->processNextRequest();

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
                                String(name+" by "+author)});
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
    void send_ws_update()
    {
        last_ws_update = millis();
        JSONVar package;
        package["status"] = cm->get_status();
        package["controls"] = cm->get_controls();
        package["apps"] = this->json_apps;
        package["active_id"] = this->activeApplication;

        if(ws->count() > 0) {
            ws->textAll(JSON.stringify(package));
        }

    }
    void start_server()
    {
        IPAddress APIP(172, 0, 0, 1); // Gateway

        WiFi.hostname("matrix");

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

        WiFi.scanNetworks();

        int best_channel_id = 3;
        for(int i=0;i<14;i++) {
            if(WiFi.channel(i) == 0) {
                best_channel_id = i;
                break;
            }
        }

        WiFi.softAP(build_ssid(),"", best_channel_id,0,4,100);
        WiFi.setSleepMode(WIFI_NONE_SLEEP);
        WiFi.printDiag(Serial);

        webServer = new AsyncWebServer(80);
        dnsServer = new DNSServer();
        ws = new AsyncWebSocket("/ws");

        ws->onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, unsigned char *data, size_t len)
                    { this->handleWebsocketEvent(server, client, type, arg, data, len); });
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
        ssid += " ID: #"+String(random(100,999));
        return ssid;
    }

    void system_draw()
    {
        if (WiFi.softAPgetStationNum() == 0 || ws->count() == 0)
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
                        this->current_application->on_event(Event::UP,mm,cm);
                    }

                    if (action == "down")
                    {
                        this->current_application->on_event(Event::DOWN,mm,cm);
                    }

                    if (action == "left")
                    {
                        this->current_application->on_event(Event::LEFT,mm,cm);
                    }

                    if (action == "right")
                    {
                        this->current_application->on_event(Event::RIGHT,mm,cm);
                    }

                    if (action == "middle")
                    {
                        this->current_application->on_event(Event::MIDDLE,mm,cm);
                    }

                    if (action == "a")
                    {
                        this->current_application->on_event(Event::A,mm,cm);
                    }

                    if (action == "b")
                    {
                        this->current_application->on_event(Event::B,mm,cm);
                    }

                    if (action == "c")
                    {
                        this->current_application->on_event(Event::C,mm,cm);
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
    long long last_ws_update = 0;

    uint8_t boot_code = -1;
    std::vector<String> boot_code_emoji_translation = {"🟥", "🟩", "🟦", "🟨"};
    AsyncWebServer *webServer;
    DNSServer *dnsServer;
    AsyncWebSocket *ws;
    JSONVar json_apps;
};
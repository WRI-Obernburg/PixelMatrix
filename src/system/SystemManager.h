#pragma once

#define UPDATE_DEBUG 1
#include "MatrixManager.h"
#include "Application.h"
#include <DNSServer.h>
#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include "static_files.h"
#include <Arduino_JSON.h>
#include <ElegantOTA.h>
#include <ws2812_i2s.h>
#define WRENCH_COMPACT
#include "WrenchWrapper.h"

#include "wrench/wrench.h"
#include <ESPAsyncWebServer.h>
#include <animations/Splash.h>

#define NUMPIXELS 144
#define PIN D4
#define TPS 30


struct InternalApp
{
    Application*(*createFunction)();
    String name;
    bool is_wrench = false;
    const unsigned char* wrench_code = nullptr;
    int wrench_code_size = 0;
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
    void init()
    {
        Serial.begin(9600);
        // init system
        pixels = new Pixel_t[144];
        ledstrip = new WS2812();
        pinMode(D8, INPUT_PULLDOWN_16);
        pinMode(D7, OUTPUT);
        digitalWrite(D7, HIGH);
        delay(100);
        const bool inverse = digitalRead(D8) == HIGH;
        Serial.println(digitalRead((D8)));
        ledstrip->init(144);
        ledstrip->show(pixels, 2.0f);

        mm = new MatrixManager(pixels, ledstrip, inverse);
        cm = new ControlManager([this]()
        {
            if ((millis() - this->last_ws_update) > 500)
            {
                this->send_ws_update();
            }
        });
        ce = {
            cm,
            mm
        };

        mm->set_tps(TPS);

        current_internal_app = &applications[activeApplication];
        if(current_internal_app->is_wrench)
        {
            initWrench();
        }else
        {
            current_application = applications[activeApplication].createFunction(); //TODO make sure to respect wrench code
            current_application->init(mm, cm);
        }


        for (int i = 0; i < applications.size(); i++)
        {
            this->json_apps[i] = applications[i].name;
        }

        // generate boot code between 0 255
        randomSeed(EspClass::getCycleCount());
        boot_code = random(0, 255);
        start_server();
    }

    void loop()
    {
        dnsServer->processNextRequest();


        yield();

        ElegantOTA.loop();

        if ((millis() - frame_timer) > (1000 / 50))
        {
            frame_timer = millis();
            if (!ota_update)
            {
                if (current_internal_app->is_wrench)
                {
                    if (!transmitting_wrench)
                    {
                        wr_callFunction(wc, "draw");
                    }
                    if (wr_getLastError(w) != 0)
                    {
                        Serial.print(F("WREN Error when drawing: "));
                        Serial.println(wr_getLastError(w));
                    }
                }
                else
                {
                    current_application->draw(mm, cm);
                }
                if (cm->is_animation_running())
                {
                    long long start = cm->__internal_get_animation_start();
                    float duration = cm->__internal_get_animation_duration();
                    long long time_running = millis() - start;

                    bool result = cm->__internal_get_animation()->
                                      run((static_cast<float>(time_running) / duration), mm);

                    if (result && (static_cast<float>(time_running) / (duration + cm->
                        __interal_get_animation_keep_time())) > 1)
                    {
                        delete cm->__internal_get_animation();
                        cm->__internal_set_animation(nullptr);
                    }
                }
            }
            system_draw();
            ledstrip->show(pixels, 3.0f);
        }
        if (ota_update)
            return;

        if ((static_cast<float>(millis()) - static_cast<float>(game_loop_timer)) > (1000 / mm->get_current_tps()))
        {
            game_loop_timer = millis();
            if (current_internal_app->is_wrench)
            {
                if (!transmitting_wrench)
                {
                    wr_callFunction(wc, "game_loop");
                }

                if (wr_getLastError(w) != 0)
                {
                    Serial.print(F("WREN Error when game_loop: "));
                    Serial.println(wr_getLastError(w));
                }
            }
            else
            {
                current_application->game_loop(mm, cm);
            }
        }

        if ((float)(millis() - ws_timer) > 300)
        {
            ws_timer = millis();
            ws->cleanupClients();
            if ((millis() - last_ws_update) > 300)
            {
                send_ws_update();
            }

            Serial.println(EspClass::getFreeHeap());
            // MDNS.update();
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
    void register_application(Application*(*app)(), const String& name, const String& author)
    {
        applications.push_back({
            app,
            String(name + " by " + author),
            false,
            nullptr,
            0
        });
    }

    void register_wrench(const unsigned char* code, int code_length, const String& name, const String& author)
    {
        applications.push_back({
            nullptr,
            String(name + " by " + author),
            true,
            code,
            code_length
        });
    }

    /**
     * Programmatically switch to a different application.
     * @param id The index of the application to switch to
     */
    void switch_project(int id)
    {
        if ((unsigned)id >= applications.size())
        {
            Serial.println(F("Out of range\n"));
            return;
        }


        if (w != nullptr)
        {
            Serial.println(F("WRENCH WAS ACTIVE"));
            wr_destroyState(w);
            w = nullptr;
        }


        if (current_internal_app->is_wrench)
        {
            Serial.println(F("WRENCH WAS ACTIVE"));
            if (!devMode)
                delete[] this->wrench_code;
        }
        else
        {
            current_application->clean_up(mm);
            delete current_application;
            current_internal_app = nullptr;
        }


        cm->reset();
        activeApplication = id;
        this->mm->set_tps(TPS);
        cm->__internal_set_animation(nullptr);
        mm->clear();

        current_internal_app = devMode ? this->devApp : &applications[activeApplication];
        if (current_internal_app->is_wrench)
        {
            initWrench();
        }
        else
        {
            current_application = applications[activeApplication].createFunction();
            current_application->init(mm, cm);
        }
        Serial.println(F("Switched to ") + applications[activeApplication].name);
        send_ws_update();
    }

    void initWrench()
    {
        Serial.println(F("WRENCH ACTIVE"));
        w = wr_newState(); // create the state
        wrench_wrapper::register_wrench_functions(w, &ce);
        wr_loadMathLib(w);
        wr_loadStringLib(w);
        wr_loadContainerLib(w);


        Serial.println(F("try to execute"));

        //output current_internal_app->wrench_code in hex to the console
        //   for (int i = 0; i < 83; i++)
        //      Serial.printf("%02X ", current_internal_app->wrench_code[i]);

        if (!devMode)
        {
            this->wrench_code = new unsigned char[current_internal_app->wrench_code_size];
            memcpy_P(this->wrench_code, current_internal_app->wrench_code, current_internal_app->wrench_code_size);
        }

        wc = wr_run(w, this->wrench_code, current_internal_app->wrench_code_size); // load and run the code!
        wr_setAllocatedMemoryGCHint(w, 1000);

        //print wr_getLastError(w);
        Serial.println(wr_getLastError(w));
        // clean up
        Serial.println(F("try to execute function init"));

        WRValue* result = wr_callFunction(wc, "init");
        if (!result)
        {
            Serial.println(F("Error calling function"));
        }
    }

    void handleWebsocketEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg,
                              unsigned char* data, size_t len)
    {
        switch (type)
        {
        case WS_EVT_CONNECT:
            Serial.printf("ws #%u con %s\n", client->id(),
                          client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("ws #%u dis\n", client->id());
            break;
        case WS_EVT_DATA:
            this->handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
            break;
        case WS_EVT_ERROR:
            break;
        default: break;
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
        package["active_id"] = devMode ? -1 : this->activeApplication;

        if (ws->count() > 0)
        {
            ws->textAll(JSON.stringify(package));
        }
    }

    void start_server()
    {
        IPAddress APIP(192, 168, 0, 1); // Gateway

        WiFi.persistent(false);


        //  WiFi.mode(WIFI_AP);

        wifi_station_set_hostname("matrix.local");

        WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
        WiFi.hostname(F("matrix.local"));

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

        // MDNS.begin(F("matrix"));

        webServer = new AsyncWebServer(80);
        dnsServer = new DNSServer();
        ws = new AsyncWebSocket(F("/ws"));

        ws->onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg,
                           unsigned char* data, size_t len)
        {
            this->handleWebsocketEvent(server, client, type, arg, data, len);
        });
        webServer->addHandler(ws);

        ElegantOTA.begin(webServer);
        ElegantOTA.onStart([this]()
        {
            Serial.println("OTA update started!");
            this->ota_update = true;
            this->ota_error = false;
            this->ota_progress = 0;
            this->mm->fill(0xFFFF00);
        });
        ElegantOTA.onProgress([this](size_t current, size_t final)
        {
            this->ota_progress = (float)current / (float)final;
        });
        ElegantOTA.onEnd([this](bool success)
        {
            if (success)
            {
                Serial.println(F("OTA update finished!"));
                this->ota_error = false;
                this->ota_progress = 1;
            }
            else
            {
                Serial.println(F("OTA update failed!"));
                this->ota_update = false;
                this->ota_error = true;
                this->ota_progress = 0;
            }
        });


        // Start webserver
        dnsServer->start(53, "*", APIP); // DNS spoofing (Only for HTTP)


        webServer->onNotFound([](AsyncWebServerRequest* request)
        {
            AsyncWebServerResponse* response = request->beginResponse_P(
                200, F("text/html"), static_files::f_index_html_contents, static_files::f_index_html_size);
            response->addHeader(F("Content-Encoding"), F("gzip"));
            request->send(response);
        });

        webServer->on("/", HTTP_GET, [](AsyncWebServerRequest* request)
        {
            AsyncWebServerResponse* response = request->beginResponse_P(
                200, F("text/html"), static_files::f_index_html_contents, static_files::f_index_html_size);
            response->addHeader(F("Content-Encoding"), F("gzip"));
            request->send(response);
        });

        webServer->on("/api", HTTP_GET, [this](AsyncWebServerRequest* request)
        {
            JSONVar package;
            package["version"] = "1.1.1";
            package["freeHeap"] = EspClass::getFreeHeap();
            package["bootCode"] = boot_code;
            package["ssid"] = WiFi.SSID();
            package["ip"] = WiFi.localIP().toString();
            package["mac"] = WiFi.macAddress();
            package["currentApp"] = current_internal_app->name;
            package["currentBoardFrq"] = EspClass::getCpuFreqMHz();
            AsyncWebServerResponse* response = request->beginResponse(200, F("application/json"),
                                                                      JSON.stringify(package));

            response->addHeader(F("Access-Control-Allow-Private-Network"), "true");
            response->addHeader(F("Access-Control-Allow-Origin"), "*");
            response->addHeader(F("Private-Network-Access-Name"), "LED Matrix");

            request->send(response);
        });

        webServer->on("/api", HTTP_OPTIONS, [this](AsyncWebServerRequest* request)
        {
            AsyncWebServerResponse* response = request->beginResponse(204);
            response->removeHeader("content-type");

            response->addHeader(F("Access-Control-Allow-Private-Network"), "true");
            response->addHeader(F("Access-Control-Allow-Origin"), "*");
            response->addHeader(F("Private-Network-Access-Name"), "LED Matrix");
            response->addHeader("Access-Control-Expose-Headers", "*");
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Credentials", "true");
            request->send(response);
        });

        webServer->on("/ota/upload", HTTP_OPTIONS, [this](AsyncWebServerRequest* request)
        {
            AsyncWebServerResponse* response = request->beginResponse(204);
            response->removeHeader("content-type");

            response->addHeader(F("Access-Control-Allow-Private-Network"), "true");
            response->addHeader(F("Access-Control-Allow-Origin"), "*");
            response->addHeader(F("Private-Network-Access-Name"), "LED Matrix");
            response->addHeader("Access-Control-Expose-Headers", "*");
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Credentials", "true");
            request->send(response);
        });


        webServer->on("/ota/start", HTTP_OPTIONS, [this](AsyncWebServerRequest* request)
        {
            AsyncWebServerResponse* response = request->beginResponse(204);
            response->removeHeader("content-type");

            response->addHeader(F("Access-Control-Allow-Private-Network"), "true");
            response->addHeader(F("Access-Control-Allow-Origin"), "*");
            response->addHeader(F("Private-Network-Access-Name"), "LED Matrix");
            response->addHeader("Access-Control-Expose-Headers", "*");
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Credentials", "true");
            request->send(response);
        });

        webServer->on("/pushDevCode", HTTP_OPTIONS, [this](AsyncWebServerRequest* request)
        {
            AsyncWebServerResponse* response = request->beginResponse(204);
            response->removeHeader("content-type");

            response->addHeader(F("Access-Control-Allow-Private-Network"), "true");
            response->addHeader(F("Access-Control-Allow-Origin"), "*");
            response->addHeader(F("Private-Network-Access-Name"), "LED Matrix");
            response->addHeader("Access-Control-Expose-Headers", "*");
            response->addHeader("Access-Control-Allow-Headers", "*");
            response->addHeader("Access-Control-Allow-Credentials", "true");
            request->send(response);
        });

        //push wrench code to the device
        webServer->on("/pushDevCode", HTTP_POST, [this](AsyncWebServerRequest* request)
                      {
                          AsyncWebServerResponse* response = request->beginResponse(
                              204, F("application/json"), F("{\"success\":true}"));

                          response->addHeader(F("Access-Control-Allow-Private-Network"), "true");
                          response->addHeader(F("Access-Control-Allow-Origin"), "*");
                          response->addHeader(F("Private-Network-Access-Name"), "LED Matrix");
                          request->send(response);
                      }, nullptr, [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index,
                                         size_t total)
                      {
                          Serial.printf("Index: %d, Total: %d\n, on %s", index, total, request->url().c_str());
                          if (request->url() == "/pushDevCode")
                          {
                              if (index == 0)
                              {
                                  devMode = true;
                                  transmitting_wrench = true;

                                  if (wrench_code != nullptr)
                                  {
                                      delete[] wrench_code;
                                  }

                                  wrench_code = new unsigned char[total];
                                  current_internal_app = this->devApp;
                                  current_internal_app->wrench_code_size = total;
                                  Serial.println("Code upload started\n");
                              }


                              Serial.printf("Received %d bytes\n", len);

                              memcpy(wrench_code + index, data, len);

                              if ((index + len) == total)
                              {
                                  Serial.println("Code upload finished");
                                  switch_project(applications.size() - 1);
                                  transmitting_wrench = false;
                              }
                          }
                      });

        // Create a route handler for each of the build artifacts
        for (int i = 0; i < static_files::num_of_files; i++)
        {
            webServer->on(static_files::files[i].path, HTTP_GET, [i](AsyncWebServerRequest* request)
            {
                AsyncWebServerResponse* response = request->beginResponse_P(
                    200, static_files::files[i].type, static_files::files[i].contents, static_files::files[i].size);
                response->addHeader(F("Content-Encoding"), F("gzip"));
                request->send(response);
            });
        }


        webServer->begin();
    }

    [[nodiscard]] String build_ssid() const
    {
        String ssid = F("Matrix ");
        ssid += get_boot_code_emoji(boot_code & 0x03).emoji;
        ssid += get_boot_code_emoji((boot_code >> 2) & 0x03).emoji;
        ssid += get_boot_code_emoji((boot_code >> 4) & 0x03).emoji;
        ssid += get_boot_code_emoji((boot_code >> 6) & 0x03).emoji;
        ssid += F(" ID: #") + String(random(100, 999));
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

                if ((millis() - hide_connect_code_timer) > (1000 * 30))
                {
                    hide_connect_code_timer = millis();
                    is_code_hidden = true;
                }
            }
            else
            {
                if ((millis() - hide_connect_code_timer) > (1000 * 4 * 60))
                {
                    hide_connect_code_timer = millis();
                    is_code_hidden = false;
                }
            }
        }
        else
        {
            is_code_hidden = false;
            hide_connect_code_timer = millis();
        }

    }

    static boot_code_result get_boot_code_emoji(uint8_t boot_code)
    {
        if (boot_code == 0x00)
            return {F("🟥"), 0xFF0000};
        if (boot_code == 0x01)
            return {F("🟩"), 0x00FF00};
        if (boot_code == 0x02)
            return {F("🟦"), 0x0000FF};
        if (boot_code == 0x03)
            return {F("🟨"), 0xFFFF00};
        return {"", 0};
    }

    void handleWebSocketMessage(void* arg, unsigned char* data, size_t len)
    {
        auto* info = static_cast<AwsFrameInfo*>(arg);
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            String message = String((char*)data);
            JSONVar json = JSON.parse(message);
            if (json.hasOwnProperty(F("command")))
            {
                String command = json["command"];
                if (command == F("switch_project"))
                {
                    int project = atoi(json["project"]);
                    Serial.println(message);
                    devMode = false;
                    switch_project(project);
                }
                else if (command == F("action"))
                {
                    String action = json["action"];

                    if (action == F("up"))
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::UP);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::UP, mm, cm);
                        }
                    }

                    if (action == F("down"))
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::DOWN);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::DOWN, mm, cm);
                        }
                    }

                    if (action == F("left"))
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::LEFT);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::LEFT, mm, cm);
                        }
                    }

                    if (action == F("right"))
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::RIGHT);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::RIGHT, mm, cm);
                        }
                    }

                    if (action == F("middle"))
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::MIDDLE);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::MIDDLE, mm, cm);
                        }
                    }

                    if (action == "a")
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::A);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::A, mm, cm);
                        }
                    }

                    if (action == "b")
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::B);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::B, mm, cm);
                        }
                    }

                    if (action == "c")
                    {
                        if (this->current_internal_app->is_wrench)
                        {
                            WRValue val;
                            wr_makeInt(&val, Event::C);
                            wr_callFunction(wc, "on_event", &val, 1);
                        }
                        else
                        {
                            this->current_application->on_event(Event::C, mm, cm);
                        }
                    }
                }
            }
        }
    }

    MatrixManager* mm = nullptr;
    std::vector<InternalApp> applications;
    Application* current_application = nullptr;
    InternalApp* current_internal_app = nullptr;
    InternalApp* devApp = new InternalApp{
        nullptr, "Dev", true, nullptr, 0
    };
    ControlManager* cm = nullptr;
    ControlElements ce = {};
    int activeApplication = 0;
    long long frame_timer = 0;
    long long game_loop_timer = 0;
    long long ws_timer = 0;
    long long last_ws_update = 0;
    long long hide_connect_code_timer = 0;
    bool is_code_hidden = false;

    uint8_t boot_code = -1;
    AsyncWebServer* webServer = nullptr;
    DNSServer* dnsServer = nullptr;
    AsyncWebSocket* ws = nullptr;
    JSONVar json_apps;
    uint8_t* wrench_code = nullptr;

    // pixel buffer
    Pixel_t* pixels = nullptr;
    int brightness = 100;
    WS2812* ledstrip = nullptr;
    WRState* w = nullptr;
    WRContext* wc = nullptr;
    int outLen = 0;
    bool devMode = false;
    bool transmitting_wrench = false;
};

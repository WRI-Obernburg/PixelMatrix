#include <Arduino.h>
#include "system/SystemManager.h"
#include "applications/Tim/ChessAnimation.h"
#include "applications/Tim/HelloRobo.h"
#include "applications/Tim/RacingGame.h"
#include "applications/Tim/Rainbow.h"
#include "applications/Tim/Snake.h"
#include "applications/Tim/CircleAnimation.h"
#include "applications/Tim/Pumpkin.h"
#include "applications/Totem/Totem.h"
#include "applications/PixelRunner.h"
#include "applications/clock.h"

SystemManager sm;
#define DEBUG_WIFI

void setup()
{

  sm.register_application(ChessAnimation::create, "Chess Animation", "Tim");
  sm.register_application(HelloRobo::create, "HelloRobo", "Tim");
  sm.register_application(RacingGame::create, "SuperRacer", "Tim");
  sm.register_application(Snake::create, "Snake", "Tim");
  sm.register_application(Rainbow::create, "Rainbow", "Tim");
  sm.register_application(CircleAnimation::create, "Circle Animation", "Tim");
  sm.register_application(Pumpkin::create, "Pumpkin", "Tim");
  sm.register_application(Totem::create, "Totem", "Insert name here");
  sm.register_application(PixelRunner::create, "PixelRunner", "Pixel Runners");
  sm.register_application(clock::create, "Clock", "Anton");
  sm.init();

  sm.switch_project(5);
  Serial.println(ESP.getResetInfo());
}

void loop()
{
  sm.loop();
  delay(1);
}

#include <Arduino.h>
#include "system/SystemManager.h"
#include "applications/Tim/ChessAnimation.h"
#include "applications/Tim/HelloRobo.h"
#include "applications/Tim/RacingGame.h"
#include "applications/Tim/Rainbow.h"


SystemManager sm;
#define DEBUG_WIFI

void setup()
{
  sm.register_application(ChessAnimation::create, "Chess Animation","Tim");
  sm.register_application(HelloRobo::create, "HelloRobo","Tim");
  sm.register_application(RacingGame::create, "SuperRacer","Tim");
  sm.register_application(Rainbow::create, "Rainbow", "Tim");
  sm.init();

  sm.switch_project(0);
  Serial.println(ESP.getResetInfo());

}


void loop()
{
  sm.loop();
}

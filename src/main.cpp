#include <Arduino.h>
#include "system/SystemManager.h"
#include "applications/ChessAnimation.h"
#include "applications/Staircase.h"
#include "applications/RacingGame.h"
#include "applications/Rainbow.h"


SystemManager sm;
#define DEBUG_WIFI

void setup()
{
  sm.register_application(ChessAnimation::create, "Chess Animation","Tim");
  sm.register_application(Staircase::create, "HelloRobo","Tim");
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

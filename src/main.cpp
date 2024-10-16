#include <Arduino.h>
#include "system/SystemManager.h"
#include "applications/ChessAnimation.h"
#include "applications/Staircase.h"
#include "applications/RacingGame.h"
#include "applications/Rainbow.h"


SystemManager sm;

void setup()
{
  sm.register_application(ChessAnimation::create, "ChessAnimation");
  sm.register_application(Staircase::create, "Staircase");
  sm.register_application(RacingGame::create, "RacingGame");
  sm.register_application(Rainbow::create, "Rainbow");
  sm.init();

  sm.switch_project(1);
  
}


void loop()
{
  sm.loop();
}

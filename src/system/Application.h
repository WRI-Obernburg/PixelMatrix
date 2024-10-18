#pragma once
#include "MatrixManager.h"
#include "ControlManager.h"
#include "Event.h"
class Application {
    public:
        Application() {
            
        }

        virtual ~Application() {};

        virtual void init(MatrixManager * mm, ControlManager * cm) = 0;
        virtual void draw(MatrixManager * mm, ControlManager *cm) = 0;
        virtual void game_loop(MatrixManager * mm, ControlManager * cm) = 0;
        virtual void clean_up(MatrixManager * mm) = 0;
        virtual void on_event(Event e,MatrixManager * mm, ControlManager * cm) = 0;
    
    private:

};
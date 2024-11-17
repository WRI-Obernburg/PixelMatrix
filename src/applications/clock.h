#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>
// INCLUDE ANIMATIONS HERE (e.g. #include <animations/Splash.h>)

class clock : public Application
{ // CHANGE NAME HERE
public:
    clock()
    { // CHANGE NAME HERE
    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {
        cm->set_controls(button_left | button_right|button_up);

        mm->set_tps(1);
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        t+=1;
        t=t%(12*60*60);
        cm->set_status(String(t));
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {
        mm->clear();
        float min=(t/60)%60;
        float h=(t/60/60)%60;
        mm->line(6,6,int(sin(min*PI/30)*5+6),int(cos(min*PI/30)*5+6),MatrixManager::Color(255,0,0)); 
        mm->line(6,6,int(sin(h*PI/6)*4+6),int(cos(h*PI/6)*4+6),MatrixManager::Color(0,0,255)); 

    }

    void clean_up(MatrixManager *mm) override
    {
    }
    void on_event(Event e, MatrixManager *mm, ControlManager *cm)
    {
        if (e == Event::LEFT)
        {
            t -= 10;
        }

        if (e == Event::UP)
        {
            t += 1000;
        }
        if (e == Event::DOWN)
        {
            t += 10000;
        }

        if (e == Event::RIGHT)
        {
            t += 10;
        }
        cm->set_status(String(t));
    }

    static Application *create()
    {
        return new clock(); // CHANGE NAME HERE
    }

private:
    uint16 t = 0;
    // ADD YOUR VARIABLES HERE
};
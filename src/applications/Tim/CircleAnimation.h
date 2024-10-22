#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>
#include <animations/Splash.h>

class CircleAnimation : public Application
{ 
public:
    CircleAnimation()
    { 
    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        if (cm->is_animation_running())
            return;
        cm->run_animation(new Splash(6, 6, MatrixManager::Color(random(0,255), random(0,255), random(0,255))), 1000, 0);
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {
    }

    void clean_up(MatrixManager *mm) override
    {
    }
    void on_event(Event e, MatrixManager *mm, ControlManager *cm)
    {
    }

    static Application *create()
    {
        return new CircleAnimation(); 
    }

private:
};
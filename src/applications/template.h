#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>
// INCLUDE ANIMATIONS HERE (e.g. #include <animations/Splash.h>)

class Template : public Application
{ // CHANGE NAME HERE
public:
    Template()
    { // CHANGE NAME HERE

    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {

    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {

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
        return new Template(); // CHANGE NAME HERE
    }

private:
    // ADD YOUR VARIABLES HERE
};
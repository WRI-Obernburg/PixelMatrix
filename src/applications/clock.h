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
        cm->set_controls(button_a);

        mm->set_tps(1);
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        t += 1;
        t = t % (12 * 60 * 60);
        cm->set_status(String(t));
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {

        mm->clear();
        if (!digital)
        {
            float min = (t / 60) % 60;
            float h = (t / 60 / 60) % 60;

            mm->line(round(sin(min * PI / 6) * 0.5 + 5.5), round(cos(min * PI / 6) * 0.5 + 5.5), round(sin(min * PI / 30) * 5 + 5.5), round(cos(min * PI / 30) * 5 + 5.5), MatrixManager::Color(255, 0, 0));
            mm->line(round(sin(h * PI / 6) * 0.5 + 5.5), round(cos(h * PI / 6) * 0.5 + 5.5), round(sin(h * PI / 6) * 4 + 5.5), round(cos(h * PI / 6) * 4 + 5.5), MatrixManager::Color(0, 0, 255));
        }
        else
        {
            int min = (t / 60) % 60;
            int h = (t / 60 / 60) % 60;
            mm->digit(0,6, (h-h%10)/10, MatrixManager::Color(0,0,255));
            mm->digit(4,6, h%10, MatrixManager::Color(0,0,255));
            mm->digit(5,0, (min-min%10)/10, MatrixManager::Color(255,0,0));
            mm->digit(9,0, min%10, MatrixManager::Color(255,0,0));
        }
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
        if (e == Event::A)
        {
            digital = !digital;
        }

        cm->set_status(String(t));
    }

    static Application *create()
    {
        return new clock(); // CHANGE NAME HERE
    }

private:
    bool digital = false;
    uint16 t = 0;
    // bool nums[10][5][3] = {
    //     {
    //         {true, true, true},
    //         {true, false, true},
    //         {true, false, true},
    //         {true, false, true},
    //         {true, true, true},
    //     },
    //     {
    //         {false, true, false},
    //         {false, true, false},
    //         {false, true, false},
    //         {false, true, false},
    //         {false, true, false},
    //     },
    //     {
    //         {true, true, true},
    //         {false, false, true},
    //         {true, true, true},
    //         {true, false, false},
    //         {true, true, true},

    //     },
    //     {{true, true, true},
    //      {false, false, true},
    //      {true, true, true},
    //      {false, false, true},
    //      {true, true, true}},
    //     {{true, false, true},
    //      {true, false, true},
    //      {true, true, true},
    //      {false, false, true},
    //      {false, false, true}},
    //     {{true, true, true},
    //      {true, false, false},
    //      {true, true, true},
    //      {false, false, true},
    //      {true, true, true}},
    //     {{true, true, true},
    //      {true, false, false},
    //      {true, true, true},
    //      {true, false, true},
    //      {true, true, true}},
    //     {{true, true, true},
    //      {false, false, true},
    //      {false, false, true},
    //      {false, true, false},
    //      {false, true, false}},
    //     {
    //         {true, true, true},
    //         {true, false, true},
    //         {true, true, true},
    //         {true, false, true},
    //         {true, true, true},
    //     },
    //     {{true, true, true},
    //      {true, false, true},
    //      {true, true, true},
    //      {false, false, true},
    //      {true, true, true}},
    // };
};
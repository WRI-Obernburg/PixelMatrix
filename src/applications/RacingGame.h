/*
 * App: RacingGame
 * Steure ein Fahrzeug durch eine Straße, die immer schnellere
 * zufällige Bewegungen macht
 * Autor: Tim Arnold
 */

#include <system/MatrixManager.h>
#include <system/Application.h>
#include "../animations/Splash.h"

class RacingGame : public Application
{
public:
    RacingGame()
    {
    }
    void init(MatrixManager *mm, ControlManager * cm) override
    {
        mm->set_tps(10);
        reset(mm);
        cm->set_controls(button_left | button_right);
    }
    int loop_counter = 0;
    int points = 0;
    bool loose_state = false;
    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        if(loose_state) return;
        points++;
        if (cm->is_animation_running())
            return;
        mm->set_tps(mm->get_current_tps() + 0.013f / (mm->get_current_tps() / 10));
        cm->set_status("Deine Punkte: "+String(points));
        randomSeed(ESP.getCycleCount());
        if (keep_state < 0 && random(0, 10) > 4)
        {
            if (random(0, 9) > 4)
            {
                if (ref_point > 0)
                {
                    ref_point--;
                    keep_state = 7;
                }
            }
            else
            {
                if (ref_point < 6)
                {
                    ref_point++;
                    keep_state = 7;
                }
            }
        }

        keep_state--;

        for (int y = 0; y < 12; y++)
        {
            for (int x = 0; x < 12; x++)
            {
                roadpieces[x][y] = roadpieces[x][y + 1];
            }
        }

        // remove old road on top of the game field
        for (int i = 0; i < 12; i++)
        {
            roadpieces[i][12] = false;
        }

        roadpieces[ref_point][12] = true;
        int distance = points > 300 ? 4:5;
        roadpieces[ref_point + distance][12] = true;

        // scrolling animation
        if (loop_counter >= 3)
        {
            loop_counter = 0;
        }

        for (int i = 0; i < 12; i++)
        {
            this->is_on[i] = false;
            if ((i + loop_counter) % 3 == 0)
            {
                this->is_on[i] = true;
            }
        }
        loop_counter++;

        if (roadpieces[car_x][car_y] || roadpieces[car_x][car_y + 1])
        {
            // loose
           loose_state = true;
           cm->set_controls(button_a);
           Animation * newAnimation = new Splash(car_x,car_y,MatrixManager::Color(255,0,0),mm);
           cm->run_animation(newAnimation,1000,1000);
        }
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {

        if(cm->is_animation_running()) return; 

        if(loose_state) {
            mm->clear();
        

            mm->number(1,4,this->points,MatrixManager::Color(255,0,0));
            return;
        }

        for (int y = 0; y < 12; y++)
        {
            for (int x = 0; x < 12; x++)
            {
                if (this->roadpieces[x][y])
                {
                    if (this->is_on[y])
                    {
                        mm->set(x, y, MatrixManager::Color(0, 255, 0));
                    }
                    else
                    {
                        mm->set(x, y, MatrixManager::Color(255, 255, 255));
                    }
                }
                else
                {
                    mm->off(x, y);
                }
            }
        }

        mm->set(car_x, car_y, MatrixManager::Color(255, 0, 0));
        mm->set(car_x, car_y + 1, MatrixManager::Color(255, 0, 0));
    }

    void clean_up(MatrixManager *mm) override
    {
    }

    void on_event(Event e,MatrixManager * mm, ControlManager * cm)
    {
        if(e==Event::LEFT) {
            if(this->car_x > 0)
            this->car_x--;
        }

        if(e==Event::RIGHT) {
            if(this->car_x < 11)
            this->car_x++;
        }

        if(e==Event::A) {
            loose_state = false;
            reset(mm);
            cm->set_controls(button_left | button_right);

        }
    }

    void reset(MatrixManager * mm)
    {

        car_x = 5;
        car_y = 2;
        ref_point = 4;
        keep_state = 2;
        loop_counter = 0;

        for (int x = 0; x < 12; x++)
        {
            for (int y = 0; y < 13; y++)
            {
                roadpieces[x][y] = y == 12 && (x <= ref_point || x >= ref_point + 5); // inital bounds
            }
        }

        for (int i = 0; i < 12; i++)
        {
            is_on[i] = false;
        }
        mm->set_tps(10);
        points=0;
    }

    static Application *create()
    {
        return new RacingGame();
    }

private:
    bool roadpieces[12][13];
    int row_x[12];
    bool is_on[12]; // scrolling animation
    int car_x = 6, car_y = 2;
    int ref_point = 4;
    int keep_state = 2; // amount of straight road pieces
};
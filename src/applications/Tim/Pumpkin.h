#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>
// INCLUDE ANIMATIONS HERE (e.g. #include <animations/Splash.h>)

class Pumpkin : public Application
{ // CHANGE NAME HERE
public:
    Pumpkin()
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
        mm->clear();
        mm->rect(1,1,9,8,MatrixManager::Color(255,140,0),true);
        //unten links
        mm->set(1,1,MatrixManager::Color(0,0,0));
        mm->set(2,1,MatrixManager::Color(0,0,0));
        mm->set(1,2,MatrixManager::Color(0,0,0));

        //links oben
        mm->set(1,7,MatrixManager::Color(0,0,0));
        mm->set(1,8,MatrixManager::Color(0,0,0));
        mm->set(2,8,MatrixManager::Color(0,0,0));

        //rechts oben
        mm->set(9,7,MatrixManager::Color(0,0,0));
        mm->set(9,8,MatrixManager::Color(0,0,0));
        mm->set(8,8,MatrixManager::Color(0,0,0));

        //rechts unten
        mm->set(9,1,MatrixManager::Color(0,0,0));
        mm->set(8,1,MatrixManager::Color(0,0,0));
        mm->set(9,2,MatrixManager::Color(0,0,0));

        //stiel
        mm->set(5,9,MatrixManager::Color(0,255,0));
        mm->set(5,10,MatrixManager::Color(0,255,0));
        mm->set(6,10,MatrixManager::Color(0,255,0));

        mm->set(4,2,0);
        mm->set(5,2,0);
        mm->set(6,2,0);
        mm->set(3,3,0);
        mm->set(7,3,0);
        mm->set(5,4,0);

        mm->set(2,5,0);
        mm->set(3,5,0);
        mm->set(3,6,0);
        mm->set(7,5,0);
        mm->set(8,5,0);
        mm->set(7,6,0);


       
    }

    void clean_up(MatrixManager *mm) override
    {

    }
    void on_event(Event e, MatrixManager *mm, ControlManager *cm)
    {

    }

    static Application *create()
    {
        return new Pumpkin(); // CHANGE NAME HERE
    }

private:
    // ADD YOUR VARIABLES HERE
};
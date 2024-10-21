#include <system/MatrixManager.h>
#include <system/Application.h>
#include <animations/Splash.h>

class UseAnimation : public Application
{ // CHANGE NAME HERE
public:
    UseAnimation()
    { // CHANGE NAME HERE

    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {

    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        if(cm->is_animation_running()) return;
        cm->run_animation(new Splash(5,5 ,MatrixManager::Color(255,0,0)),1000,1000);
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
        return new UseAnimation(); // CHANGE NAME HERE
    }

private:
    // ADD YOUR VARIABLES HERE
};
#include <system/MatrixManager.h>
#include <system/Application.h>

class UseControlAPI : public Application
{ // CHANGE NAME HERE
public:
    UseControlAPI()
    { // CHANGE NAME HERE

    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {
        cm->set_controls(button_left | button_right | button_up | button_down);
        cm->set_status("Hello World");
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
        return new UseControlAPI(); // CHANGE NAME HERE
    }

private:
    // ADD YOUR VARIABLES HERE
};
#include <system/MatrixManager.h>
#include <system/Application.h>

class UseMatrixManager : public Application
{ // CHANGE NAME HERE
public:
    UseMatrixManager()
    { // CHANGE NAME HERE

    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {
        mm->set_tps(3);
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {
        mm->clear();
        mm->set(5,5,MatrixManager::Color(255,0,0));
        mm->line(0,0,11,11,MatrixManager::Color(0,255,0));
        mm->circle(5,5,5,MatrixManager::Color(0,0,255));
        mm->rect(0,0,11,11,MatrixManager::Color(255,255,0));
        mm->number(1,1,123,MatrixManager::Color(0,255,255));
        mm->off(5,5);
        mm->fill(MatrixManager::Color(255,0,255));
    }

    void clean_up(MatrixManager *mm) override
    {

    }
    void on_event(Event e, MatrixManager *mm, ControlManager *cm)
    {

    }

    static Application *create()
    {
        return new UseMatrixManager(); // CHANGE NAME HERE
    }

private:
    // ADD YOUR VARIABLES HERE
};
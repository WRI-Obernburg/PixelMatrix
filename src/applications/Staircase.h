#include <system/MatrixManager.h>
#include <system/Application.h>

class Staircase : public Application
{
public:
    Staircase()
    {
    }
    void init(MatrixManager *mm) override
    {
        Serial.println("Hello world");
    }
    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
    }

    void draw(MatrixManager *mm)
    {
        /*   mm->set(0, 0, MatrixManager::Color(131, 187, 27));
           mm->set(1, 1, MatrixManager::Color(131, 187, 27));
           mm->set(2, 2, MatrixManager::Color(131, 187, 27));
           mm->set(3, 3, MatrixManager::Color(131, 187, 27));
           mm->set(4, 4, MatrixManager::Color(131, 187, 27));
           mm->set(5, 5, MatrixManager::Color(131, 187, 27));
           mm->set(6, 6, MatrixManager::Color(131, 187, 27));
           mm->set(7, 7, MatrixManager::Color(131, 187, 27));
           mm->set(8, 8, MatrixManager::Color(131, 187, 27));
           mm->set(9, 9, MatrixManager::Color(131, 187, 27));
           mm->set(10, 10, MatrixManager::Color(131, 187, 27));
           mm->set(11, 11, MatrixManager::Color(131, 187, 27));
           */
        // mm->clear();
        // mm->circle(6,6,5,MatrixManager::Color(131, 187, 27));
        // mm->rect(2,6,3,3,MatrixManager::Color(131, 187, 27), true);
        mm->set(3, 0, MatrixManager::Color(131, 187, 27));
        mm->set(4, 0, MatrixManager::Color(131, 187, 27));
        mm->set(7, 0, MatrixManager::Color(131, 187, 27));
        mm->set(8, 0, MatrixManager::Color(131, 187, 27));

         mm->set(4, 1, MatrixManager::Color(131, 187, 27));
        mm->set(7, 1, MatrixManager::Color(131, 187, 27));
         mm->set(4, 2, MatrixManager::Color(131, 187, 27));
        mm->set(7, 2, MatrixManager::Color(131, 187, 27));

         mm->set(4, 3, MatrixManager::Color(131, 187, 27));
        mm->set(7, 3, MatrixManager::Color(131, 187, 27));

         mm->set(4, 4, MatrixManager::Color(131, 187, 27));
        mm->set(7, 4, MatrixManager::Color(131, 187, 27));

         mm->set(4, 5, MatrixManager::Color(131, 187, 27));
        mm->set(7, 5, MatrixManager::Color(131, 187, 27));

         mm->set(4, 6, MatrixManager::Color(131, 187, 27));
        mm->set(7, 6, MatrixManager::Color(131, 187, 27));

         mm->set(4, 7, MatrixManager::Color(131, 187, 27));
        mm->set(7, 7, MatrixManager::Color(131, 187, 27));

         mm->set(5, 3, MatrixManager::Color(131, 187, 27));
        mm->set(6, 3, MatrixManager::Color(131, 187, 27));

         mm->set(5, 7, MatrixManager::Color(131, 187, 27));
        mm->set(6, 7, MatrixManager::Color(131, 187, 27));

         mm->set(5, 8, MatrixManager::Color(131, 187, 27));
        mm->set(6, 8, MatrixManager::Color(131, 187, 27));

         mm->set(5, 9, MatrixManager::Color(131, 187, 27));
        mm->set(6, 9, MatrixManager::Color(131, 187, 27));

        mm->set(3,6,MatrixManager::Color(131, 187, 27));
        mm->set(2,6,MatrixManager::Color(131, 187, 27));
        mm->set(2,5,MatrixManager::Color(131, 187, 27));

        mm->set(8,6,MatrixManager::Color(131, 187, 27));
        mm->set(9,6,MatrixManager::Color(131, 187, 27));
        mm->set(9,7,MatrixManager::Color(131, 187, 27));

        mm->rect(4,3,4,4,MatrixManager::Color(131, 187, 27), true);


    }

    void clean_up(MatrixManager *mm) override
    {
    }
    void on_event(Event e)
    {
    }

    static Application *create()
    {
        return new Staircase();
    }
};
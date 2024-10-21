
#include <system/MatrixManager.h>
#include <system/Application.h>
class ChessAnimation : public Application
{
public:
    ChessAnimation()
    {
    }

    void init(MatrixManager *mm, ControlManager * cm)
    {
        mm->set_tps(2);
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {
        for (int i = 0; i < 144; i++)
        {

            if (i % 2 == 0)
            {
                if (toggle)
                {
                    mm->set_string(i, mm->Color(131, 187, 27));
                }
                else
                {
                    mm->set_string(i, mm->Color(103, 59, 183));
                }
            }
            else
            {
                if (toggle)
                {
                    mm->set_string(i, mm->Color(103, 59, 183));
                }
                else
                {
                    mm->set_string(i, mm->Color(131, 187, 27));
                }
            }
        }
    }

    void game_loop(MatrixManager *mm, ControlManager *cm)
    {

        this->toggle = !this->toggle;
    }

    void clean_up(MatrixManager *mm)
    {
    }

    void on_event(Event e,MatrixManager * mm, ControlManager * cm)
    {
    }

    static Application *create()
    {
        return new ChessAnimation();
    }

private:
    bool toggle = false;
};
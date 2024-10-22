#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>
#include <animations/Splash.h>
#include <vector>

struct pos
{
    int x;
    int y;
};

class Snake : public Application
{
public:
    Snake()
    {
    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {
        mm->set_tps(2);
        reset(cm);
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {

        if (cm->is_animation_running() || loose_state)
            return;

        if (direction == 0)
        {
            snake_pieces.push_back({snake_pieces.back().x,
                                    snake_pieces.back().y + 1});
        }

        if (direction == 1)
        {
            snake_pieces.push_back({snake_pieces.back().x + 1,
                                    snake_pieces.back().y});
        }

        if (direction == 2)
        {
            snake_pieces.push_back({snake_pieces.back().x,
                                    snake_pieces.back().y - 1});
        }

        if (direction == 3)
        {
            snake_pieces.push_back({snake_pieces.back().x - 1,
                                    snake_pieces.back().y});
        }

        // iterate over the queue and check if the most recent element collides with an older one
        for (int i = 0; i < snake_pieces.size() - 1; i++)
        {
            if (snake_pieces.at(i).x == snake_pieces.back().x && snake_pieces.at(i).y == snake_pieces.back().y)
            {
                loose_state = true;
                cm->set_controls(button_a);
                cm->run_animation(new Splash(snake_pieces.back().x, snake_pieces.back().y, MatrixManager::Color(255, 0, 0)), 1000, 1000);
                return;
            }
        }

        // check if the new element is outside the screen
        if (snake_pieces.back().x < 0 || snake_pieces.back().x > 11 || snake_pieces.back().y < 0 || snake_pieces.back().y > 11)
        {
            loose_state = true;
            cm->set_controls(button_a);
            cm->run_animation(new Splash(snake_pieces.back().x, snake_pieces.back().y, MatrixManager::Color(255, 0, 0)), 1000, 1000);
            return;
        }

        bool food_collision = (snake_pieces.back().x == food_position.x) && (snake_pieces.back().y == food_position.y);

        if (direction != -1 && !food_collision)
        {
            snake_pieces.erase(snake_pieces.begin());
        }

        if (food_collision)
        {
            food_position.x = random(0, 11);
            food_position.y = random(0, 11);
        }
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {
        if (cm->is_animation_running())
            return;
        mm->clear();

        if (loose_state)
        {
            mm->number(1, 4, this->snake_pieces.size() - 1, MatrixManager::Color(255, 0, 0));
            return;
        }

        for (int i = 0; i < snake_pieces.size(); i++)
        {
            mm->set(snake_pieces.at(i).x, snake_pieces.at(i).y, MatrixManager::Color(0, 255, 0), true);
        }

        mm->set(food_position.x, food_position.y, MatrixManager::Color(255, 0, 0));
    }

    void clean_up(MatrixManager *mm) override
    {
    }
    void on_event(Event e, MatrixManager *mm, ControlManager *cm)
    {
        if (e == Event::UP)
        {
            direction = 0;
        }

        if (e == Event::RIGHT)
        {
            direction = 1;
        }

        if (e == Event::DOWN)
        {
            direction = 2;
        }

        if (e == Event::LEFT)
        {
            direction = 3;
        }

        if(e == Event::A) {
            reset(cm);
        }
    }

    static Application *create()
    {
        return new Snake();
    }

    void reset(ControlManager *cm)
    {
        snake_pieces.clear();
        snake_pieces.push_back({1, 5});

        snake_pieces.push_back({2, 5});

        snake_pieces.push_back({3, 5});

        food_position = {
            random(0, 11),
            random(0, 11)};
        cm->set_controls(button_up | button_right | button_down | button_left);
        loose_state = false;
        direction = -1;
    }

private:
    int direction = -1; // 0 up 1 right 2 down 3 left
    std::vector<pos> snake_pieces;
    bool loose_state = false;
    pos food_position;
};
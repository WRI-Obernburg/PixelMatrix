#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>
// INCLUDE ANIMATIONS HERE (e.g. #include <animations/Splash.h>)
#define LEVEL_COUNT 7

class Totem : public Application
{ // CHANGE NAME HERE
public:
    Totem()
    { // CHANGE NAME HERE
    }
    void init(MatrixManager *mm, ControlManager *cm) override
    {
        cm->set_controls(button_up | button_down | button_left | button_right);
    }

    void game_loop(MatrixManager *mm, ControlManager *cm) override
    {
        if (cm->is_animation_running())
            return;
        if (map[lvlcount][11 - player_y][player_x] == 3)
        {

            if ((LEVEL_COUNT) > (lvlcount + 1))
            {
                lvlcount += 1;
                reset(mm);
            }
            else
            {
                final_level_done = true;
            }
        }

        if (map[lvlcount][11 - player_y][player_x] == 6)
        {
            toggle = true;
        }

        if (map[lvlcount][11 - player_y][player_x] == 7 && display_spikes)
        {
            loose(mm,cm);
        }


        if ((millis() - spike_timer) > 900)
        {
            spike_timer = millis();
            display_spikes = !display_spikes;
        }
    }

    void reset(MatrixManager *mm)
    {
        player_x = 1;
        player_y = 1;
        toggle = false;
    }

    void loose(MatrixManager *mm, ControlManager *cm)
    {
        cm->run_animation(new Splash(player_x, player_y, MatrixManager::Color(250, 0, 125)), 1000, 1000);
        reset(mm);
    }

    void draw(MatrixManager *mm, ControlManager *cm)
    {
        if(cm->is_animation_running()) return;

        if (final_level_done)
        {
            mm->fill(0x00FF00);
            return;
        }
        for (size_t x = 0; x < 12; x++)
        {
            for (size_t y = 0; y < 12; y++)
            {
                if (map[lvlcount][11 - y][x] == 0)
                {
                    mm->off(x, y);
                }
                else if (map[lvlcount][11 - y][x] == 1)
                {
                    mm->set(x, y, MatrixManager::Color(125, 125, 125));
                }
                else if (map[lvlcount][11 - y][x] == 2)
                {
                    // mm->set(x, y, MatrixManager::Color(0, 250, 125));
                    mm->off(x, y);
                }
                else if (map[lvlcount][11 - y][x] == 3)
                {
                    mm->set(x, y, MatrixManager::Color(0, 250, 125));
                }
                else if (map[lvlcount][11 - y][x] == 4 && toggle == false)
                {
                    mm->set(x, y, MatrixManager::Color(125, 125, 125));
                }
                else if (map[lvlcount][11 - y][x] == 4 && toggle == true)
                {
                    mm->off(x, y);
                }
                else if (map[lvlcount][11 - y][x] == 5 && toggle == false)
                {
                    mm->off(x, y);
                }
                else if (map[lvlcount][11 - y][x] == 5 && toggle == true)
                {
                    mm->set(x, y, MatrixManager::Color(125, 125, 125));
                }
                else if (map[lvlcount][11 - y][x] == 6 && toggle == false)
                {
                    mm->set(x, y, MatrixManager::Color(250, 250, 0));
                }
                else if (map[lvlcount][11 - y][x] == 6 && toggle == true)
                {
                    mm->set(x, y, MatrixManager::Color(125, 125, 0));
                }
                else if (map[lvlcount][11 - y][x] == 7 && display_spikes)
                {
                    mm->set(x, y, MatrixManager::Color(250, 0, 0));
                }
                else if (map[lvlcount][11 - y][x] == 7 && !display_spikes)
                {
                    mm->off(x, y);
                }
            }
        }
        mm->set(player_x, player_y, MatrixManager::Color(250, 0, 125));
    }

    void clean_up(MatrixManager *mm) override
    {
    }
    void on_event(Event e, MatrixManager *mm, ControlManager *cm)
    {
        if(cm->is_animation_running()) return;

        if (e == Event::RIGHT)
        {
            while (true)
            {
                if (map[lvlcount][11 - player_y][player_x + 1] == 1)
                    break;
                if (map[lvlcount][11 - player_y][player_x + 1] == 4 && toggle == false)
                    break;
                if (map[lvlcount][11 - player_y][player_x + 1] == 5 && toggle == true)
                    break;
                if (map[lvlcount][11 - player_y][player_x + 1] == 7 && display_spikes)
                {
                    loose(mm, cm);
                    break;
                }
                player_x += 1;
            }
        }
        if (e == Event::LEFT)
        {
            while (true)
            {
                if (map[lvlcount][11 - player_y][player_x - 1] == 1)
                    break;
                if (map[lvlcount][11 - player_y][player_x - 1] == 4 && toggle == false)
                    break;
                if (map[lvlcount][11 - player_y][player_x - 1] == 5 && toggle == true)
                    break;
                if (map[lvlcount][11 - player_y][player_x - 1] == 7 && display_spikes)
                {
                    loose(mm, cm);
                    break;
                }

                player_x -= 1;
            }
        }
        if (e == Event::UP)
        {
            while (true)
            {
                if (map[lvlcount][11 - player_y - 1][player_x] == 1)
                    break;
                if (map[lvlcount][11 - player_y - 1][player_x] == 4 && toggle == false)
                    break;
                if (map[lvlcount][11 - player_y - 1][player_x] == 5 && toggle == true)
                    break;
                if (map[lvlcount][11 - player_y - 1][player_x] == 7 && display_spikes)
                {
                    loose(mm, cm);
                    break;
                }
                player_y += 1;
            }
        }
        if (e == Event::DOWN)
        {
            while (true)
            {
                if (map[lvlcount][11 - player_y + 1][player_x] == 1)
                    break;
                if (map[lvlcount][11 - player_y + 1][player_x] == 4 && toggle == false)
                    break;
                if (map[lvlcount][11 - player_y + 1][player_x] == 5 && toggle == true)
                    break;
                if (map[lvlcount][11 - player_y + 1][player_x] == 7 && display_spikes)
                {
                    loose(mm, cm);
                    break;
                }
                player_y -= 1;
            }
        }
    }

    static Application *create()
    {
        return new Totem(); // CHANGE NAME HERE
    }

private:
    int map[LEVEL_COUNT][12][12] = {{
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                    },
                                    {
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 3, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 2, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                    },
                                    {
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 3, 1},
                                        {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
                                        {1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1},
                                        {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
                                        {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1},
                                        {1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1},
                                        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1},
                                        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                    },
                                    {
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 5, 1, 0, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 6, 1},
                                        {1, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 1},
                                        {1, 0, 0, 0, 1, 3, 0, 0, 1, 0, 5, 1},
                                        {1, 0, 1, 1, 0, 5, 0, 0, 1, 0, 0, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
                                        {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1},
                                        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                        {1, 2, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
                                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                    },
                                    {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                     {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
                                     {1, 0, 0, 5, 0, 0, 5, 6, 0, 0, 0, 1},
                                     {1, 1, 0, 0, 5, 1, 0, 0, 0, 0, 1, 1},
                                     {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                                     {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 4, 0, 0, 0, 5, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 1, 0, 5, 1, 1, 5, 0, 0, 0, 1},
                                     {1, 2, 0, 0, 0, 4, 3, 1, 0, 0, 0, 1},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}},
                                    {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                     {1, 7, 5, 0, 0, 0, 0, 0, 1, 5, 6, 1},
                                     {1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 1},
                                     {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 0, 0, 0, 1, 1, 1, 1, 0, 7, 0, 1},
                                     {1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 5, 0, 0, 0, 0, 0, 0, 0, 7, 1, 1},
                                     {1, 3, 5, 0, 0, 1, 0, 0, 1, 0, 0, 1},
                                     {1, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                                     {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1},
                                     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
                                     },
                                    {
                                    {1,1,1,1,1,1,1,1,1,1,1,1},
                                    {1,7,7,7,7,1,7,7,7,7,5,1},
                                    {1,1,7,7,7,7,7,4,5,5,7,1},
                                    {1,7,7,5,7,7,5,5,7,7,7,1},
                                    {1,1,5,4,7,7,7,5,7,7,1,1},
                                    {1,5,7,7,7,7,3,7,4,4,1,1},
                                    {1,7,4,7,7,7,5,7,7,5,5,1},
                                    {1,5,4,7,7,7,7,7,7,7,7,1},
                                    {1,7,5,7,7,7,7,7,4,5,1,1},
                                    {1,7,4,5,7,4,7,7,5,7,5,1},
                                    {1,2,1,6,4,7,4,5,4,5,7,1},
                                    {1,1,1,1,1,1,1,1,1,1,1,1} }
    };
    int player_x = 1, player_y = 1;
    int lvlcount = 0;
    bool toggle = false;
    bool final_level_done = false;
    bool display_spikes = false;
    long long spike_timer = 0;
};
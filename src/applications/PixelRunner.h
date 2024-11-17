#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>

class PixelRunner : public Application
{
public:
  PixelRunner() {};

  void clean_up(MatrixManager *mm) override {}

  static Application *create()
  {
    return new PixelRunner();
  }

  void init(MatrixManager *mm, ControlManager *cm) override
  {

    mm->set_tps((float)2);
    state_to_controls();
    cm->set_controls(controls);
  }

  void game_loop(MatrixManager *mm, ControlManager *cm) override
  {
    if (t > 36)
    {
      state = "win";
    }
    if (state == "running")
    {
      t = t + 1;
      if (levels[level][t][py + sy] != 1)
      {
        py = py + sy;
      }
      if (py < 0)
      {
        py = 0;
      }
      sy = sy + -1;
      if (sy <= -1)
      {
        sy = -1;
      }
      if (levels[level][t][py] != 0)
      {
        state = "gameover";
      }
    }
    state_to_controls();
    cm->set_controls(controls);
  }

  void draw(MatrixManager *mm, ControlManager *cm) override
  {

    mm->clear();
    for (x = 0; x <= 11; x = x + 1)
    {
      for (y = 0; y <= 11; y = y + 1)
      {
        if (36 > x + t)
        {
          if (levels[level][x + t][y] == 1)
          {
            mm->set(x, y, MatrixManager::Color(255, 3, 238));
          }
          else if (levels[level][x + t][y] == 2)
          {
            mm->set(x, y, MatrixManager::Color(255, 0, 0));
          }
          else
          {
          }
        }
        else
        {
          if (0 + (y + (x + t)) % 2 == 0)
          {
            mm->set(x, y, MatrixManager::Color(255, 255, 255));
          }
        }
      }
    }
    mm->set(0, py, MatrixManager::Color(0, 255, 0));
  }

  void on_event(Event e, MatrixManager *mm, ControlManager *cm) override
  {
    switch (e)
    {
    case Event::UP:
      up(mm, cm);
      break;
    case Event::DOWN:
      down(mm, cm);
      break;
    case Event::LEFT:
      left(mm, cm);
      break;
    case Event::RIGHT:
      right(mm, cm);
      break;
    case Event::MIDDLE:
      middle(mm, cm);
      break;
    case Event::A:
      aButton(mm, cm);
      break;
    case Event::B:
      bButton(mm, cm);
      break;
    case Event::C:
      cButton(mm, cm);
      break;
    default:
      break;
    }
  }

  void up(MatrixManager *mm, ControlManager *cm)
  {

    if (py == 0)
    {
      sy = 2;
    }
    else
    {
      if (levels[level][t][py - 1] == 1)
      {
        sy = 2;
      }
    }
  }
  void down(MatrixManager *mm, ControlManager *cm)
  {
  }
  void left(MatrixManager *mm, ControlManager *cm)
  {
  }
  void right(MatrixManager *mm, ControlManager *cm)
  {
  }
  void middle(MatrixManager *mm, ControlManager *cm)
  {

    if (py == 0)
    {
      sy = 2;
    }
    else
    {
      if (levels[level][t][py - 1] == 1)
      {
        sy = 2;
      }
    }
  }
  void aButton(MatrixManager *mm, ControlManager *cm)
  {

    if (state == "idle")
    {
      state = "running";
    }
    else if (state == "gameover")
    {
      t = 0;
      state = "idle";
      py = 0;
    }
    else if (state == "win")
    {
      level = level + 1;
      t = 0;
      state = "idle";
      py = 0;
    }
  }
  void bButton(MatrixManager *mm, ControlManager *cm)
  {
  }
  void cButton(MatrixManager *mm, ControlManager *cm)
  {
  }

private:
  int levels[4][36][12] = {

      {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      },
      {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
          {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
          {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}
      },
      {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
       {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
       {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
       {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
  String state = "idle";
  int x;
  int y;
  int controls = 0;
  int t = 0;
  int py = 0;
  int sy = 0;
  int level = 0;

  void state_to_controls()
  {
    controls = 0;
    if (state == "idle")
    {
      controls = controls + 32;
    }
    else if (state == "running")
    {
      controls = controls + 1;
      controls = controls + 4;
    }
    else if (state == "gameover")
    {
      controls = controls + 32;
    }
    else if (state == "win")
    {
      controls = controls + 32;
    }
  }
};
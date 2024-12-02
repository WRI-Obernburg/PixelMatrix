#pragma once
#include <system/MatrixManager.h>
#include <system/Application.h>

class Dev : public Application {
  public:

Dev(){};

void clean_up(MatrixManager *mm) override {}

static Application *create() {
   return new Dev();
}

void init(MatrixManager *mm, ControlManager *cm) override {

}

void game_loop(MatrixManager *mm, ControlManager *cm) override {

}

void draw(MatrixManager *mm, ControlManager *cm) override {

  mm->set(5,5, MatrixManager::Color(255,0,0));

}

void on_event(Event e, MatrixManager *mm, ControlManager *cm) override { switch (e) { case Event::UP: up(mm, cm); break; case Event::DOWN: down(mm, cm); break; case Event::LEFT: left(mm, cm); break; case Event::RIGHT: right(mm, cm); break; case Event::MIDDLE: middle(mm, cm); break; case Event::A: aButton(mm, cm); break; case Event::B: bButton(mm, cm); break; case Event::C: cButton(mm, cm); break; default: break; } }

void up(MatrixManager *mm, ControlManager *cm) {

   y_pos = y_pos + 1;

}
void down(MatrixManager *mm, ControlManager *cm) {

   y_pos = y_pos + -1;

}
void left(MatrixManager *mm, ControlManager *cm) {

   x_pos = x_pos + -1;

}
void right(MatrixManager *mm, ControlManager *cm) {

   x_pos = x_pos + 1;

}
void middle(MatrixManager *mm, ControlManager *cm) {

}
void aButton(MatrixManager *mm, ControlManager *cm) {

}
void bButton(MatrixManager *mm, ControlManager *cm) {

}
void cButton(MatrixManager *mm, ControlManager *cm) {

}

private:
  int x_pos = 2;
  char y_pos = 2;

 void reset() {
}


};
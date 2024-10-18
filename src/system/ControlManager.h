#pragma once
#include "Animation.h"

class ControlManager
{
public:
    ControlManager(std::function<void()> change)
    {
        this->change = change;
    }
    void set_status(String status)
    {
        this->status = status;
        change();
    }

    String get_status()
    {
        return this->status;
    }

    uint8_t get_controls()
    {
        return this->controls;
    }

    void set_controls(uint8_t controls)
    {
        this->controls = controls;
        change();
    }

    void reset() {
        status = "";
        controls = 0x00;
    }

      bool is_animation_running()
    {
        return current_animation!=nullptr;
    }

    void run_animation(Animation* newAnimation, int duration_ms, int keep_time = 0) {
        this->current_animation = newAnimation;
        this->duration_ms = duration_ms;
        this->animation_start_time = millis();
        this->keep_time = keep_time;
    }

    Animation* __internal_get_animation() {
        return this->current_animation;
    }

    void __internal_set_animation(Animation* animation) {
        this->current_animation = animation;
    }

    float __internal_get_animation_duration() {
        return this->duration_ms;
    }


    float __interal_get_animation_keep_time() {
        return this->keep_time;
    }

    long long __internal_get_animation_start() {
        return this->animation_start_time;
    }

private:
    uint8_t controls = 0x00;
    String status = "";
    std::function<void()> change;
       Animation* current_animation = nullptr;
    float duration_ms = 0;
    long long animation_start_time = 0;
    float keep_time = 0;
};

uint8_t button_up = 0b00000001;
uint8_t button_left = 0b00000010;
uint8_t button_middle = 0b00000100;
uint8_t button_right = 0b00001000;
uint8_t button_down = 0b00010000;
uint8_t button_a = 0b00100000;
uint8_t button_b = 0b01000000;
uint8_t button_c = 0b10000000;

uint8_t all_buttons = 0b11111111;

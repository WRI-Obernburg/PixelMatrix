#pragma once
#include "Animation.h"

/**
 * Used to set the current status and controls of your application.
 * It is also used to run animations.
 */
class ControlManager
{
public:
    ControlManager(std::function<void()> change)
    {
        this->change = change;
    }

    /**
     * Send a string containing the current status of your application to the user.
     * It is displayed on the control website. Chaning the status can take up to 1s.
     * @param status String
     */
    void set_status(String status)
    {
        this->status = status;
        change();
    }

    /**
     * Get the current status of your application.
     * @return String
     */
    String get_status()
    {
        return this->status;
    }

    /**
     * Get the current controls of your application.
     * @return uint8_t
     */
    uint8_t get_controls()
    {
        return this->controls;
    }

    /**
     * Set the current controls of your application. Every bit of the uint8_t
     * represents a button. If the bit is set, the button is pressed. @see button_up
     * for an example. Chaning the controls can take up to 1s.
     * @param controls uint8_t
     */

    void set_controls(uint8_t controls)
    {
        this->controls = controls;
        change();
    }

    /**
     * Reset the current controls and status of your application.
     */
    void reset() {
        status = "";
        controls = 0x00;
    }

    /**
     * Check if an animation is running.
     * This can be useful to disable the user interface while an animation is running or to
     * stop your application logic while an animation is running.
     * @return bool
     */

      bool is_animation_running()
    {
        return current_animation!=nullptr;
    }


    /**
     * Run a new animation. The animation will be played for the given duration and
     * will be stopped after the given keep_time. If the keep_time is 0, the animation
     * will be played until it is done. You need to call this function once to run an animation.
     * Each frame will than be handled internally.
     * @param newAnimation Animation
     * @param duration_ms duration in ms
     * @param keep_time duration in ms
     */
    void run_animation(Animation* newAnimation, int duration_ms, int keep_time = 0) {
        this->current_animation = newAnimation;
        this->duration_ms = duration_ms;
        this->animation_start_time = millis();
        this->keep_time = keep_time;
    }

    /**
     * Stop an animation before it is finished. 
     */
    void stop_animation() {
        this->current_animation = nullptr;
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

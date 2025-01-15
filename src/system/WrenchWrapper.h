//
// Created by T.Arnold on 19.12.2024.
//

#ifndef WRENCHWRAPPER_H
#define WRENCHWRAPPER_H
#include <animations/Splash.h>

#include "wrench/wrench.h"
#include "ControlManager.h"

struct ControlElements
{
    ControlManager* cm;
    MatrixManager* mm;
};
namespace wrench_wrapper
{
    inline void print(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        char buf[128];
        for( int i=0; i<argn; ++i )
        {
            printf( "%s\n", argv[i].asString(buf, 128) );
        }
    }

    inline void set_status(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn == 0) return;
        auto* ce = static_cast<ControlElements*>(usr);

        char buf[128];
        String status = "";
        for (int i=0;i<argn;i++) {
            status += argv[i].asString(buf, 128);
        }
        ce->cm->set_status(status);

    }

    inline void get_status(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto* ce = static_cast<ControlElements*>(usr);
        wr_makeString(c, &retVal, ce->cm->get_status().c_str(), strlen(ce->cm->get_status().c_str()));
    }

    inline void get_current_tps(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto* ce = static_cast<ControlElements*>(usr);
        wr_makeFloat(&retVal, ce->mm->get_current_tps());
    }

    inline void set_tps(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 1) return;
        auto* ce = static_cast<ControlElements*>(usr);
        ce->mm->set_tps(argv[0].asFloat());
        wr_makeInt(&retVal, 1);
    }

    inline void get_controls(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto* ce = static_cast<ControlElements*>(usr);
        wr_makeInt(&retVal, ce->cm->get_controls());
    }

    inline void set_controls(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 1) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->cm->set_controls(argv[0].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void reset_controls(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto ce = static_cast<ControlElements*>(usr);
        ce->cm->reset();
        wr_makeInt(&retVal, 1);
    }

    inline void is_animation_running(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto ce = static_cast<ControlElements*>(usr);
        wr_makeInt(&retVal, ce->cm->is_animation_running());

    }

    /*void run_animation(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr) {
        if (argn != 2) return;
        ControlElements* ce = static_cast<ControlElements*>(usr);
        Animation* anim = new Animation(); // Assuming Animation has a default constructor
        ce->cm->run_animation(anim, argv[0].asInt(), argv[1].asInt());
        wr_makeInt(&retVal, 1);
    }
    */


    inline void stop_animation(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto ce = static_cast<ControlElements*>(usr);
        ce->cm->stop_animation();
        wr_makeInt(&retVal, 1);
    }

    inline void set_pixel(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 3) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->set(argv[0].asInt(), argv[1].asInt(), argv[2].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void off_pixel(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 2) return;
        auto* ce = static_cast<ControlElements*>(usr);
        ce->mm->off(argv[0].asInt(), argv[1].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void fill_matrix(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 1) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->fill(argv[0].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void clear_matrix(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->clear();
        wr_makeInt(&retVal, 1);
    }

    inline void draw_line(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 5) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->line(argv[0].asInt(), argv[1].asInt(), argv[2].asInt(), argv[3].asInt(), argv[4].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void draw_rect_filled(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 5) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->rect(argv[0].asInt(), argv[1].asInt(), argv[2].asInt(), argv[3].asInt(), argv[4].asInt(),true); //make fillable controlable
        wr_makeInt(&retVal, 1);
    }

    inline void draw_rect(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 5) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->rect(argv[0].asInt(), argv[1].asInt(), argv[2].asInt(), argv[3].asInt(), argv[4].asInt()); //make fillable controlable
        wr_makeInt(&retVal, 1);
    }

    inline void draw_circle(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 4) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->circle(argv[0].asInt(), argv[1].asInt(), argv[2].asInt(), argv[3].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void draw_number(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 4) return;
        auto ce = static_cast<ControlElements*>(usr);
        ce->mm->number(argv[0].asInt(), argv[1].asInt(), argv[2].asInt(), argv[3].asInt());
        wr_makeInt(&retVal, 1);
    }

    //animations
    inline void run_animation_splash(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 6) return;
        auto ce = static_cast<ControlElements*>(usr);
        auto anim = new Splash(argv[0].asInt(), argv[1].asInt(), argv[2].asInt(), argv[3].asInt() > 0);
        ce->cm->run_animation(anim, argv[4].asInt(), argv[5].asInt());
        wr_makeInt(&retVal, 1);
    }

    inline void wrench_random(WRContext* c, const WRValue* argv, const int argn, WRValue& retVal, void* usr)
    {
        if (argn != 2) return;
        wr_makeInt(&retVal, random(argv[0].asInt(), argv[1].asInt()));
    }

    static void register_wrench_functions(WRState* w, ControlElements* ce)
    {
        wr_registerFunction(w, "print", wrench_wrapper::print, &ce); // bind a function

        wr_registerFunction(w, "set_status", wrench_wrapper::set_status, ce);
        wr_registerFunction(w, "get_status", wrench_wrapper::get_status, ce);
        wr_registerFunction(w, "get_controls", wrench_wrapper::get_controls, ce);
        wr_registerFunction(w, "set_controls", wrench_wrapper::set_controls, ce);
        wr_registerFunction(w, "get_current_tps", wrench_wrapper::get_current_tps, ce);
        wr_registerFunction(w, "set_tps", wrench_wrapper::set_tps, ce);
        wr_registerFunction(w, "reset_controls", wrench_wrapper::reset_controls, ce);
        wr_registerFunction(w, "is_animation_running", wrench_wrapper::is_animation_running, ce);
        // wr_registerFunction(w, "run_animation", wrench_wrapper::run_animation, ce);
        wr_registerFunction(w, "stop_animation", wrench_wrapper::stop_animation, ce);

        wr_registerFunction(w, "set", wrench_wrapper::set_pixel, ce);
        wr_registerFunction(w, "off", wrench_wrapper::off_pixel, ce);
        wr_registerFunction(w, "fill", wrench_wrapper::fill_matrix, ce);
        wr_registerFunction(w, "clear", wrench_wrapper::clear_matrix, ce);
        wr_registerFunction(w, "line", wrench_wrapper::draw_line, ce);
        wr_registerFunction(w, "rect", wrench_wrapper::draw_rect, ce);
        wr_registerFunction(w, "rect_filled", wrench_wrapper::draw_rect_filled, ce);
        wr_registerFunction(w, "circle", wrench_wrapper::draw_circle, ce);
        wr_registerFunction(w, "number", wrench_wrapper::draw_number, ce);

        //animations
        wr_registerFunction(w, "run_animation_splash", wrench_wrapper::run_animation_splash, ce);

        //utils
        wr_registerFunction(w, "random", wrench_wrapper::wrench_random, ce);
    }
}
#endif //WRENCHWRAPPER_H

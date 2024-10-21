#pragma once
#include "MatrixManager.h"
/**
 * Represents an animation that can be run with the MatrixManager
 * Each instance of this class represents one animation which can be run once
 * The matrix manager will call the run function of the animation once it is started
 * by the current application. The run function will be called with a progress value
 * between 0 and 1 which indicates the progress of the animation. It can be the case
 * that the run function will be called multiple times with the same progress value, 
 * if the animation is supposed to keep the current state.
 */
class Animation {
    public:
    virtual ~Animation() {};
    /**
     *  Run the animation and return true if animation is done
     *  @param progress progress of the animation
     *  @param mm MatrixManager
     *  @return true if animation is done
     */
    virtual bool run(float progress, MatrixManager *mm) = 0;
};
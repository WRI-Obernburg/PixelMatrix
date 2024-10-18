#pragma once
#include "MatrixManager.h"
class Animation {
    public:
    virtual ~Animation() {};
    //return true if animation is dones
    virtual bool run(float progress, MatrixManager *mm) = 0;
};
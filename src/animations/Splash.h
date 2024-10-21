#include "../system/Animation.h"
class Splash : public Animation
{
public:
    Splash(int x, int y, uint32_t color, bool filled = true)
    {
        this->x = x;
        this->y = y;
        this->color = color;
        this->filled = filled;

        int max_radius = longestDistanceToBorder(x,y);
        this->max_radius = max_radius;
        // get longest distance to border 0,0 11,11
    }
    bool run(float progress, MatrixManager *mm)
    {
        int currentSteps = max_radius * progress;
        if(!filled) {
            mm->clear();
        }
        mm->circle(x, y, currentSteps, color,false,1);
       // Serial.println(progress);

        return progress > 1;
    }

private:
    int x, y;
    uint32_t color;
    int max_radius = 0;
    bool filled = false;
    int longestDistanceToBorder(int x, int y)
    {
        // Calculate the distances to each border
        int distanceToTop = y;
        int distanceToBottom = 11 - y;
        int distanceToLeft = x;
        int distanceToRight = 11 - x;

        // Return the maximum of these distances
        return std::max({distanceToTop, distanceToBottom, distanceToLeft, distanceToRight});
    }
};
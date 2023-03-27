#include "Definition.h"
Factor factor;
Rocket rocket;
double unitTransform(unsigned presentParameter, double value, bool direction)
{
    if (direction == true)
    {
        if (presentParameter == 0)
            return value / 1e3;
        else if (presentParameter == 1)
            return value / 1e6;
        else if (presentParameter == 2)
            return value;
        else if (presentParameter == 3)
            return value;
        else if (presentParameter == 4)
            return value * 1e3;
        else if (presentParameter == 5)
            return value / 1e6;
        return value * value * PI / 4e6;
    }
    else
    {
        if (presentParameter == 0)
            return value * 1e3;
        else if (presentParameter == 1)
            return value * 1e6;
        else if (presentParameter == 2)
            return value;
        else if (presentParameter == 3)
            return value;
        else if (presentParameter == 4)
            return value / 1e3;
        else if (presentParameter == 5)
            return value * 1e6;
        return sqrt(value * 4e6 / PI);
    }
}
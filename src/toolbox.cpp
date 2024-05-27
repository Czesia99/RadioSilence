#include "toolbox.hpp"

float map_range(float value, float min1, float max1, float min2, float max2)
{
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float clamp(float n, float lower, float upper)
{
    return n <= lower ? lower : n >= upper ? upper : n;
}
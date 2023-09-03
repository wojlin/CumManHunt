# include "../../include/Utils/Math.h"

float radians(float degrees)
{
    return degrees * (PI / 180.0);
}

float degrees(float radians)
{
    return radians * (180.0 / PI);
}

float pointToAngle(std::pair<int, int> vertex, int x, int y)
{
    float deltaX = (float) vertex.first - (float) x;
    float deltaY = (float) vertex.second - (float) y;
    return degrees(atan2(deltaY, deltaX));
}

float norm(float angle)
{   
    angle = fmod(angle, 360.0);

    if(angle < 0)
    {
        return angle + 360;
    }else
    {
        return angle;
    }
}

float calculateDistance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}
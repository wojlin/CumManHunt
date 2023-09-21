# include "../../include/Utils/Math.h"

double radians(double degrees)
{
    return degrees * (PI / 180.0);
}

double degrees(double radians)
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

double calculateDistance(double  x1, double  y1, double  x2, double  y2) {
    double  dx = x1 - x2;
    double  dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}

int modulo(int a, int b)
{
    return (a % b + b) % b;
}

std::string toUpper(std::string name)
{
    for (char &c : name) {
    c = std::toupper(c);
    }
    return name;
}
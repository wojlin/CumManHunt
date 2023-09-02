#ifndef MATH_H
#define MATH_H

#include <utility>
#include <cmath>

const double PI = 3.14159265358979323846;

float radians(float degrees);

float degrees(float radians);

float pointToAngle(std::pair<int, int> vertex, int x, int y);

float norm(float angle);

float calculateDistance(float x1, float y1, float x2, float y2);

#endif
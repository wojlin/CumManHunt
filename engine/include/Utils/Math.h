#ifndef MATH_H
#define MATH_H

#include <utility>
#include <cmath>
#include <iostream>
#include <string>
#include <cctype>

const double PI = 3.14159265358979323846;

double radians(double degrees);

double degrees(double radians);

float pointToAngle(std::pair<int, int> vertex, int x, int y);

float norm(float angle);

double calculateDistance(double  x1, double  y1, double  x2, double  y2);

int modulo(int a, int b);

std::string toUpper(std::string name);

#endif
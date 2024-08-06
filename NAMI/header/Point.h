#pragma once
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;

class Point
{
public:
    Point(double x_p, double y_p, double z_p, long long id_p = numeric_limits<long long>::infinity())
    {
        x = x_p;
        y = y_p;
        z = z_p;
        id = id_p;
    }

    Point(long long id_p)
    {
        id = id_p;
    }
    double distanceTo(Point &p)// определяет расстояние до точки
    {
        return sqrt(pow((x - p.x), 2) + pow((y - p.y), 2) + pow((z - p.z), 2));
    }

    Point getVector(Point &endOfVector)
    {
        return Point(endOfVector.x - x, endOfVector.y - y, endOfVector.z - z);
    }

    static double getAngle(Point vector1, Point vector2)// возвращает угол между векторами
    {
        return atan2(vector2.y, vector2.x) - atan2(vector1.y, vector1.x);
    }

    long long getId()// передаёт ID
    {
        return id;
    }

    double length () // возвращает длину вектора
    {
        return sqrt(x*x + y*y);
    }

public:
    double x{0};
    double y{0};
    double z{0};
private:
    long long id = numeric_limits<long long>::infinity();
};



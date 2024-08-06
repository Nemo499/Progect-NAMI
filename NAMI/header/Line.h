#pragma once
#include "Point.h"
#include <iostream>
#include <vector>
using namespace std;

class Line {
public:
    Line(long long id_p)
    {
        id = id_p;
    }

    Line() = default;

    void addPoint(Point p)
    {
        points.push_back(p);
    }

    Point &getPoint(int index)
    {
        return points[index];
    }

    void removePoint(int index)
    {
        points.erase(points.begin() + index);
    }


    int searchPoint(long long id_p) //ищет номер точки в списке точек линии, если наход точку, с искомым ID возвращает её номер, в проитвном случае возвращает -2
    {
        for(int i = 0; i < points.size(); i++)
        {
            if(points[i].getId() == id_p)
            {
                return i;
            }
        }
        return -1;
    }

    double length()
    {
        double l{0.0};

        if(points.size() < 2)
        {
            return 0.0;
        }

        for(int i = 1; i < points.size(); i++)
        {
            l += points[i].distanceTo(points[i-1]);
        }

        return l;
    }

    Point &front()
    {
        return points.front();
    }

    Point &back()
    {
        return points.back();
    }

    int size()
    {
        return points.size();
    }

    long long getId()// передаёт ID
    {
        return id;
    }
    int sizePrev()
    {
        return prev_lines_id.size();
    }

    int sizeFollow()
    {
        return follow_lines_id.size();
    }

public:
    enum class TYPE { NONE = 0, OSM = 1, HD = 2, LINK = 3 };// почитать про enum и про enum class
public:
    vector<Point> points;
    vector<int> prev_lines_id{};// линии предшественники
    vector<int> follow_lines_id{};// линии последователи
    TYPE type = TYPE::NONE;
private:
    long long id = 0;
};

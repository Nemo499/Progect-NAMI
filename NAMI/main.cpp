#include <iostream>
#include "pugixml.hpp"
#include <GeographicLib/Constants.hpp>
#include "Line.h"
#include "Point.h"
#include "GPSPoint.h"
#include "OsmParser.h"
#include <string>
using namespace std;


int main()
{
    setlocale(0,"");
    OsmParser parser;
    vector<Line> osm_lines, hd_lines;
    vector<Line> lines;
    double maxdist {30};
    double maxAngle {30};
    string pathRead;
    string pathWrite;
    cout << "Введите масимальную дистанцию поиска: ";
    cin >> maxdist;
    cout << endl;
    cout << "Введите масимальный угол поиска: ";
    cin >> maxAngle;
    cout << endl;
    cout << "Введите полный путь до файла чтения: ";
    getline(cin >> ws, pathRead);
    cout << endl;
    cout << "Введите полный путь для создания нового файла: ";
    getline(cin >> ws, pathWrite);
    cout << endl;
    parser.readFile(pathRead, lines);
    cout<<"-----------------------------"<<endl;

    for(auto cline : lines)
    {
        if(cline.type == Line::TYPE::OSM)
        {
            osm_lines.push_back(cline);

        }
        else if(cline.type == Line::TYPE::HD)
        {
            hd_lines.push_back(cline);
        }
    }
    for(auto osm_line : osm_lines )
    {

        if(osm_line.sizeFollow() == 0)
        {
            auto &p = osm_line.back();

            for(auto hd_line : hd_lines )
            {

                auto a = osm_line.front().getVector(osm_line.back());
                auto b = hd_line.front().getVector(hd_line.back());
                auto angle = Point::getAngle(a, b) * 3.14 / 180.0;
                if(hd_line.sizePrev() == 0 && p.distanceTo(hd_line.front()) < maxdist && angle < maxAngle)
                {
                    Line buf;
                    buf.type = Line::TYPE::LINK;
                    buf.addPoint(p);
                    buf.addPoint(hd_line.front());
                    lines.push_back(buf);
                }
            }
        }

        if(osm_line.sizePrev() == 0)
        {
            auto &p = osm_line.front();

            for(auto hd_line : hd_lines)
            {

                auto a = osm_line.front().getVector(osm_line.back());
                auto b = hd_line.front().getVector(hd_line.back());
                auto angle = Point::getAngle(a, b) * 3.14 / 180.0;
                if(hd_line.sizeFollow() == 0 && p.distanceTo(hd_line.back()) < maxdist && angle < maxAngle)
                {
                    Line buf;
                    buf.type = Line::TYPE::LINK;
                    buf.addPoint(hd_line.back());
                    buf.addPoint(p);
                    lines.push_back(buf);
                }
            }
        }
    }



    parser.saveFile(pathWrite,lines);
    return 0;
}

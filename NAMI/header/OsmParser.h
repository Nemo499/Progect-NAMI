#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <Point.h>
#include <Line.h>
#include "GPSPoint.h"
#include <algorithm>
#include <string>

using namespace std;
using namespace pugi;


class OsmParser {
public:

    long long freeId {0};
    using ll = long long;
    OsmParser() = default;// запись конструктора по умолчанию

    long long getFreeId(xml_node root)
    {
        std::vector<ll> ids;

        string node_name = "node";

        for (xml_node node = root.child(node_name.c_str()); node; node = node.next_sibling(node_name.c_str()))
        {
            ids.push_back(atoll(node.attribute("id").value()));
        }

        node_name = "way";

        for (xml_node node = root.child(node_name.c_str()); node; node = node.next_sibling(node_name.c_str()))
        {
            ids.push_back(atoll(node.attribute("id").value()));
        }

        sort(ids.begin(), ids.end());

        return ids.empty() ? 0 : ids.back() + 1;
    }

    bool readFile(string input_filename, vector<Line>& lines)
    {
        xml_document doc;
        freeId = 0;
        if(doc.load_file(input_filename.c_str()))// читаем докумнт
        {

            xml_node root = doc.document_element();
            xml_node node2;
            string node_name = "way";
            string node_name2 = "nd";
            int nomerPoint{0};
            Line buf;
            // перебор линий
            for (xml_node node = root.child(node_name.c_str()); node; node = node.next_sibling(node_name.c_str()))
            {
                buf = Line(atoll(node.attribute("id").value()));
                lines.push_back(buf);// добавляем линию

                for(node2 = node.child(node_name2.c_str()); node2; node2 = node2.next_sibling(node_name2.c_str()))
                {
                    lines.back().addPoint(Point(atoll(node2.attribute("ref").value())));// добавляем точку
                }
                node_name2="tag";
                node2 = node.child(node_name2.c_str());
                string type = node2.attribute("v").value();
                if(type == "osm")
                {
                    lines[lines.size()-1].type = Line::TYPE::OSM;
                }
                else
                {
                    lines[lines.size()-1].type = Line::TYPE::HD; 
                }
                node_name2 = "nd";
            }

            for(int i = 0; i < lines.size(); i++)
            {
                for (int j = 0; j < lines.size(); j++)
                {
                    if( j != i )
                    {
                        if(lines[i].points[0].getId() == lines[j].points.back().getId())
                        {
                            lines[i].prev_lines_id.push_back(j);// линии предшественники
                        }
                        else if (lines[i].points.back().getId() == lines[j].points[0].getId())
                        {
                            lines[i].follow_lines_id.push_back(j);// линии последователи
                        }
                    }
                }
            }

            // перебор точек
            node_name = "node";
            for (xml_node node = root.child(node_name.c_str()); node; node = node.next_sibling(node_name.c_str()))
            {
                //ищем линию, к которой относится точка
                for(int i = 0; i < lines.size(); i++)
                {
                    nomerPoint = lines[i].searchPoint(atoll(node.attribute("id").value()));// получаем номер точки в векторе точек линии, если такой нет, то возвращается -2
                    if(nomerPoint >= 0)
                    {
                        auto &point = lines[i].points[nomerPoint];
                        //преобразуем координаты
                        GPSPoint::convertWGStoGeocentric(atof(node.attribute("lat").value()), atof(node.attribute("lon").value()), 0.0, point.x, point.y, point.z);
                        // выходим из цикла
                        //break;
                    }
                }

            }
            freeId = getFreeId(root);
        }
        else
        {
            cout<<"ошибка чтения файла"<<endl;
            return false;
        }

        return true;
    }

    bool saveFile (string input_filename, vector<Line>& lines)
    {
        xml_document doc;
        xml_node node;
        xml_node nd;
        xml_node osm;
        double lat{0.0};
        double lon{0.0};
        double ele{0.0};
        osm = doc.append_child("osm");
        osm.append_attribute("version") = "0.6";
        osm.append_attribute("generator") = "JOSM";
        // добавляем точки
        for (int i = 0; i < lines.size(); i++)
        {
            for (int j = 0; j < lines[i].points.size(); j++)
            {
                //преобразуем координаты из декартовой системы в географические
                GPSPoint::convertGeocentricToWGS(lines[i].points[j].x, lines[i].points[j].y, lines[i].points[j].z, lat, lon, ele);
                node = osm.append_child("node");// создаём элемнт node
                node.append_attribute("id") = lines[i].points[j].getId(); // добовляем атрибут id
                node.append_attribute("action") = "modify"; // добовляем атрибут action
                node.append_attribute("visible") = "true"; // добовляем атрибут visible
                node.append_attribute("lat") = lat; // добовляем атрибут lat
                node.append_attribute("lon") = lon; // добовляем атрибут lon
                node.append_attribute("version") = 1; // добовляем атрибут version
            }
        }
        //добавляем линии
        for (int i = 0; i < lines.size(); i++)
        {
            node = osm.append_child("way");// создаём элемнт node
            if(lines[i].getId()==0)
            {
                node.append_attribute("id") = freeId;
                freeId++;
            }
            else {
                node.append_attribute("id") = lines[i].getId();
            }
            node.append_attribute("action") = "modify"; // добовляем атрибут action
            node.append_attribute("visible") = "true"; // добовляем атрибут visible
            node.append_attribute("version") = 1; // добовляем атрибут version
            for(int j = 0; j < lines[i].points.size(); j++)
            {
                nd = node.append_child("nd");
                nd.append_attribute("ref") = lines[i].points[j].getId();// добовляем атрибут ref
            }
            nd = node.append_child("tag");
            nd.append_attribute("k") = "type"; //добовляем атрибут type
            if(lines[i].type == Line::TYPE::HD)
            {
                nd.append_attribute("v") ="hd";
            }
            else if (lines[i].type == Line::TYPE::OSM)
            {
                nd.append_attribute("v") ="osm";
            }
            else
            {
                nd.append_attribute("v") ="link";
            }
            //nd.append_attribute("v") = lines[i].line.tipe.c_str(); // добовляем атрибут v
        }

        if(doc.save_file(input_filename.c_str()))
        {
            cout << "Файл сохранён успешно" << endl;
        }
        else
        {
            cout << "Ошибка сохранения файла" << endl;
            return false;
        }
        return true;
    }

};

#pragma once
#include <vector>

class point
{
    static int _prePointID;
    int pointID; // 顶点索引
    double _x, _y, _z; // 顶点的三个坐标
public:
    point(double x, double y, double z) :_x(x), _y(y), _z(z) {
        pointID = _prePointID++;
    }
    point(double x, double y, double z, bool key) :_x(x), _y(y), _z(z) {
        if (key) {
            pointID = _prePointID++;
        }
        else {
            pointID = _prePointID;
        }
    }
    bool operator<(point o) const {
        return (_x < o.GetX()) || 
            (_x == o.GetX() && _y < o.GetY()) || 
            (_x == o.GetX() && _y == o.GetY() && _z < o.GetZ());
    }
    bool operator==(point o) const {
        return _x == o.GetX() && _y == o.GetY() && _z == o.GetZ();
    }
    int GetID() {
        return pointID;
    }
    double GetX() {
        return _x;
    }
    double GetY() {
        return _y;
    }
    double GetZ() {
        return _z;
    }
};

class normal
{
public:
    double _x, _y, _z;
    normal(double x, double y, double z) :_x(x), _y(y), _z(z) {}
};

// 三角形数据结构
class facet
{
    static int _preFaceID;
    int faceID; // 三角形索引
    int _v1, _v2, _v3; // 三角形三个顶点的索引
    normal _normal; // 三角形的法向量
    std::vector<int> adjTri; // 邻接三角形的索引
public:
    facet(int v1, int v2, int v3, normal normal):_v1(v1), _v2(v2), _v3(v3), _normal(normal) {
        faceID = _preFaceID++;
    }
    int GetID() {
        return faceID;
    }
    /*get_1_ring_facets() {

    }*/
};

// 三角网络曲面数据结构
class triangle_mesh
{
public:
    std::vector<point> points; // 点集
    std::vector<facet> facets; // 三角形集
    /*get_pointsv() {
        
    }
    get_facets() {
        
    }*/
};

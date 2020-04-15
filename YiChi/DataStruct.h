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
    normal():_x(.0),_y(.0),_z(.0) {}
    normal(double x, double y, double z) :_x(x), _y(y), _z(z) {}
    normal& operator=(const normal& o) {
        if (this != &o) {
            _x = o._x;
            _y = o._y;
            _z = o._z;
        }
        return *this;
    }
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
    facet(const facet& o) {
        _v1 = o._v1;
        _v2 = o._v2;
        _v3 = o._v3;
        _normal = o._normal;
    }
    facet& operator=(const facet& o) {
        if (this != &o) {
            _v1 = o._v1;
            _v2 = o._v2;
            _v3 = o._v3;
            _normal = o._normal;
        }
        return *this;
    }
    /*get_1_ring_facets() {

    }*/
};

class edge
{
    point* _p1, * _p2;
    std::vector<facet*> nearTri;
public:
    edge():_p1(NULL), _p2(NULL) {}
    edge(point* p1, point* p2) :_p1(p1), _p2(p2) {}
    bool operator<(edge o) const {
        if (*_p1 == *o._p1) {
            return *_p2 < *o._p2;
        }
        else {
            return *_p1 < *o._p1;
        }
    }
    bool operator==(edge o) const {
        return (*_p1 == *o._p1 && *_p2 == *o._p2) ||
            (*_p1 == *o._p2 && *_p2 == *o._p1);
    }
    edge& operator=(const edge& o) {
        if (this != &o) {
            _p1 = o._p1;
            _p2 = o._p2;
            for (int i = 0; i < o.nearTri.size(); ++i) {
                facet* temp = NULL;
                nearTri.push_back(temp);
                nearTri[nearTri.size() - 1] = o.nearTri[i];
            }
        }
        return *this;
    }

    void putNearTri(facet& _facet) {
        nearTri.push_back(&_facet);
    }
};

// 三角网络曲面数据结构
class triangle_mesh
{
public:
    std::vector<point> points; // 点集
    std::vector<facet> facets; // 三角形集
    std::vector<edge> edges;
    /*get_pointsv() {
        
    }
    get_facets() {
        
    }*/
};

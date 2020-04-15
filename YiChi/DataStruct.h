#pragma once
#include <vector>
#include <iostream>

class edge;

class point
{
    static int _prePointID;
    int pointID; // 顶点索引
    double _x, _y, _z; // 顶点的三个坐标
    std::vector<edge*> adjTriEdge;
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
    // std::vector<int> adjTri; // 邻接三角形的索引
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
    double _length;
    std::vector<facet*> nearTri;
    std::vector<edge*> adjTriEdge;
    std::vector<double> alpheBeta;
    std::vector<point*> adjTriPoint;
public:
    edge():_p1(NULL), _p2(NULL), _length(0) {}
    edge(point* p1, point* p2) :_p1(p1), _p2(p2) {
        _length = sqrt((_p1->GetX() - _p2->GetX()) * (_p1->GetX() - _p2->GetX()) +
            (_p1->GetY() - _p2->GetY()) * (_p1->GetY() - _p2->GetY()) +
            (_p1->GetZ() - _p2->GetZ()) * (_p1->GetZ() - _p2->GetZ())); // 长度
    }
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
            _length = o._length;
            for (int i = 0; i < o.nearTri.size(); ++i) {
                facet* temp = NULL;
                nearTri.push_back(temp);
                nearTri[nearTri.size() - 1] = o.nearTri[i];
            }
        }
        return *this;
    }
    double calc_he() {
        // 计算边的评价因子
        if (adjTriEdge.size() == 4) {
            return 6 * _length / (2 * _length +
            adjTriEdge[0]->GetLength() +
            adjTriEdge[1]->GetLength() +
            adjTriEdge[2]->GetLength() +
            adjTriEdge[3]->GetLength()
            );
        }
        else {
            // std::cout << "邻接三角形不是两个，无法计算评价因子" << adjTriEdge.size() << std::endl;
            return 0;
        }
    }
    double calc_curvature_weight() {
        // 计算边的曲率权值
        // std::cout << alpheBeta.size();
        if (alpheBeta.size() == 2) {
            double d = 1.0 / tan(alpheBeta[0]) + 1.0 / tan(alpheBeta[0]);
            return d;
        }
        // std::cout << "邻接三角形不是两个，无法计算曲率权值" << alpheBeta.size() << std::endl;
        return 0;
    }
    void putNearTri(facet& _facet) {
        nearTri.push_back(&_facet);
    }
    void AddAdjTriEdge(edge* e) {
        adjTriEdge.push_back(e);
    }
    void AddAdjTriPoint(point* p) {
        adjTriPoint.push_back(p);
    }
    void AddAlpheBeta(edge* b, edge* c) {
        double d = acos((b->GetLength() * b->GetLength() + 
            c->GetLength() * c->GetLength() -
            _length * _length
        )/ (2 * b->GetLength() * c->GetLength()));
        alpheBeta.push_back(d);
    }
    double GetLength() {
        return _length;
    }
    point* GetP1() {
        return _p1;
    }
    point* GetP2() {
        return _p2;
    }
    std::vector<point*> GetAdjTriPoint() {
        return adjTriPoint;
    }
};

// 三角网络曲面数据结构
class triangle_mesh
{
public:
    std::vector<point> points; // 点集
    std::vector<facet> facets; // 三角形集
    std::vector<edge*> edges;
    /*get_pointsv() {
        
    }
    get_facets() {
        
    }*/
};

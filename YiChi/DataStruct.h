#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

class edge;
class normal;
class facet;

class point
{
    static int _prePointID;
    int pointID; // ��������
    double _x, _y, _z; // �������������
    std::vector<facet*> adjTri;
    std::vector<edge*> adjEdge;
    std::vector<edge*> adjTriOpEdge;
    std::vector<normal*> adjTriOpNormal;
    bool _isDeleted;
public:
    point():_x(0), _y(0), _z(0), pointID(0), _isDeleted(false){}
    point(double x, double y, double z) :_x(x), _y(y), _z(z), _isDeleted(false) {
        pointID = _prePointID++;
    }
    point(double x, double y, double z, bool key) :_x(x), _y(y), _z(z), _isDeleted(false) {
        if (key) {
            pointID = _prePointID++;
        }
        else {
            pointID = _prePointID;
        }
    }
    void AddAll(std::vector<facet*> fV, std::vector<edge*> eV, std::vector<edge*> oeV, std::vector<normal*> onV, edge* olde) {
        for (facet* f : fV) {
            if (!f->GetDeleted()) {
                adjTri.push_back(f);
            }
        }
        for (edge* e : eV) {
            if (!e->GetDeleted()) {
                adjEdge.push_back(e);
                // �������˵�������ڽӱߵĶ˵��Ϊpm_
                e->ChangeAnotherEndPoint(this, olde);
            }
        }
        for (int i = 0; i < oeV.size(); ++i) {
            if (!oeV[i]->GetDeleted()) {
                adjTriOpEdge.push_back(oeV[i]);
                adjTriOpNormal.push_back(onV[i]);
            }
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
    double GetX() {
        return _x;
    }
    double GetY() {
        return _y;
    }
    double GetZ() {
        return _z;
    }
    void AddAdjTri(facet* f) {
        adjTri.push_back(f);
    }
    void AddAdjEdge(edge* e) {
        adjEdge.push_back(e);
    }
    std::vector<edge*> GetAdjEdge() {
        return adjEdge;
    }
    void AddAdjTriOpEdge(edge* e) {
        adjTriOpEdge.push_back(e);
    }
    void AddAdjTriOpNormal(normal* n) {
        adjTriOpNormal.push_back(n);
    }
    std::vector<facet*> GetAdjTri() {
        return adjTri;
    }
    std::vector<edge*> GetAdjTriOpEdge() {
        return adjTriOpEdge;
    }
    std::vector<normal*> GetAdjTriOpNormal() {
        return adjTriOpNormal;
    }
    void SetDeleted() {
        _isDeleted = true;
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

// ���������ݽṹ
class facet
{
    static int _preFaceID;
    int faceID; // ����������
    point* _p1, * _p2, * _p3; // �������������������
    normal _normal; // �����εķ�����
    // std::vector<int> adjTri; // �ڽ������ε�����
    edge* _edge1, * _edge2, * _edge3;
    bool _isDeleted;
public:
    facet(point* v1, point* v2, point* v3, normal normal):_p1(v1), _p2(v2), _p3(v3), _normal(normal) {
        faceID = _preFaceID++;
        _isDeleted = false;
    }
    int GetID() {
        return faceID;
    }
    facet(const facet& o) {
        _p1 = o._p1;
        _p2 = o._p2;
        _p3 = o._p3;
        _normal = o._normal;
        _isDeleted = o._isDeleted;
        _edge1 = o._edge1;
        _edge2 = o._edge2;
        _edge3 = o._edge3;
    }
    facet& operator=(const facet& o) {
        if (this != &o) {
            _p1 = o._p1;
            _p2 = o._p2;
            _p3 = o._p3;
            _normal = o._normal;
            _isDeleted = o._isDeleted;
            _edge1 = o._edge1;
            _edge2 = o._edge2;
            _edge3 = o._edge3;
        }
        return *this;
    }
    normal* GetNormalAdd() {
        return &_normal;
    }
    void SetDeleted() {
        _edge1->SetDeleted();
        _edge2->SetDeleted();
        _edge3->SetDeleted();
        _isDeleted = true;
    }
    bool GetDeleted() {
        return _isDeleted;
    }
    void SetEdges(edge* e1, edge* e2, edge* e3) {
        _edge1 = e1;
        _edge2 = e2;
        _edge3 = e3;
    }
    std::vector<edge*> GetEdges() { // ��δʹ��
        std::vector<edge*> result;
        result.push_back(_edge1);
        result.push_back(_edge2);
        result.push_back(_edge3);
        return result;
    }
    std::vector<edge*> GetEdgesExE(edge* e) {
        std::vector<edge*> result;
        if (_edge1 != e) {
            result.push_back(_edge1);
        }
        if (_edge2 != e) {
            result.push_back(_edge2);
        }
        if (_edge3 != e) {
            result.push_back(_edge3);
        }
        if (result.size() != 2) {
            std::cout << "error: �����ܳ��ֵĴ���GetEdgeExE" << std::endl;
        }
        return result;
    }
};

class edge
{
    point* _p1, * _p2;
    double _length;
    std::vector<facet*> adjTri; // �ڽ�������
    std::vector<edge*> adjTriEdges; // �ڽ��������е������ĸ���
    std::vector<double> alpheBeta; // ���ڽ��������жԽǵĽǶ�
    std::unordered_map<facet*,point*> adjTriOpPoint; // ���ڽ��������еĶԵ�
    bool _isDeleted;
public:
    edge():_p1(NULL), _p2(NULL), _length(0), _isDeleted(false){}
    edge(point* p1, point* p2) :_p1(p1), _p2(p2), _isDeleted(false) {
        _length = sqrt((_p1->GetX() - _p2->GetX()) * (_p1->GetX() - _p2->GetX()) +
            (_p1->GetY() - _p2->GetY()) * (_p1->GetY() - _p2->GetY()) +
            (_p1->GetZ() - _p2->GetZ()) * (_p1->GetZ() - _p2->GetZ())); // ����
    }
    void AddAll(std::vector<edge*> oldEV) {
        // oldEV��ԭ��������
        for (auto olde: oldEV) {
            for (auto oldAdjTri: olde->GetAdjTri()) {
                if (!oldAdjTri->GetDeleted()) {
                    adjTri.push_back(oldAdjTri);
                    std::vector<edge*> edgesExOldE = oldAdjTri->GetEdgesExE(olde);
                    for (auto oldAdjTriEdge : edgesExOldE) {
                        adjTriEdges.push_back(oldAdjTriEdge);
                    }
                    AddAlpheBeta(edgesExOldE[0],edgesExOldE[1]);
                }
            }
            for (auto temp : olde->GetAdjTriOpPointExE(this)) {
                adjTriOpPoint.insert(temp);
            }
        }
        // ����alpheBeta
        
        // �ڸ�����������κ󣬸���alpheBeta
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
            _isDeleted = o._isDeleted;
            for (int i = 0; i < o.adjTri.size(); ++i) {
                facet* temp = NULL;
                adjTri.push_back(temp);
                adjTri[adjTri.size() - 1] = o.adjTri[i];
            }
        }
        return *this;
    }
    double calc_he() {
        // ����ߵ���������
        if (adjTriEdges.size() == 4) {
            return 6 * _length / (2 * _length +
            adjTriEdges[0]->GetLength() +
            adjTriEdges[1]->GetLength() +
            adjTriEdges[2]->GetLength() +
            adjTriEdges[3]->GetLength()
            );
        }
        else {
            // std::cout << "�ڽ������β����������޷�������������" << adjTriEdges.size() << std::endl;
            return 0;
        }
    }
    double calc_curvature_weight() {
        // ����ߵ�����Ȩֵ
        // std::cout << alpheBeta.size();
        if (alpheBeta.size() == 2) {
            double d = 1.0 / tan(alpheBeta[0]) + 1.0 / tan(alpheBeta[0]);
            return d;
        }
        // std::cout << "�ڽ������β����������޷���������Ȩֵ" << alpheBeta.size() << std::endl;
        return 0;
    }
    void AddAdjTri(facet* _facet) {
        adjTri.push_back(_facet);
    }
    std::vector<facet*> GetAdjTri() {
        return adjTri;
    }
    void AddAdjTriEdge(edge* e) {
        adjTriEdges.push_back(e);
    }
    void AddAdjTriOpPoint(facet* f, point* p) {
        adjTriOpPoint.insert({ f,p });
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
    std::unordered_map<facet*, point*> GetAdjTriOpPoint() {
        return adjTriOpPoint;
    }
    std::unordered_map<facet*, point*> GetAdjTriOpPointExE(edge* e) {
        std::unordered_map<facet*, point*> result;
        for (auto m: adjTriOpPoint) {
            if (m.second != e->GetP1() && m.second != e->GetP2()) {
                result.insert(m);
            }
        }
        if (result.size() != 1) {
            std::cout << "error: �����ܳ��ֵĴ���GetAdjTriOpPointExE" << std::endl;
        }
        return result;
    }
    void SetDeleted() {
        _isDeleted = true;
    }
    bool GetDeleted() {
        return _isDeleted;
    }
    // ���ĵ�ǰ�����e��ͬ���Ǹ��˵�Ϊpm_
    void ChangeAnotherEndPoint(point* pm_, edge* e) {
        point* p1 = e->GetP1(), * p2 = e->GetP2();
        if (_p1 != p1 &&
            _p2 != p1 &&
            _p1 != p2 &&
            _p2 != p2) {
            std::cout << "error: �����ܳ��ֵĴ���ChangeAnotherEndPoint" << std::endl;
        }
        if (_p1 == p1 || _p1 == p2) {
            _p1 = pm_;
        }
        else if (_p2 == p1 || _p2 == p2) {
            _p2 = pm_;
        }
        // ���³���
        _length = sqrt((_p1->GetX() - _p2->GetX()) * (_p1->GetX() - _p2->GetX()) +
            (_p1->GetY() - _p2->GetY()) * (_p1->GetY() - _p2->GetY()) +
            (_p1->GetZ() - _p2->GetZ()) * (_p1->GetZ() - _p2->GetZ()));
        // ����������
    }
};

// ���������������ݽṹ
class triangle_mesh
{
public:
    std::vector<point*> points; // �㼯
    std::vector<facet*> facets; // �����μ�
    std::vector<edge*> edges;
};

#pragma warning(disable : 4018)
#pragma once
#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <algorithm>

class edge;
class facet;
class normal;

class point
{
    static int _prePointID;
    int pointID; // 顶点索引
    double _x, _y, _z; // 顶点的三个坐标
    std::vector<facet*> adjTri;
    std::vector<edge*> adjEdge; // 邻接边，即以这个点作为端点的边
    std::vector<edge*> adjTriOpEdge;
    std::vector<normal*> adjTriOpNormal;
    std::vector<point*> adjPoint;
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
    void AddAdjTri(std::vector<facet*> fV,
        edge* newe1,
        edge* newe2);
    void AddAdjEdge(edge* e) {
        adjEdge.push_back(e);
    }
    void AddAdjEdge(std::vector<edge*> eV, edge* olde);
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
    bool GetDeleted() {
        return _isDeleted;
    }
    void AddAdjTriOpEdgeAndAdjTriOpNormal(point* op);
    void UpdateAdjTriAndAdjEdge(edge* ne);
    void UpdateAdjTri();
    void UpdateAdjEdge();
    void UpdateAdjTriOpEdgeAndAdjTriOpNormal();
    void UpdateAdjEdgeOpPointData();
    void SetAdjPoint();
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
    void setXYZ(double x, double y, double z) {
        _x = x;
        _y = y;
        _z = z;
    }
    double GetAngleWithN(normal& n) {
        return acos(
            (_x * n._x + _y * n._y + _z * n._z) /
            ((sqrt(_x * _x + _y * _y + _z * _z)) *
             (sqrt(n._x * n._x + n._y * n._y + n._z * n._z)))
        );
    }
    void Negate() {
        _x = -_x;
        _y = -_y;
        _z = -_z;
    }
};

// 三角形数据结构
class facet
{
    static int _preFaceID;
    int faceID; // 三角形索引
    point* _p1, * _p2, * _p3; // 三角形三个顶点的索引
    normal _normal; // 三角形的法向量
    // std::vector<int> adjTri; // 邻接三角形的索引
    edge* _edge1, * _edge2, * _edge3;
    bool _isDeleted;
    double _miniInnerAngle;
public:
    facet(point* v1, point* v2, point* v3, normal normal):
        _p1(v1), 
        _p2(v2), 
        _p3(v3), 
        _normal(normal), 
        _edge1(nullptr), 
        _edge2(nullptr), 
        _edge3(nullptr) {
        faceID = _preFaceID++;
        _isDeleted = false;
        _miniInnerAngle = 0;
    }
    facet(edge* newE, edge* oldE, point* oldP);
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
    void SetDeleted();
    void SetSelfDeleted() {
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
    std::vector<edge*> GetEdges() { // 暂未使用
        std::vector<edge*> result;
        result.push_back(_edge1);
        result.push_back(_edge2);
        result.push_back(_edge3);
        return result;
    }
    std::vector<edge*> GetEdgesExE(edge* e, edge* ne) {
        std::vector<edge*> result;
        // bool _edge1Result = false, _edge2Result = false, _edge3Result = false;
        if (_edge1 != e && _edge1 != ne) {
            result.push_back(_edge1);
        }
        if (_edge2 != e && _edge2 != ne) {
            result.push_back(_edge2);
        }
        if (_edge3 != e && _edge3 != ne) {
            result.push_back(_edge3);
        }
        /****result.size()可能为1****/
        if (result.size() != 2 && result.size() != 1) {
            std::cout << "error: 不可能出现的错误，GetEdgesExE" << std::endl;
        }
        return result;
    }
    void UpdatePointAndEdge(point* pm_, edge* ne1, edge* ne2);
    bool containPoint(point* p) {
        if ((_p1 == p) || (_p2 == p) || (_p3 == p)) {
            return true;
        }
        return false;
    }
    edge* GetEdgeOpP(point* p);
    point* GetPointExE(edge* e);
    std::vector<point*> GetPoints() {
        std::vector<point*> pointsAdd;
        pointsAdd.push_back(_p1);
        pointsAdd.push_back(_p2);
        pointsAdd.push_back(_p3);
        return pointsAdd;
    }
    void UpdateMiniInnerAngle();
    double GetMiniInnerAngle() {
        return _miniInnerAngle;
    }
    std::vector<point*> GetPointOrderStartWithP(point * p) {
        std::vector<point*> result;
        if (_p1 == p) {
            result.push_back(_p1);
            result.push_back(_p2);
            result.push_back(_p3);
        }
        else if (_p2 == p) {
            result.push_back(_p2);
            result.push_back(_p3);
            result.push_back(_p1);
        }
        else if (_p3 == p) {
            result.push_back(_p3);
            result.push_back(_p1);
            result.push_back(_p2);
        }
        else {
            std::cout << "error: 不可能出现的错误，GetPointOrderStartWithP" << std::endl;
        }
        return result;
    }
};

class edge
{
    point* _p1, * _p2;
    double _length;
    std::vector<facet*> adjTri; // 邻接三角形
    std::vector<edge*> adjTriEdges; // 邻接三角形中的其他四个边
    std::vector<double> alpheBeta; // 在邻接三角形中对角的角度
    std::unordered_map<facet*,point*> adjTriOpPoint; // 在邻接三角形中的对点
    bool _isDeleted;
public:
    edge():_p1(NULL), _p2(NULL), _length(0), _isDeleted(false){}
    edge(point* p1, point* p2) :_p1(p1), _p2(p2), _isDeleted(false) {
        _length = sqrt((_p1->GetX() - _p2->GetX()) * (_p1->GetX() - _p2->GetX()) +
            (_p1->GetY() - _p2->GetY()) * (_p1->GetY() - _p2->GetY()) +
            (_p1->GetZ() - _p2->GetZ()) * (_p1->GetZ() - _p2->GetZ())); // 长度
    }
    void AddAll(std::vector<edge*> oldEV, edge* ne) {
        // oldEV是当前处理的边e的某一个邻接三角形中其他两条边
        for (auto olde: oldEV) {
            for (auto oldAdjTri: olde->GetAdjTri()) {
                if (oldAdjTri->GetDeleted() == false) {
                    adjTri.push_back(oldAdjTri);
                    bool flag = true;// flag为true说明是ne1
                    std::vector<edge*> edgesExOldE = oldAdjTri->GetEdgesExE(olde, ne);/*****此处已经将邻接三角形的顶点和边换成了新的点和边*****/
                    for (auto oldAdjTriEdge : edgesExOldE) {
                        adjTriEdges.push_back(oldAdjTriEdge);
                    }
                    // 更新alpheBeta
                    if (edgesExOldE.size() == 2) {
                        AddAlpheBeta(edgesExOldE[0], edgesExOldE[1]);
                    }
                    else if (edgesExOldE.size() == 1) {
                        AddAlpheBeta(edgesExOldE[0], ne);
                    }
                    else {
                        std::cout << "error: 不可能出现的错误，edge::AddAll" << std::endl;
                    }
                    
                }
            }
            // 添加边在邻接三角形中的对点
            for (auto temp : olde->GetAdjTriOpPointExE(this)) {
                adjTriOpPoint.insert(temp);
            }
        }
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
        // 计算边的评价因子
        if (adjTriEdges.size() == 4) {
            return 6 * _length / (2 * _length +
            adjTriEdges[0]->GetLength() +
            adjTriEdges[1]->GetLength() +
            adjTriEdges[2]->GetLength() +
            adjTriEdges[3]->GetLength()
            );
        }
        else {
            // std::cout << "邻接三角形不是两个，无法计算评价因子" << adjTriEdges.size() << std::endl;
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
    void AddAdjTri(facet* _facet) {
        adjTri.push_back(_facet);
    }
    std::vector<facet*> GetAdjTri() {
        return adjTri;
    }
    void AddAdjTriEdge(edge* e) {
        adjTriEdges.push_back(e);
    }
    void AddAdjTriEdge(std::vector<edge*> eV) {
        for(edge* e: eV) {
            adjTriEdges.push_back(e);
        }
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
    std::unordered_map<facet*, point*>* GetAdjTriOpPointAdd() {
        return &adjTriOpPoint;
    }
    std::unordered_map<facet*, point*> GetAdjTriOpPointExE(edge* e) {
        std::unordered_map<facet*, point*> result;
        for (auto m: adjTriOpPoint) {
            if (m.second != e->GetP1() && m.second != e->GetP2() && m.second->GetDeleted() == false) {
                result.insert(m);
            }
        }
        /*if (result.size() != 1) {
            std::cout << "error: 不可能出现的错误，GetAdjTriOpPointExE" << std::endl;
        }*/
        return result;
    }
    void SetDeleted() {
        _isDeleted = true;
    }
    bool GetDeleted() {
        return _isDeleted;
    }
    // 更改当前边与边e共同的那个端点为pm_
    void ChangeAnotherEndPoint(point* pm_, edge* e) {
        point* p1 = e->GetP1(), * p2 = e->GetP2();
        if (_p1 != p1 &&
            _p2 != p1 &&
            _p1 != p2 &&
            _p2 != p2) {
            std::cout << "error: 不可能出现的错误，ChangeAnotherEndPoint" << std::endl;
        }
        if (_p1 == p1 || _p1 == p2) {
            _p1 = pm_;
        }
        else if (_p2 == p1 || _p2 == p2) {
            _p2 = pm_;
        }
        // 更新长度
        _length = sqrt((_p1->GetX() - _p2->GetX()) * (_p1->GetX() - _p2->GetX()) +
            (_p1->GetY() - _p2->GetY()) * (_p1->GetY() - _p2->GetY()) +
            (_p1->GetZ() - _p2->GetZ()) * (_p1->GetZ() - _p2->GetZ()));
        // 改其他数据
    }
    point* GetPointExP(point* p) {
        if (_p1 == p) {
            return _p2;
        }
        else if (_p2 == p) {
            return _p1;
        }
        else {
            std::cout << "error: 不可能出现的错误，GetPointExP" << std::endl;
            return nullptr;
        }
    }
    void UpdateAdjTriEdgesAndAlpheBeta() {
        // 更新adjTriEdges和alpheBeta
        adjTriEdges.clear();
        alpheBeta.clear();
        for (auto f: adjTri) {
            std::vector<edge*> eV = f->GetEdgesExE(this, this);
            if (eV.size() == 2) {
                adjTriEdges.push_back(eV[0]);
                adjTriEdges.push_back(eV[1]);
                AddAlpheBeta(eV[0], eV[1]);
            }
            else {
                std::cout << "error: 不可能出现的错误，UpdateAdjTriEdgesAndAlpheBeta" << std::endl;
            }
        }
    }
    void UpdateAdjTriEdges() {
        // 更新adjTriEdges
        /*if (adjTri.size() != 2) {
            std::cout << "error: 不可能出现的错误，UpdateAdjTriEdges1" << std::endl;
        }*/
        adjTriEdges.clear();
        for (auto f : adjTri) {
            std::vector<edge*> eV = f->GetEdgesExE(this, this);
            if (eV.size() == 2) {
                adjTriEdges.push_back(eV[0]);
                adjTriEdges.push_back(eV[1]);
            }
            else {
                std::cout << "error: 不可能出现的错误，UpdateAdjTriEdges2" << std::endl;
            }
        }
    }
    void UpdateAdjTriOpPoint(point* p, point* op) {
        /***注意这里迭代器会被insert操作破坏掉***/
        facet* f = nullptr;
        for (auto temp : adjTriOpPoint) {
            if (temp.second == op || temp.second == p) {
                f = temp.first;
            }
        }
        if (f == nullptr) {
            std::cout << "error: 不可能出现的错误，UpdateAdjTriOpPoint1" << std::endl;
        }
        else {
            adjTriOpPoint[f] = p;
        }
        // 做边界边处可以出现adjTriOpPoint.size() == 1
        /*if (adjTriOpPoint.size() != 1 && adjTriOpPoint.size() != 2) {
            std::cout << "error: 不可能出现的错误，UpdateAdjTriOpPoint2" << std::endl;
        }*/
    }
    void UpdateAdjTriOpPoint() {
        // 使用当前的adjTri对adjTriOpPoint进行更新整理
        adjTriOpPoint.clear();
        for (facet* f: adjTri) {
            adjTriOpPoint.insert({f,f->GetPointExE(this)});
        }
        if (adjTriOpPoint.size() != adjTri.size()) {
            std::cout << "error: 不可能出现的错误，UpdateAdjTriOpPoint" << std::endl;
        }
    }
    bool IsContainPoint(point* p) {
        if (_p1 == p || _p2 == p) {
            return true;
        }
        return false;
    }
    std::vector<edge*> GetAdjTriEdges() {
        return adjTriEdges;
    }
    void UpdateAdjTri() {
        std::stack<int> subs;
        for (int i = 0; i < adjTri.size(); ++i) {
            if (adjTri[i]->GetDeleted()) {
                subs.push(i);
            }
        }
        while (!subs.empty()) {
            adjTri.erase(adjTri.begin() + subs.top());
            subs.pop();
        }
    }
};

// 三角网络曲面数据结构
class triangle_mesh
{
public:
    std::vector<point*> points; // 点集
    std::vector<facet*> facets; // 三角形集
    std::vector<edge*> edges; // 边集
    std::vector<point*> pointsTemp; // 临时点集
    std::vector<edge*> edgesTemp; // 临时边集
    std::vector<facet*> facetsTemp; // 临时三角形集

    int EdgeExchangeTimes;

    int GetPointAmount() {
        int amount = 0;
        for (auto p: points) {
            if (!p->GetDeleted()) {
                amount++;
            }
        }
        return amount;
    }
    int GetFacetAmount() {
        int amount = 0;
        for (auto f : facets) {
            if (!f->GetDeleted()) {
                amount++;
            }
        }
        return amount;
    }
    int GetEdgeAmount() {
        int amount = 0;
        for (auto e : edges) {
            if (!e->GetDeleted()) {
                amount++;
            }
        }
        return amount;
    }
    void PrintEdgeLength() {
        for (auto e : edges) {
            std::cout << e->GetLength() << " ";
        }
    }
    void PrintNoBoundPointAdjTriAmount() {
        for (auto p: points) {
            if (p->GetAdjTri().size() == p->GetAdjEdge().size()) {
                std::cout << p->GetAdjTri().size() << " ";
            }
        }
    }
    void DataDeleteAndDataMerge1() {
        // 数据删除：将mesh.points和mesh.facets和mesh.edges中已经删除点和边进行物理删除
        std::stack<int> subs;
        for (int i = 0; i < points.size(); ++i) {
            if (points[i]->GetDeleted()) {
                subs.push(i);
            }
        }
        while (!subs.empty()) {
            if (!points[subs.top()]->GetDeleted()) {
                std::cout << "error: DataDeleteAndDataMerge1" << std::endl;
            }
            points.erase(points.begin() + subs.top());
            subs.pop();
        }
        for (int i = 0; i < facets.size(); ++i) {
            if (facets[i]->GetDeleted()) {
                subs.push(i);
            }
        }
        while (!subs.empty()) {
            if (!facets[subs.top()]->GetDeleted()) {
                std::cout << "error: DataDeleteAndDataMerge2" << std::endl;
            }
            facets.erase(facets.begin() + subs.top());
            subs.pop();
        }
        for (int i = 0; i < edges.size(); ++i) {
            if (edges[i]->GetDeleted()) {
                subs.push(i);
            }
        }
        while (!subs.empty()) {
            if (!edges[subs.top()]->GetDeleted()) {
                std::cout << "error: DataDeleteAndDataMerge3" << std::endl;
            }
            edges.erase(edges.begin() + subs.top());
            subs.pop();
        }
        // 数据合并：mesh.pointsTemp和mesh.edgesTemp中的数据合并到mesh.points和mesh.edges中
        for (auto p: pointsTemp) {
            points.push_back(p);
        }
        for (auto e: edgesTemp) {
            edges.push_back(e);
        }
            // 合并结束后将pointsTemp与edgesTemp清空
        pointsTemp.clear();
        edgesTemp.clear();
    }
    void UpdateFacetsMiniInnerAngle() {
        for (auto f: facets) {
            f->UpdateMiniInnerAngle();
        }
    }
    void DataDeleteAndDataMerge2() {
        // 数据删除：将mesh.facets和mesh.edges中已经删除点和边进行物理删除
        std::stack<int> subs;
        for (int i = 0; i < facets.size(); ++i) {
            if (facets[i]->GetDeleted()) {
                subs.push(i);
            }
        }
        while (!subs.empty()) {
            if (!facets[subs.top()]->GetDeleted()) {
                std::cout << "error: DataDeleteAndDataMerge2" << std::endl;
            }
            facets.erase(facets.begin() + subs.top());
            subs.pop();
        }
        for (int i = 0; i < edges.size(); ++i) {
            if (edges[i]->GetDeleted()) {
                subs.push(i);
            }
        }
        while (!subs.empty()) {
            if (!edges[subs.top()]->GetDeleted()) {
                std::cout << "error: DataDeleteAndDataMerge3" << std::endl;
            }
            edges.erase(edges.begin() + subs.top());
            subs.pop();
        }
        // 数据合并：mesh.facetsTemp和mesh.edgesTemp中的数据合并到mesh.facets和mesh.edges中
        for (auto f : facetsTemp) {
            facets.push_back(f);
        }
        for (auto e : edgesTemp) {
            edges.push_back(e);
        }
            // 合并结束后将facetsTemp与edgesTemp清空
        EdgeExchangeTimes = edgesTemp.size();
        facetsTemp.clear();
        edgesTemp.clear();
    }
    void clear() {
        points.clear(); // 点集
        facets.clear(); // 三角形集
        edges.clear(); // 边集
    }
};

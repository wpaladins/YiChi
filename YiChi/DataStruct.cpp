#include "DataStruct.h"
/*********************************************point*******************************************/
void point::AddAdjTri(std::vector<facet*> fV,
    edge* newe1,
    edge* newe2) {
    for (facet* f : fV) {
        if (!f->GetDeleted()) {
            // 此处得到的为图中淡红色三角形中的一半
            adjTri.push_back(f);
            // 更新三角形的顶点索引和边索引
            f->UpdatePointAndEdge(this, newe1, newe2);
        }
    }
}
void point::UpdateAdjTriOpEdgeAndAdjTriOpNormal() {
    // 删邻接三角形对应的数据
    adjTriOpEdge.clear();
    adjTriOpNormal.clear();
    for (facet* f : adjTri) {
        adjTriOpEdge.push_back(f->GetEdgeOpP(this));
        adjTriOpNormal.push_back(f->GetNormalAdd());
    }
}
void point::AddAdjEdge(std::vector<edge*> eV, edge* olde) {
    for (edge* e : eV) {
        if (e->GetDeleted() == false) {
            // 此处得到的为图中蓝色边的一半
            adjEdge.push_back(e);
            // 边两个端点的所有邻接边的端点改为pm_
            e->ChangeAnotherEndPoint(this, olde);
            // 更新边的邻接三角形的四条边（先更新点的邻接三角形）
            e->UpdateAdjTriEdgesAndAlpheBeta();
        }
    }
}
void point::AddAdjTriOpEdgeAndAdjTriOpNormal(point* op) {
    std::vector<edge*> oeV = op->GetAdjTriOpEdge();
    std::vector<normal*> onV = op->GetAdjTriOpNormal();
    for (int i = 0; i < oeV.size(); ++i) {
        if (oeV[i]->GetDeleted() == false) {
            // 此处得到的为绿色边中的一半
            adjTriOpEdge.push_back(oeV[i]);
            adjTriOpNormal.push_back(onV[i]);
            // 更新绿色边的四邻边和对角
            oeV[i]->UpdateAdjTriEdgesAndAlpheBeta();
            // 更新绿色边的对点
            if (oeV[i]->GetAdjTri().size() != oeV[i]->GetAdjTriOpPoint().size()) {
                oeV[i]->UpdateAdjTriOpPoint();
            }
            oeV[i]->UpdateAdjTriOpPoint(this, op);
        }
    }
}
void point::UpdateAdjTriAndAdjEdge(edge* ne) {/**此函数中1.和2.处可以更新为调用UpdateAdjTri()和UpdateAdjEdge()**/
    // 1. 删邻接三角形，只需要删一个
    /****erase会导致迭代器失效****/
    std::vector<facet*>::iterator delAdjTriAdd = adjTri.end();
    for (auto it = adjTri.begin(); it != adjTri.end(); ++it) {
        if ((*it)->GetDeleted()) {
            delAdjTriAdd = it;
            break;
        }
    }
    if (delAdjTriAdd != adjTri.end()) {
        adjTri.erase(delAdjTriAdd);
    }
    else {
        std::cout << "error: 不可能出现的错误，UpdateAdjTriAndAdjEdge1" << std::endl;
    }
    // 2. 删邻接边，需要删两条边
    std::vector<edge*>::iterator delAdjEdgeAdd = adjEdge.end();
    for (auto it = adjEdge.begin(); it != adjEdge.end(); ++it) {
        if ((*it)->GetDeleted()) {
            delAdjEdgeAdd = it;
            break;
        }
    }
    if (delAdjEdgeAdd != adjEdge.end()) {
        adjEdge.erase(delAdjEdgeAdd); // 此处破坏了迭代器
    }
    else {
        std::cout << "error: 不可能出现的错误，UpdateAdjTriAndAdjEdge2" << std::endl;
    }
    delAdjEdgeAdd = adjEdge.end();
    for (auto it = adjEdge.begin(); it != adjEdge.end(); ++it) {
        if ((*it)->GetDeleted()) {
            delAdjEdgeAdd = it;
            break;
        }
    }
    if (delAdjEdgeAdd != adjEdge.end()) {
        adjEdge.erase(delAdjEdgeAdd); // 此处破坏了迭代器
    }
    else {
        std::cout << "error: 不可能出现的错误，UpdateAdjTriAndAdjEdge3" << std::endl;
    }
    // 3. 加邻接边，只需要加一条
    adjEdge.push_back(ne);
}
void point::UpdateAdjTri() {
    // 删邻接三角形，需要删两个
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
void point::UpdateAdjEdge() {
    // 删邻接边，需要删一个
    std::stack<int> subs;
    for (int i = 0; i < adjEdge.size(); ++i) {
        if (adjEdge[i]->GetDeleted()) {
            subs.push(i);
        }
    }
    while (!subs.empty()) {
        adjEdge.erase(adjEdge.begin() + subs.top());
        subs.pop();
    }
}
void point::UpdateAdjEdgeOpPointData() {
    point* p;
    for (auto e : adjEdge) {
        p = e->GetPointExP(this);
        p->UpdateAdjTriOpEdgeAndAdjTriOpNormal();
    }
}
void point::SetAdjPoint() {
    point* p;
    for (auto e : adjEdge) {
        p = e->GetPointExP(this);
        adjPoint.push_back(p);
    }
}
/*********************************************facet*******************************************/
facet::facet(edge* newE, edge* oldE, point* oldP) {
    // 点
    _p1 = oldP;
    /**注意：由于输出STL时要通过点的顺序来确定normal的方向，因此必须保证点的顺序**/
    std::vector<point*> pOrder1 = oldE->GetAdjTri()[0]->GetPointOrderStartWithP(oldP);
    std::vector<point*> pOrder2 = oldE->GetAdjTri()[1]->GetPointOrderStartWithP(oldP);
    if (pOrder1[1] == pOrder2[2]) {
        _p2 = pOrder2[1];
        _p3 = pOrder1[2];
    }
    else if(pOrder1[2] == pOrder2[1]) {
        _p2 = pOrder1[1];
        _p3 = pOrder2[2];
    }
    else {
        std::cout << "error: 不可能出现的错误，新建facet时错误0" << std::endl;
    }

    // 找到边的索引
    std::vector<edge*> newEV;
    for (edge* e : oldE->GetAdjTriEdges()) {
        if (e->IsContainPoint(oldP)) {
            newEV.push_back(e);
        }
    }
    if (newEV.size() != 2) {
        std::cout << "error: 不可能出现的错误，新建facet时错误1" << std::endl;
    }
    _edge1 = newE;
    _edge2 = newEV[0];
    _edge3 = newEV[1];

    // normal设置为(0,0,0)
    _normal.setXYZ(0, 0, 0);
    /**注意：输出是normal是通过计算得到，所以_normal的存储无用**/

    UpdateMiniInnerAngle();

    faceID = _preFaceID++;
    _isDeleted = false;
}
void facet::SetDeleted() {
    _edge1->SetDeleted();
    _edge2->SetDeleted();
    _edge3->SetDeleted();
    _isDeleted = true;
}
void facet::UpdatePointAndEdge(point* pm_, edge* ne1, edge* ne2) {
    // 更新顶点
    bool _p1Deleted = _p1->GetDeleted(),
        _p2Deleted = _p2->GetDeleted(),
        _p3Deleted = _p3->GetDeleted();
    if (((_p1Deleted == true) && (_p2Deleted == false) && (_p3Deleted == false)) ||
        ((_p1Deleted == false) && (_p2Deleted == true) && (_p3Deleted == false)) ||
        ((_p1Deleted == false) && (_p2Deleted == false) && (_p3Deleted == true))) {
        if (_p1Deleted) {
            _p1 = pm_;
        }
        else if (_p2Deleted) {
            _p2 = pm_;
        }
        else if (_p3Deleted) {
            _p3 = pm_;
        }
    }
    else {
        std::cout << "error: 不可能出现的错误，UpdatePointAndEdge1" << std::endl;
    }
    // 更新边
    bool _edge1Deleted = _edge1->GetDeleted(),
        _edge2Deleted = _edge2->GetDeleted(),
        _edge3Deleted = _edge3->GetDeleted();
    /***边可能删掉两个***/
    point* p1ExPm_ = ne1->GetPointExP(pm_),
        * p2ExPm_ = ne2->GetPointExP(pm_);
    if (_edge1Deleted) {
        if (_edge1->IsContainPoint(p1ExPm_)) {
            _edge1 = ne1;
        }
        else if (_edge1->IsContainPoint(p2ExPm_)) {
            _edge1 = ne2;
        }
        else {
            std::cout << "error: 不可能出现的错误，UpdatePointAndEdge2" << std::endl;
        }
    }
    if (_edge2Deleted) {
        if (_edge2->IsContainPoint(p1ExPm_)) {
            _edge2 = ne1;
        }
        else if (_edge2->IsContainPoint(p2ExPm_)) {
            _edge2 = ne2;
        }
        else {
            std::cout << "error: 不可能出现的错误，UpdatePointAndEdge3" << std::endl;
        }
    }
    if (_edge3Deleted) {
        if (_edge3->IsContainPoint(p1ExPm_)) {
            _edge3 = ne1;
        }
        else if (_edge3->IsContainPoint(p2ExPm_)) {
            _edge3 = ne2;
        }
        else {
            std::cout << "error: 不可能出现的错误，UpdatePointAndEdge4" << std::endl;
        }
    }
    // 更新facet的每条边的
}
edge* facet::GetEdgeOpP(point* p) {
    if ((!_edge1->IsContainPoint(p)) &&
        _edge2->IsContainPoint(p) &&
        _edge3->IsContainPoint(p)) {
        return _edge1;
    }
    else if (_edge1->IsContainPoint(p) &&
        (!_edge2->IsContainPoint(p)) &&
        _edge3->IsContainPoint(p)) {
        return _edge2;
    }
    else if (_edge1->IsContainPoint(p) &&
        _edge2->IsContainPoint(p) &&
        (!_edge3->IsContainPoint(p))) {
        return _edge3;
    }
    else {
        std::cout << "error: 不可能出现的错误，GetEdgeOpP" << std::endl;
        return nullptr;
    }
}
point* facet::GetPointExE(edge* e) {
    point* p1 = e->GetP1(), * p2 = e->GetP2();
    if ((_p3 == p1 && _p2 == p2) || (_p3 == p2 && _p2 == p1)) {
        return _p1;
    }
    else if ((_p1 == p1 && _p3 == p2) || (_p1 == p2 && _p3 == p1)) {
        return _p2;
    }
    else if ((_p1 == p1 && _p2 == p2) || (_p1 == p2 && _p2 == p1)) {
        return _p3;
    }
    else {
        std::cout << "error: 不可能出现的错误，facet::GetPointExE" << std::endl;
        return nullptr;
    }
}
void facet::UpdateMiniInnerAngle() {
    std::vector<double> angles;
    double angle1 = acos((_edge1->GetLength() * _edge1->GetLength() +
        _edge2->GetLength() * _edge2->GetLength() -
        _edge3->GetLength() * _edge3->GetLength()
        ) / (2 * _edge1->GetLength() * _edge2->GetLength()));
    double angle2 = acos((_edge1->GetLength() * _edge1->GetLength() +
        _edge3->GetLength() * _edge3->GetLength() -
        _edge2->GetLength() * _edge2->GetLength()
        ) / (2 * _edge1->GetLength() * _edge3->GetLength()));
    double angle3 = acos((_edge3->GetLength() * _edge3->GetLength() +
        _edge2->GetLength() * _edge2->GetLength() -
        _edge1->GetLength() * _edge1->GetLength()
        ) / (2 * _edge3->GetLength() * _edge2->GetLength()));
    angles.push_back(angle1);
    angles.push_back(angle2);
    angles.push_back(angle3);
    std::sort(angles.begin(), angles.end());
    _miniInnerAngle = angles[0];
}
/*********************************************edge********************************************/
#include "DataStruct.h"
/*********************************************point*******************************************/
void point::AddAdjTri(std::vector<facet*> fV,
    edge* newe1,
    edge* newe2) {
    for (facet* f : fV) {
        if (!f->GetDeleted()) {
            // �˴��õ���Ϊͼ�е���ɫ�������е�һ��
            adjTri.push_back(f);
            // ���������εĶ��������ͱ�����
            f->UpdatePointAndEdge(this, newe1, newe2);
        }
    }
}
void point::UpdateAdjTriOpEdgeAndAdjTriOpNormal() {
    // ɾ�ڽ������ζ�Ӧ������
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
            // �˴��õ���Ϊͼ����ɫ�ߵ�һ��
            adjEdge.push_back(e);
            // �������˵�������ڽӱߵĶ˵��Ϊpm_
            e->ChangeAnotherEndPoint(this, olde);
            // ���±ߵ��ڽ������ε������ߣ��ȸ��µ���ڽ������Σ�
            e->UpdateAdjTriEdgesAndAlpheBeta();
        }
    }
}
void point::AddAdjTriOpEdgeAndAdjTriOpNormal(point* op) {
    std::vector<edge*> oeV = op->GetAdjTriOpEdge();
    std::vector<normal*> onV = op->GetAdjTriOpNormal();
    for (int i = 0; i < oeV.size(); ++i) {
        if (oeV[i]->GetDeleted() == false) {
            // �˴��õ���Ϊ��ɫ���е�һ��
            adjTriOpEdge.push_back(oeV[i]);
            adjTriOpNormal.push_back(onV[i]);
            // ������ɫ�ߵ����ڱߺͶԽ�
            oeV[i]->UpdateAdjTriEdgesAndAlpheBeta();
            // ������ɫ�ߵĶԵ�
            if (oeV[i]->GetAdjTri().size() != oeV[i]->GetAdjTriOpPoint().size()) {
                oeV[i]->UpdateAdjTriOpPoint();
            }
            oeV[i]->UpdateAdjTriOpPoint(this, op);
        }
    }
}
void point::UpdateAdjTriAndAdjEdge(edge* ne) {/**�˺�����1.��2.�����Ը���Ϊ����UpdateAdjTri()��UpdateAdjEdge()**/
    // 1. ɾ�ڽ������Σ�ֻ��Ҫɾһ��
    /****erase�ᵼ�µ�����ʧЧ****/
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
        std::cout << "error: �����ܳ��ֵĴ���UpdateAdjTriAndAdjEdge1" << std::endl;
    }
    // 2. ɾ�ڽӱߣ���Ҫɾ������
    std::vector<edge*>::iterator delAdjEdgeAdd = adjEdge.end();
    for (auto it = adjEdge.begin(); it != adjEdge.end(); ++it) {
        if ((*it)->GetDeleted()) {
            delAdjEdgeAdd = it;
            break;
        }
    }
    if (delAdjEdgeAdd != adjEdge.end()) {
        adjEdge.erase(delAdjEdgeAdd); // �˴��ƻ��˵�����
    }
    else {
        std::cout << "error: �����ܳ��ֵĴ���UpdateAdjTriAndAdjEdge2" << std::endl;
    }
    delAdjEdgeAdd = adjEdge.end();
    for (auto it = adjEdge.begin(); it != adjEdge.end(); ++it) {
        if ((*it)->GetDeleted()) {
            delAdjEdgeAdd = it;
            break;
        }
    }
    if (delAdjEdgeAdd != adjEdge.end()) {
        adjEdge.erase(delAdjEdgeAdd); // �˴��ƻ��˵�����
    }
    else {
        std::cout << "error: �����ܳ��ֵĴ���UpdateAdjTriAndAdjEdge3" << std::endl;
    }
    // 3. ���ڽӱߣ�ֻ��Ҫ��һ��
    adjEdge.push_back(ne);
}
void point::UpdateAdjTri() {
    // ɾ�ڽ������Σ���Ҫɾ����
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
    // ɾ�ڽӱߣ���Ҫɾһ��
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
    // ��
    _p1 = oldP;
    _p2 = newE->GetP1();
    _p3 = newE->GetP2();

    // �ҵ��ߵ�����
    std::vector<edge*> newEV;
    for (edge* e : oldE->GetAdjTriEdges()) {
        if (e->IsContainPoint(oldP)) {
            newEV.push_back(e);
        }
    }
    if (newEV.size() != 2) {
        std::cout << "error: �����ܳ��ֵĴ����½�facetʱ����1" << std::endl;
    }
    _edge1 = newE;
    _edge2 = newEV[0];
    _edge3 = newEV[1];

    // ���㲢����normal
    normal* normal1 = oldE->GetAdjTri()[0]->GetNormalAdd(),
        * normal2 = oldE->GetAdjTri()[1]->GetNormalAdd();
    /*
    double temp1[3], temp2[3];
    double p1x = _p1->GetX(),
        p1y = _p1->GetY(),
        p1z = _p1->GetZ(),
        p2x = _p2->GetX(),
        p2y = _p2->GetY(),
        p2z = _p2->GetZ(),
        p3x = _p3->GetX(),
        p3y = _p3->GetY(),
        p3z = _p3->GetZ();
    temp1[0] = p2x - p1x;
    temp1[1] = p2y - p1y;
    temp1[2] = p2z - p1z;
    temp2[0] = p3x - p2x;
    temp2[1] = p3y - p2y;
    temp2[2] = p3z - p2z;
    double nx = temp1[1] * temp2[2] - temp1[2] * temp2[1],
        ny = -(temp1[0] * temp2[2] - temp1[2] * temp2[0]),
        nz = temp1[0] * temp2[1] - temp1[1] * temp2[0];
    double length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length == 0.0f) { length = 1.0f; }
    _normal.setXYZ(nx / length, ny / length, nz / length);*/
    _normal.setXYZ(normal1->_x, normal1->_y, normal1->_z);
    //if (((_normal._x * ((normal2->_x + normal1->_x)/2)) + (_normal._y * ((normal2->_y + normal1->_y) / 2)) + (_normal._z * ((normal2->_z + normal1->_z) / 2))) < 0) {
    //    _normal.Negate();
    //    /*std::cout << "�۽�1" << std::endl;
    //    if (((_normal._x * normal2->_x) + (_normal._y * normal2->_y) + (_normal._z * normal2->_z)) > 0) {
    //        std::cout << "error: ���1" << std::endl;
    //    }*/
    //}
    /*else {
        if (((_normal._x * normal2->_x) + (_normal._y * normal2->_y) + (_normal._z * normal2->_z)) < 0) {
            std::cout << "error: �۽�2" << std::endl;
        }
    }*/
    /*if ((_normal.GetAngleWithN(*normal1) > 1.5708) && (_normal.GetAngleWithN(*normal2) > 1.5708)) {
        std::cout << "���������" << _normal.GetAngleWithN(*normal1) << "#" << _normal.GetAngleWithN(*normal2) << std::endl;
        _normal.Negate();
    }
    else if ((_normal.GetAngleWithN(*normal1) < 1.5708) && (_normal.GetAngleWithN(*normal2) < 1.5708)) {
        std::cout << "��СС��" << _normal.GetAngleWithN(*normal1) << "#" << _normal.GetAngleWithN(*normal2) << std::endl;
    }
    else {
        std::cout << "��ͬ��" << _normal.GetAngleWithN(*normal1) << "#" << _normal.GetAngleWithN(*normal2) << std::endl;
    }*/
    //if (_normal.GetAngleWithN(*normal1) > 1.5708) { // 1.5708ΪPI/2�Ľ���ֵ
    //    _normal.Negate();
    //    if (_normal.GetAngleWithN(*normal2) < 1.5708) {
    //        std::cout << _normal.GetAngleWithN(*normal1) << "#" << _normal.GetAngleWithN(*normal2) << " ";
    //        //std::cout << "error: �����ܳ��ֵĴ��������нǲ�ͬΪ�۽�" << std::endl;
    //    }
    //}
    //else {
    //    if (_normal.GetAngleWithN(*normal2) > 1.5708) {
    //        std::cout << "error: �����ܳ��ֵĴ��������нǲ�ͬΪ���" << std::endl;
    //    }
    //}

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
    // ���¶���
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
        std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge1" << std::endl;
    }
    // ���±�
    bool _edge1Deleted = _edge1->GetDeleted(),
        _edge2Deleted = _edge2->GetDeleted(),
        _edge3Deleted = _edge3->GetDeleted();
    /***�߿���ɾ������***/
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
            std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge2" << std::endl;
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
            std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge3" << std::endl;
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
            std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge4" << std::endl;
        }
    }
    // ����facet��ÿ���ߵ�
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
        std::cout << "error: �����ܳ��ֵĴ���GetEdgeOpP" << std::endl;
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
        std::cout << "error: �����ܳ��ֵĴ���facet::GetPointExE" << std::endl;
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
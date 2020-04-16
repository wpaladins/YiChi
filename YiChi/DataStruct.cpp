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
        if (!e->GetDeleted()) {
            // �˴��õ���Ϊͼ����ɫ�ߵ�һ��
            adjEdge.push_back(e);
            // �������˵�������ڽӱߵĶ˵��Ϊpm_
            e->ChangeAnotherEndPoint(this, olde);
            // ���±ߵ��ڽ������ε������ߣ��ȸ��µ���ڽ������Σ�
            e->UpdateAdjTriEdgesAndAlpheBeta();
        }
    }
}
void point::AddAdjTriOpEdgeAndAdjTriOpNormal(std::vector<edge*> oeV,
    std::vector<normal*> onV, point* op) {
    for (int i = 0; i < oeV.size(); ++i) {
        if (!oeV[i]->GetDeleted()) {
            // �˴��õ���Ϊ��ɫ���е�һ��
            adjTriOpEdge.push_back(oeV[i]);
            adjTriOpNormal.push_back(onV[i]);
            // ������ɫ�ߵ����ڱߺͶԽ�
            oeV[i]->UpdateAdjTriEdgesAndAlpheBeta();
            // ������ɫ�ߵĶԵ�
            oeV[i]->UpdateAdjTriOpPoint(this, op);
        }
    }
}
void point::UpdateAdjTriAndAdjEdge(edge* ne) {
    // ɾ�ڽ�������
    for (auto it = adjTri.begin(); it != adjTri.end(); ++it) {
        if ((*it)->GetDeleted()) {
            adjTri.erase(it);
        }
    }
    // ɾ�ڽӱ�
    for (auto it = adjEdge.begin(); it != adjEdge.end(); ++it) {
        if ((*it)->GetDeleted()) {
            adjEdge.erase(it);
        }
    }
    // ���ڽӱ�
    adjEdge.push_back(ne);
}
void point::UpdateAdjEdgeOpPointData() {
    point* p;
    for (auto e : adjEdge) {
        p = e->GetPointExP(this);
        p->UpdateAdjTriOpEdgeAndAdjTriOpNormal();
    }
}
/*********************************************facet*******************************************/
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

    point* p1ExPm_ = ne1->GetPointExP(pm_),
        * p2ExPm_ = ne2->GetPointExP(pm_);
    if (((_edge1Deleted == true) && (_edge2Deleted == false) && (_edge3Deleted == false)) ||
        ((_edge1Deleted == false) && (_edge2Deleted == true) && (_edge3Deleted == false)) ||
        ((_edge1Deleted == false) && (_edge2Deleted == false) && (_edge3Deleted == true))) {
        if (_edge1Deleted) {
            if (containPoint(p1ExPm_)) {
                _edge1 = ne1;
            }
            else if (containPoint(p2ExPm_)) {
                _edge1 = ne2;
            }
            else {
                std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge2" << std::endl;
            }
        }
        else if (_edge2Deleted) {
            if (containPoint(p1ExPm_)) {
                _edge2 = ne1;
            }
            else if (containPoint(p2ExPm_)) {
                _edge2 = ne2;
            }
            else {
                std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge3" << std::endl;
            }
        }
        else if (_edge3Deleted) {
            if (containPoint(p1ExPm_)) {
                _edge3 = ne1;
            }
            else if (containPoint(p2ExPm_)) {
                _edge3 = ne2;
            }
            else {
                std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge4" << std::endl;
            }
        }
        else {
            std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge5" << std::endl;
        }
    }
    else if (!((_edge1Deleted == false) && (_edge2Deleted == false) && (_edge3Deleted == false))) {
        std::cout << "error: �����ܳ��ֵĴ���UpdatePointAndEdge6" << std::endl;
    }
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
/*********************************************edge********************************************/
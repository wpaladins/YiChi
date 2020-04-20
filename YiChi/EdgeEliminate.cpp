#pragma warning(disable : 4996)
#include "EdgeEliminate.h"
#include <iostream>

double det[4][4];
// �����Ľ�����ʽ
double calc_det() {
	double
		a1 = det[0][0],
		a2 = det[0][1],
		a3 = det[0][2],
		a4 = det[0][3],
		b1 = det[1][0],
		b2 = det[1][1],
		b3 = det[1][2],
		b4 = det[1][3],
		c1 = det[2][0],
		c2 = det[2][1],
		c3 = det[2][2],
		c4 = det[2][3],
		d1 = det[3][0],
		d2 = det[3][1],
		d3 = det[3][2],
		d4 = det[3][3];
	double result =
		a1 * b2 * c3 * d4 -
		a1 * b2 * c4 * d3 -
		a1 * b3 * c2 * d4 +
		a1 * b3 * c4 * d2 +
		a1 * b4 * c2 * d3 -
		a1 * b4 * c3 * d2 -
		a2 * b1 * c3 * d4 +
		a2 * b1 * c4 * d3 +
		a2 * b3 * c1 * d4 -
		a2 * b3 * c4 * d1 -
		a2 * b4 * c1 * d3 +
		a2 * b4 * c3 * d1 +
		a3 * b1 * c2 * d4 -
		a3 * b1 * c4 * d2 -
		a3 * b2 * c1 * d4 +
		a3 * b2 * c4 * d1 +
		a3 * b4 * c1 * d2 -
		a3 * b4 * c2 * d1 -
		a4 * b1 * c2 * d3 +
		a4 * b1 * c3 * d2 +
		a4 * b2 * c1 * d3 -
		a4 * b2 * c3 * d1 -
		a4 * b3 * c1 * d2 +
		a4 * b3 * c2 * d1;
	return result;
}

// ����һԪ���η���
double calc_distance(double x1, double y1, double z1, double x2, double y2, double z2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

void EdgeEliminate() {
	times = 0;
	for (int i = 0; i < mesh.edges.size(); ++i) {
		edge* e = mesh.edges[i];
		point* ep1 = e->GetP1();
		point* ep2 = e->GetP2();
		// ������Ѿ���ɾ���ˣ�������
		if(e->GetDeleted()){
			continue;
		}
		// ����ߵ������˵���һ���ڱ߽��ϣ�������
		if ((ep1->GetAdjTri().size() != ep1->GetAdjEdge().size()) ||
			(ep2->GetAdjTri().size() != ep2->GetAdjEdge().size())) {
			continue;
		}
		// ����ߵ������˵���ڽ������ε���ĿС��3��������
		if (ep1->GetAdjTri().size() < 3 || ep2->GetAdjTri().size() < 3) {
			continue;
		}
		double he = e->calc_he();
		if (he != 0 && he < 1/sqrt(2)) {
			double curvatureWeight = mesh.edges[i]->calc_curvature_weight();
			if (curvatureWeight > 3) { // ������ֵ
				// ���б�����

				times++;
				
				// ��֯����������ʽ
				// 0. ��ȡֵ
				if (e->GetAdjTri().size() != e->GetAdjTriOpPoint().size()) {
					e->UpdateAdjTriOpPoint();
				}
				double
					x1 = ep1->GetX(),
					y1 = ep1->GetY(),
					z1 = ep1->GetZ(),
					x2 = ep2->GetX(),
					y2 = ep2->GetY(),
					z2 = ep2->GetZ(),
					x3 = e->GetAdjTriOpPoint()[e->GetAdjTri()[0]]->GetX(),
					y3 = e->GetAdjTriOpPoint()[e->GetAdjTri()[0]]->GetY(),
					z3 = e->GetAdjTriOpPoint()[e->GetAdjTri()[0]]->GetZ(),
					x4 = e->GetAdjTriOpPoint()[e->GetAdjTri()[1]]->GetX(),
					y4 = e->GetAdjTriOpPoint()[e->GetAdjTri()[1]]->GetY(),
					z4 = e->GetAdjTriOpPoint()[e->GetAdjTri()[1]]->GetZ(),
					t1 = x1 * x1 + y1 * y1 + z1 * z1,
					t2 = x2 * x2 + y2 * y2 + z2 * z2,
					t3 = x3 * x3 + y3 * y3 + z3 * z3,
					t4 = x4 * x4 + y4 * y4 + z4 * z4;
				// 1. x^2+y^2+z^2��ϵ��A
				det[0][0] = x1;
				det[1][0] = x2;
				det[2][0] = x3;
				det[3][0] = x4;
				det[0][1] = y1;
				det[1][1] = y2;
				det[2][1] = y3;
				det[3][1] = y4;
				det[0][2] = z1;
				det[1][2] = z2;
				det[2][2] = z3;
				det[3][2] = z4;
				det[0][3] = 1;
				det[1][3] = 1;
				det[2][3] = 1;
				det[3][3] = 1;
				// debug 
				/*det[0][0] = -8.53;
				det[0][1] = -8.16;
				det[0][2] = 5.34;
				det[0][3] = 1;
				det[1][0] = -8.54;
				det[1][1] = -8.08;
				det[1][2] = 5.33;
				det[1][3] = 1;
				det[2][0] = -8.41;
				det[2][1] = -8.14;
				det[2][2] = 5.18;
				det[2][3] = 1;
				det[3][0] = -8.64;
				det[3][1] = -8.08;
				det[3][2] = 5.40;
				det[3][3] = 1;*/
				double A = calc_det(); //debug A = [-8.53 -8.16 5.34 1;-8.54 -8.08 5.33 1;-8.41 -8.14 5.18 1;-8.64 -8.08 5.40 1]
				// 2. x��ϵ��B
				det[0][0] = t1;
				det[1][0] = t2;
				det[2][0] = t3;
				det[3][0] = t4;
				det[0][1] = y1;
				det[1][1] = y2;
				det[2][1] = y3;
				det[3][1] = y4;
				det[0][2] = z1;
				det[1][2] = z2;
				det[2][2] = z3;
				det[3][2] = z4;
				det[0][3] = 1;
				det[1][3] = 1;
				det[2][3] = 1;
				det[3][3] = 1;
				double B = calc_det();
				// 3. y��ϵ��C
				det[0][0] = t1;
				det[1][0] = t2;
				det[2][0] = t3;
				det[3][0] = t4;
				det[0][1] = x1;
				det[1][1] = x2;
				det[2][1] = x3;
				det[3][1] = x4;
				det[0][2] = z1;
				det[1][2] = z2;
				det[2][2] = z3;
				det[3][2] = z4;
				det[0][3] = 1;
				det[1][3] = 1;
				det[2][3] = 1;
				det[3][3] = 1;
				double C = calc_det();
				// 4. z��ϵ��D
				det[0][0] = t1;
				det[1][0] = t2;
				det[2][0] = t3;
				det[3][0] = t4;
				det[0][1] = x1;
				det[1][1] = x2;
				det[2][1] = x3;
				det[3][1] = x4;
				det[0][2] = y1;
				det[1][2] = y2;
				det[2][2] = y3;
				det[3][2] = y4;
				det[0][3] = 1;
				det[1][3] = 1;
				det[2][3] = 1;
				det[3][3] = 1;
				double D = calc_det();
				// 5. ������E
				det[0][0] = t1;
				det[1][0] = t2;
				det[2][0] = t3;
				det[3][0] = t4;
				det[0][1] = x1;
				det[1][1] = x2;
				det[2][1] = x3;
				det[3][1] = x4;
				det[0][2] = y1;
				det[1][2] = y2;
				det[2][2] = y3;
				det[3][2] = y4;
				det[0][3] = z1;
				det[1][3] = z2;
				det[2][3] = z3;
				det[3][3] = z4;
				double E = calc_det();
				
				// ����e���е�Pm��Pm���ķ�����
				std::vector<edge*> adjTriOpEdge1 = ep1->GetAdjTriOpEdge(),
					adjTriOpEdge2 = ep2->GetAdjTriOpEdge();
				std::vector<normal*> adjTriOpNormal1 = ep1->GetAdjTriOpNormal(),
					adjTriOpNormal2 = ep2->GetAdjTriOpNormal();

				if (adjTriOpEdge1.size() == 0 || adjTriOpEdge2.size() == 0) {
					std::cout << "error: �����ܳ��ֵĴ��󣬶�����ĿΪ0" << std::endl;
				}
				if (adjTriOpEdge1.size() != adjTriOpNormal1.size() || adjTriOpEdge2.size() != adjTriOpNormal2.size()) {
					std::cout << "error: �����ܳ��ֵĴ��󣬱���������������" << std::endl;
				}
				// std::cout << adjTriOpEdge1.size() << " " << adjTriOpEdge2.size() << " ";
				double lengthSum1 = .0, lengthSum2 = .0,
					lengthMultiNjX1 = .0,
					lengthMultiNjY1 = .0,
					lengthMultiNjZ1 = .0,
					lengthMultiNjX2 = .0,
					lengthMultiNjY2 = .0,
					lengthMultiNjZ2 = .0;
				for (int i = 0; i < adjTriOpEdge1.size(); ++i) {
					lengthSum1 += adjTriOpEdge1[i]->GetLength();
					lengthMultiNjX1 += adjTriOpEdge1[i]->GetLength() * adjTriOpNormal1[i]->_x;
					lengthMultiNjY1 += adjTriOpEdge1[i]->GetLength() * adjTriOpNormal1[i]->_y;
					lengthMultiNjZ1 += adjTriOpEdge1[i]->GetLength() * adjTriOpNormal1[i]->_z;
				}
				double N1X = lengthMultiNjX1 / lengthSum1,
					N1Y = lengthMultiNjY1 / lengthSum1,
					N1Z = lengthMultiNjZ1 / lengthSum1;
				for (int i = 0; i < adjTriOpEdge2.size(); ++i) {
					lengthSum2 += adjTriOpEdge2[i]->GetLength();
					lengthMultiNjX2 += adjTriOpEdge2[i]->GetLength() * adjTriOpNormal2[i]->_x;
					lengthMultiNjY2 += adjTriOpEdge2[i]->GetLength() * adjTriOpNormal2[i]->_y;
					lengthMultiNjZ2 += adjTriOpEdge2[i]->GetLength() * adjTriOpNormal2[i]->_z;
				}
				double N2X = lengthMultiNjX2 / lengthSum2,
					N2Y = lengthMultiNjY2 / lengthSum2,
					N2Z = lengthMultiNjZ2 / lengthSum2;
				// ����P1���P2��ķ�����
				double xm = (x1 + x2) / 2,
					ym = (y1 + y2) / 2,
					zm = (z1 + z2) / 2,
					xn = (N1X + N2X) / 2,
					yn = (N1Y + N2Y) / 2,
					zn = (N1Z + N2Z) / 2;
				// std::cout << std::endl;
				
				// ��һԪ���η��̵�ϵ���ͳ���
				double a = A * (xn * xn + yn * yn + zn * zn),
					b = 2 * xm * xn +2 * ym * yn + 2 * zm * zn - B * xn + C * yn - D * zn ,
					c = A * xm * xm + A * ym * ym + A * zm * zm - B * xm + C * ym - D * zm + E;
				// ��һԪ���η��̣��󽻵�
				double delte = b * b - 4 * a * c, result1 = 0, result2 = 0;
				if (delte <= 0) {
					std::cout << "error: �����ܳ��ֵ��������������������ȵ�ʵ����" << std::endl;
				}
				else {
					result1 = (-b + sqrt(delte)) / (2 * a);
					result2 = (-b - sqrt(delte)) / (2 * a);
				}
				// ����t1��t2���������
				double pm_1_x = xm + result1 * xn,
					pm_1_y = ym + result1 * yn,
					pm_1_z = zm + result1 * zn,
					pm_2_x = xm + result2 * xn,
					pm_2_y = ym + result2 * yn,
					pm_2_z = zm + result2 * zn,
					pm_x, pm_y, pm_z;
				// �����pm_1��pm_2�������о���pm�����Ǹ���
				if (calc_distance(xm,ym,zm,pm_1_x, pm_1_y, pm_1_y) < calc_distance(xm, ym, zm, pm_2_x, pm_2_y, pm_2_y)) {
					pm_x = pm_1_x;
					pm_y = pm_1_y;
					pm_z = pm_1_z;
				}
				else {
					pm_x = pm_2_x;
					pm_y = pm_2_y;
					pm_z = pm_2_z;
				}

				// ���������Ĵ���
				// 0. ����µĽṹ
					// 0.1. ��
				point* pm_ = new point(pm_x, pm_y, pm_z);
					// 0.2. ��
				edge* ne1 = new edge(pm_, e->GetAdjTriOpPoint()[e->GetAdjTri()[0]]),
					* ne2 = new edge(pm_, e->GetAdjTriOpPoint()[e->GetAdjTri()[1]]);

				// 1. ɾ��
					// 1.1. ɾ�ڽ������Σ������ڽ������ε����бߣ�
				e->GetAdjTri()[0]->SetDeleted();// 1.2. ɾ�Լ�e->SetDeleted();
				e->GetAdjTri()[1]->SetDeleted();
					// 1.3. ɾ�˵�
				ep1->SetDeleted();
				ep2->SetDeleted();

				// 2. ���
					// 2.1. Ϊpm_������ݽṹ�е�����ֵ
				pm_->AddAdjTri(ep1->GetAdjTri(), ne1, ne2);
				pm_->AddAdjTri(ep2->GetAdjTri(), ne1, ne2);
					// �����λ��֮ǰ���Ѿ������������θ������
				pm_->AddAdjEdge(ep1->GetAdjEdge(), e); /********�������ڽ������κ��ڽӱߵ���Ŀ����ȣ�˵���õ�Ϊ�߽��********/
				pm_->AddAdjEdge(ep2->GetAdjEdge(), e);
					// �����λ��֮ǰ���Ѿ���������ɫ�ı�����Ҫ���µ����ݸ������
				pm_->AddAdjEdge(ne1);
				pm_->AddAdjEdge(ne2); // �ڽӱߣ������±ߣ�

				pm_->AddAdjTriOpEdgeAndAdjTriOpNormal(ep1);
				pm_->AddAdjTriOpEdgeAndAdjTriOpNormal(ep2);
					// ע�⣺������֮ǰ��ne1��ne2��ֻ�������˵����ʹ��
					// 2.2. Ϊne1��ne2������ݽṹ�е�����ֵ
				ne1->AddAll(e->GetAdjTri()[0]->GetEdgesExE(e, e), ne1);
				ne2->AddAll(e->GetAdjTri()[1]->GetEdgesExE(e, e), ne2);

				// 3. ���������ܵ�Ӱ�������
				// �ۺ�ɫ��1�Ų�����2�Ų���
				point* p1ExPm_ = ne1->GetPointExP(pm_),
					* p2ExPm_ = ne2->GetPointExP(pm_);
				p1ExPm_->UpdateAdjTriAndAdjEdge(ne1);
				p2ExPm_->UpdateAdjTriAndAdjEdge(ne2);
				// �ۺ�ɫ��3�Ų���
				pm_->UpdateAdjEdgeOpPointData();

				// ���½��ĵ�ͱ߼��뵽ȫ�ֱ����ļ�����
				mesh.pointsTemp.push_back(pm_);
				mesh.edgesTemp.push_back(ne1);
				mesh.edgesTemp.push_back(ne2);

				// debug���STL
				/*char buff[100], buffT[100];
				strcpy(buff, "EdgeEliminateDebug");
				sprintf(buffT, "%d", times);
				strcat(buff, buffT);
				OutputSTL("H:\\YJ\\bysj\\rec\\", buff);*/
			}//if
		}
	}
	// ����ɾ������mesh.points��mesh.edges���Ѿ�ɾ����ͱ߽�������ɾ��
	// ���ݺϲ���mesh.pointsTemp��mesh.edgesTemp�е����ݺϲ���mesh.points��mesh.edges��
	mesh.DataDeleteAndDataMerge1();
}
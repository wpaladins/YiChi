#include "EdgeEliminate.h"
#include <iostream>

//double calc_he(edge &e) {
//	double le = e. // e的长度
//}

double det[4][4];
// 计算四阶行列式
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

void EdgeEliminate() {
	for (int i = 0; i < mesh.edges.size(); ++i) {
		double he = mesh.edges[i]->calc_he();
		if (he != 0 && he < 1/sqrt(2)) {
			double curvatureWeight = mesh.edges[i]->calc_curvature_weight();
			if (curvatureWeight > 5) { // 特征阈值
				// 进行边消除
				edge* e = mesh.edges[i];

				// 组织并计算行列式
				// 0. 获取值
				double
					x1 = e->GetP1()->GetX(),
					y1 = e->GetP1()->GetY(),
					z1 = e->GetP1()->GetZ(),
					x2 = e->GetP2()->GetX(),
					y2 = e->GetP2()->GetY(),
					z2 = e->GetP2()->GetY(),
					x3 = e->GetAdjTriPoint()[0]->GetX(),
					y3 = e->GetAdjTriPoint()[0]->GetY(),
					z3 = e->GetAdjTriPoint()[0]->GetZ(),
					x4 = e->GetAdjTriPoint()[1]->GetX(),
					y4 = e->GetAdjTriPoint()[1]->GetY(),
					z4 = e->GetAdjTriPoint()[1]->GetZ(),
					t1 = x1 * x1 + y1 * y1 + z1 * z1,
					t2 = x2 * x2 + y2 * y2 + z2 * z2,
					t3 = x3 * x3 + y3 * y3 + z3 * z3,
					t4 = x4 * x4 + y4 * y4 + z4 * z4;
				// 1. x^2+y^2+z^2的系数A
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
				double A = calc_det();
				// 2. x的系数B
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
				// 3. y的系数C
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
				// 4. z的系数D
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
				// 5. 常数项E
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
				
				// 计算e的中点Pm和Pm处的法向量
				// 计算P1点和P2点的法向量
				double xm = (x1 + x2) / 2,
					ym = (y1 + y2) / 2,
					zm = (z1 + z2) / 2;
					//xn = () / 2,
					//yn = () / 2,
					//zn = () / 2,
			}
		}
	}
}
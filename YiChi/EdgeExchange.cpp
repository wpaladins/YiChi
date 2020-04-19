#pragma warning(disable : 4996)
#include "EdgeExchange.h"
#include <iostream>

void EdgeExchange() {
	// �����ȱ����������������ε���С�ڽǣ�������
	mesh.UpdateFacetsMiniInnerAngle();

	for (int i = 0; i < mesh.edges.size(); ++i) {
		edge* e = mesh.edges[i];
		point* ep1 = e->GetP1();
		point* ep2 = e->GetP2();

		// ɾ���ı߶����Ѿ��������ģ��������жϱ��Ƿ��Ѿ�ɾ��

		// ����ߵ��ڽ������ε���Ŀ��Ϊ2��������
		if (e->GetAdjTri().size() != 2) {
			continue;
		}

		// ����ߵ��ڽ������ε���С�ڽ�
		/*
		�����ȱ����������������ε���С�ڽǣ����������������Ҫ�ظ����������ε���С�ڽǣ�
		���ڸ��±ߵ��ڽ������ε�ʱ��Ը����ݽ��и���
		*/
		std::vector<double> miniInnerAnglesOfAdjTri;
		for (auto f: e->GetAdjTri()) {
			miniInnerAnglesOfAdjTri.push_back(f->GetMiniInnerAngle());
		}
		sort(miniInnerAnglesOfAdjTri.begin(), miniInnerAnglesOfAdjTri.end());
		double miniInnerAnglesOfE = miniInnerAnglesOfAdjTri[0];
		miniInnerAnglesOfAdjTri.clear();


		// ������ʱ�Ķ�ż�߲������ż���ڽ������ε���С�ڽ�
		edge* dualE = new edge(e->GetAdjTri()[0]->GetPointExE(e), e->GetAdjTri()[1]->GetPointExE(e));
		
		facet* dualEAdjTri1 = new facet(dualE, e, ep1),
			* dualEAdjTri2 = new facet(dualE, e, ep2);
		
		miniInnerAnglesOfAdjTri.push_back(dualEAdjTri1->GetMiniInnerAngle());
		miniInnerAnglesOfAdjTri.push_back(dualEAdjTri2->GetMiniInnerAngle());
		sort(miniInnerAnglesOfAdjTri.begin(), miniInnerAnglesOfAdjTri.end());
		double miniInnerAnglesOfDualE = miniInnerAnglesOfAdjTri[0];

		// �����ż���ڽ������ε���С�ڽ�>��ǰ�ߵ��ڽ������ε���С�ڽǣ�����б߽���
		if (miniInnerAnglesOfDualE > miniInnerAnglesOfE) {
			// ���б߽���

			times++;

			// ��e��e���ڽ������α��Ϊɾ��
			e->SetDeleted();
			e->GetAdjTri()[0]->SetSelfDeleted();/* ע�⣺������������ǵ��õĲ�ͬ��ֻɾ���ڽ������α���*/
			e->GetAdjTri()[1]->SetSelfDeleted();

			// dualE�����˲�ȫ
			dualE->AddAdjTri(dualEAdjTri1);
			dualE->AddAdjTri(dualEAdjTri2);
			dualE->AddAdjTriEdge(e->GetAdjTriEdges());
			/**ע�⣺alpheBeta��adjTriOpPoint��û�и��£���Ϊ����Ӧ���Ѿ�û������**/

			// �ۺ�ɫ������˸���
			// 1> �ڽ�������
				// 1.1> ɾ
			ep1->UpdateAdjTri();
			ep2->UpdateAdjTri();
			dualE->GetP1()->UpdateAdjTri();
			dualE->GetP2()->UpdateAdjTri();
				// 1.2> ��
			ep1->AddAdjTri(dualEAdjTri1);
			ep2->AddAdjTri(dualEAdjTri2);
			dualE->GetP1()->AddAdjTri(dualEAdjTri1);
			dualE->GetP1()->AddAdjTri(dualEAdjTri2);
			dualE->GetP2()->AddAdjTri(dualEAdjTri1);
			dualE->GetP2()->AddAdjTri(dualEAdjTri2);
			// 2> �ڽӱ�
			ep1->UpdateAdjEdge();
			ep2->UpdateAdjEdge();
			dualE->GetP1()->AddAdjEdge(dualE);
			dualE->GetP2()->AddAdjEdge(dualE);
			/**ע�⣺adjTriOpEdge��adjTriOpNormal��û�и��£���Ϊ����Ӧ���Ѿ�û������**/

			// ��ɫ�ߵ����˸���
			// 1> �ڽ�������
			for (edge* e: dualE->GetAdjTriEdges()) {
				e->UpdateAdjTri();
				if (e->IsContainPoint(ep1)) {
					e->AddAdjTri(dualEAdjTri1);
				}
				else if (e->IsContainPoint(ep2)) {
					e->AddAdjTri(dualEAdjTri2);
				}
				else {
					std::cout << "error: �����ܳ��ֵĴ���e���ڽ������ε��ıߵ��ڽ���������û�а���e�������˵�" << std::endl;
				}
			}
			// 2> �ڽ��������е�����������
			for (edge* e : dualE->GetAdjTriEdges()) {
				e->UpdateAdjTriEdges();
			}
			/**ע�⣺alpheBeta��adjTriOpPoint��û�и��£���Ϊ����Ӧ���Ѿ�û������**/
		
			// ���½��ıߺͼ��뵽ȫ�ֱ����ļ�����
			mesh.edgesTemp.push_back(dualE);
			mesh.facetsTemp.push_back(dualEAdjTri1);
			mesh.facetsTemp.push_back(dualEAdjTri2);

			// debug���STL
			/*char buff[100], buffT[100];
			strcpy(buff, "EdgeExchangeDebug");
			sprintf(buffT, "%d", times);
			strcat(buff, buffT);
			OutputSTL("H:\\YJ\\bysj\\rec\\", buff);*/
		}
		else {
			delete dualE;
			delete dualEAdjTri1;
			delete dualEAdjTri2;
		}
	}
	mesh.DataDeleteAndDataMerge2();
}
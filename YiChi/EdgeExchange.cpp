#pragma warning(disable : 4996)
#include "EdgeExchange.h"
#include <iostream>

void EdgeExchange() {
	// 可以先遍历计算所有三角形的最小内角，存起来
	mesh.UpdateFacetsMiniInnerAngle();

	for (int i = 0; i < mesh.edges.size(); ++i) {
		edge* e = mesh.edges[i];
		point* ep1 = e->GetP1();
		point* ep2 = e->GetP2();

		// 删除的边都是已经遍历过的，故无需判断边是否已经删除

		// 如果边的邻接三角形的数目不为2，就跳过
		if (e->GetAdjTri().size() != 2) {
			continue;
		}

		// 计算边的邻接三角形的最小内角
		/*
		可以先遍历计算所有三角形的最小内角，存起来（否则就需要重复计算三角形的最小内角）
		并在更新边的邻接三角形的时候对该数据进行更新
		*/
		std::vector<double> miniInnerAnglesOfAdjTri;
		for (auto f: e->GetAdjTri()) {
			miniInnerAnglesOfAdjTri.push_back(f->GetMiniInnerAngle());
		}
		sort(miniInnerAnglesOfAdjTri.begin(), miniInnerAnglesOfAdjTri.end());
		double miniInnerAnglesOfE = miniInnerAnglesOfAdjTri[0];
		miniInnerAnglesOfAdjTri.clear();


		// 构造临时的对偶边并计算对偶边邻接三角形的最小内角
		edge* dualE = new edge(e->GetAdjTri()[0]->GetPointExE(e), e->GetAdjTri()[1]->GetPointExE(e));
		
		facet* dualEAdjTri1 = new facet(dualE, e, ep1),
			* dualEAdjTri2 = new facet(dualE, e, ep2);
		
		miniInnerAnglesOfAdjTri.push_back(dualEAdjTri1->GetMiniInnerAngle());
		miniInnerAnglesOfAdjTri.push_back(dualEAdjTri2->GetMiniInnerAngle());
		sort(miniInnerAnglesOfAdjTri.begin(), miniInnerAnglesOfAdjTri.end());
		double miniInnerAnglesOfDualE = miniInnerAnglesOfAdjTri[0];

		// 如果对偶边邻接三角形的最小内角>当前边的邻接三角形的最小内角，则进行边交换
		if (miniInnerAnglesOfDualE > miniInnerAnglesOfE) {
			// 进行边交换

			times++;

			// 将e和e的邻接三角形标记为删除
			e->SetDeleted();
			e->GetAdjTri()[0]->SetSelfDeleted();/* 注意：这里与边消除是调用的不同，只删除邻接三角形本身*/
			e->GetAdjTri()[1]->SetSelfDeleted();

			// dualE的拓扑补全
			dualE->AddAdjTri(dualEAdjTri1);
			dualE->AddAdjTri(dualEAdjTri2);
			dualE->AddAdjTriEdge(e->GetAdjTriEdges());
			/**注意：alpheBeta和adjTriOpPoint并没有更新，因为它们应该已经没有用了**/

			// 粉红色点的拓扑更新
			// 1> 邻接三角形
				// 1.1> 删
			ep1->UpdateAdjTri();
			ep2->UpdateAdjTri();
			dualE->GetP1()->UpdateAdjTri();
			dualE->GetP2()->UpdateAdjTri();
				// 1.2> 加
			ep1->AddAdjTri(dualEAdjTri1);
			ep2->AddAdjTri(dualEAdjTri2);
			dualE->GetP1()->AddAdjTri(dualEAdjTri1);
			dualE->GetP1()->AddAdjTri(dualEAdjTri2);
			dualE->GetP2()->AddAdjTri(dualEAdjTri1);
			dualE->GetP2()->AddAdjTri(dualEAdjTri2);
			// 2> 邻接边
			ep1->UpdateAdjEdge();
			ep2->UpdateAdjEdge();
			dualE->GetP1()->AddAdjEdge(dualE);
			dualE->GetP2()->AddAdjEdge(dualE);
			/**注意：adjTriOpEdge和adjTriOpNormal并没有更新，因为它们应该已经没有用了**/

			// 蓝色边的拓扑更新
			// 1> 邻接三角形
			for (edge* e: dualE->GetAdjTriEdges()) {
				e->UpdateAdjTri();
				if (e->IsContainPoint(ep1)) {
					e->AddAdjTri(dualEAdjTri1);
				}
				else if (e->IsContainPoint(ep2)) {
					e->AddAdjTri(dualEAdjTri2);
				}
				else {
					std::cout << "error: 不可能出现的错误，e的邻接三角形的四边的邻接三角形中没有包含e的两个端点" << std::endl;
				}
			}
			// 2> 邻接三角形中的其他四条边
			for (edge* e : dualE->GetAdjTriEdges()) {
				e->UpdateAdjTriEdges();
			}
			/**注意：alpheBeta和adjTriOpPoint并没有更新，因为它们应该已经没有用了**/
		
			// 将新建的边和加入到全局变量的集合中
			mesh.edgesTemp.push_back(dualE);
			mesh.facetsTemp.push_back(dualEAdjTri1);
			mesh.facetsTemp.push_back(dualEAdjTri2);

			// debug输出STL
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
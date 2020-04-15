// YiChi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include "OpenSTL.h"
#include "DataStruct.h"
#include "AVL.h"
#include "EdgeAVL.h" // EdgeAVL
//AVLTree--- #include "AVLTree.h"

using namespace std;

triangle_mesh mesh;
int point::_prePointID = 0;
int facet::_preFaceID = 0;
BSTree root = NULL;
eBSTree eRoot = NULL; // EdgeAVL
//AVLTree--- AVLTree<edge, edge> edgeAVLTree;

int main()
{
    if (ReadSTLFile("H:\\YJ\\bysj\\rec\\DInlay1.stl")) {
        std::cout << "成功" << std::endl;
        std::cout << "顶点数：" << mesh.points.size() << std::endl;
        std::cout << "网格数：" << mesh.facets.size() << std::endl;
        std::cout << "边数：" << mesh.edges.size() << std::endl;


        //for (int i = 0; i < mesh.points.size(); ++i) {
        //    std::cout << "point_id:" << mesh.points[i].GetID() << " x:" << mesh.points[i].GetX() << " y:" << mesh.points[i].GetY() << " z:" << mesh.points[i].GetZ() << std::endl;
        //}
        //for (int i = 0; i < mesh.facets.size(); ++i) {
        //    std::cout << "facet_id:" << mesh.facets[i].GetID() << std::endl;
        //}
    }
}
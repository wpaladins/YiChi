// YiChi.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#pragma warning(disable : 4244)
#include <iostream>
#include <vector>
#include "OpenSTL.h"
#include "DataStruct.h"
#include "AVL.h"
#include "EdgeAVL.h"
#include "EdgeEliminate.h"
#include "EdgeExchange.h"

using namespace std;

triangle_mesh mesh;
int point::_prePointID = 0;
int facet::_preFaceID = 0;
BSTree root = NULL;
eBSTree eRoot = NULL;
int times = 0;

void OutputSTL(const char* pathname, const char* filename);

int main()
{
    int oPointsSize, oFacetsSize, oEdgesSize;
    if (ReadBinary("H:\\YJ\\bysj\\rec\\EdgeEliminateOut.stl")) {
        oPointsSize = mesh.points.size();
        oFacetsSize = mesh.facets.size();
        oEdgesSize = mesh.edges.size();
        std::cout << "数据读成功" << " "
            << "顶点数：" << oPointsSize << " "
            << "网格数：" << oFacetsSize << " "
            << "边数：" << oEdgesSize << std::endl;
        // mesh.PrintEdgeLength();
        // mesh.PrintNoBoundPointAdjTriAmount();
        // OutputSTL("H:\\YJ\\bysj\\rec\\", "out1");
        
        // 边消除调用
        /*EdgeEliminate();
        std::cout << "边消除成功" << " "
            << "顶点减少了" << oPointsSize - mesh.points.size() << "个 "
            << "网格减少了" << oFacetsSize - mesh.facets.size() << "个 "
            << "边数减少了" << oEdgesSize - mesh.edges.size() << "个 " << std::endl;*/
        
        // 边交换调用
        EdgeExchange();
        std::cout << "边交换成功" << " "
            << "边交换了" << mesh.EdgeExchangeTimes << "次 " << std::endl;
        
        /// 输出为STL进行查看
        OutputSTL("H:\\YJ\\bysj\\rec\\", "FinnalOut");
    }
}

void OutputSTL(const char* pathname, const char* filename)
{
    // 存为Binary格式
    int m_TriaNum = mesh.facets.size();

    char* savename = new char[100];
    sprintf(savename, "%s%s.stl", pathname, filename);

    char* fileInf = new char[200];
    sprintf(fileInf, "solid %s.stl  %s", filename, "by wpaladins");

    FILE* fp = fopen(savename, "wb");
    delete[] savename;

    float* dat = new float[12];

    fwrite(fileInf, sizeof(char), 80, fp);
    fwrite(&m_TriaNum, sizeof(int), 1, fp); // 三角片数目

    for (int i = 0; i < m_TriaNum; i++)
    {
        normal* nAdd = mesh.facets[i]->GetNormalAdd();
        dat[0] = nAdd->_x;
        dat[1] = nAdd->_y;
        dat[2] = nAdd->_z;

        vector<point*> pointsAdd = mesh.facets[i]->GetPoints();

        dat[3] = pointsAdd[0]->GetX();
        dat[4] = pointsAdd[0]->GetY();
        dat[5] = pointsAdd[0]->GetZ();

        dat[6] = pointsAdd[1]->GetX();
        dat[7] = pointsAdd[1]->GetY();
        dat[8] = pointsAdd[1]->GetZ();

        dat[9] = pointsAdd[2]->GetX();
        dat[10] = pointsAdd[2]->GetY();
        dat[11] = pointsAdd[2]->GetZ();

        fwrite(dat, sizeof(float), 12, fp);
        fwrite("wl", sizeof(char), 2, fp);
    }

    fclose(fp);

    delete[] dat;
    delete[] fileInf;
}
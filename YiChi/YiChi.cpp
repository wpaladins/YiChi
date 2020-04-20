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

    /*注意：当前代码先进行边消除，再进行边交换，一次运行即可完成*/
    const char inputSTLPathAndFile[] = "H:\\YJ\\bysj\\rec\\input\\test1.stl"; // 输入的STL文件
    const char outputSTLPath[] = "H:\\YJ\\bysj\\rec\\"; // 需要输出STL文件的目录

    if (ReadBinary(inputSTLPathAndFile)) {
        oPointsSize = mesh.points.size();
        oFacetsSize = mesh.facets.size();
        oEdgesSize = mesh.edges.size();
        std::cout << "数据读成功" << " "
            << "顶点数：" << oPointsSize << " "
            << "网格数：" << oFacetsSize << " "
            << "边数：" << oEdgesSize << std::endl;
        // mesh.PrintEdgeLength();
        // mesh.PrintNoBoundPointAdjTriAmount();
        // OutputSTL("H:\\YJ\\bysj\\rec\\", "out-debug");
        
        // 边消除调用
        EdgeEliminate();
        std::cout << "边消除成功" << " "
            << "顶点减少了" << oPointsSize - mesh.points.size() << "个 "
            << "网格减少了" << oFacetsSize - mesh.facets.size() << "个 "
            << "边数减少了" << oEdgesSize - mesh.edges.size() << "个 " << std::endl;
        OutputSTL(outputSTLPath, "EdgeEliminateOut");
        mesh.clear();
        char buff[100];
        strcpy(buff, outputSTLPath);
        strcat(buff, "EdgeEliminateOut.stl");
        ReadBinary(buff);
        
        // 边交换调用
        EdgeExchange();
        std::cout << "边交换成功" << " "
            << "边交换了" << mesh.EdgeExchangeTimes << "次 " << std::endl;
        
        /// 输出为STL进行查看
        OutputSTL(outputSTLPath, "FinnalOut");
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
        vector<point*> pointsAdd = mesh.facets[i]->GetPoints();
        double v1x = pointsAdd[0]->GetX(),
            v1y = pointsAdd[0]->GetY(),
            v1z = pointsAdd[0]->GetZ(),
            v2x = pointsAdd[1]->GetX(),
            v2y = pointsAdd[1]->GetY(),
            v2z = pointsAdd[1]->GetZ(),
            v3x = pointsAdd[2]->GetX(),
            v3y = pointsAdd[2]->GetY(),
            v3z = pointsAdd[2]->GetZ();
        double nx = (v1y - v3y) * (v2z - v3z) - (v1z - v3z) * (v2y - v3y),
            ny = (v1z - v3z) * (v2x - v3x) - (v2z - v3z) * (v1x - v3x),
            nz = (v1x - v3x) * (v2y - v3y) - (v2x - v3x) * (v1y - v3y);
        double length = sqrt(nx * nx + ny * ny + nz * nz);
        
        dat[0] = nx / length;
        dat[1] = ny / length;
        dat[2] = nz / length;

        dat[3] = v1x;
        dat[4] = v1y;
        dat[5] = v1z;

        dat[6] = v2x;
        dat[7] = v2y;
        dat[8] = v2z;

        dat[9] = v3x;
        dat[10] = v3y;
        dat[11] = v3z;

        fwrite(dat, sizeof(float), 12, fp);
        fwrite("wl", sizeof(char), 2, fp);
    }

    fclose(fp);

    delete[] dat;
    delete[] fileInf;
}
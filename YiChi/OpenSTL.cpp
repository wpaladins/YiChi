#include "OpenSTL.h"

bool ReadASCII(const char* cfilename)
{
    int i = 0, j = 0, cnt = 0, pCnt = 4;
    char a[100];
    char str[100];
    double x = 0, y = 0, z = 0;

    std::ifstream in(cfilename, std::ifstream::in);

    if (!in)
        return false;
    do
    {
        i = 0;
        cnt = 0;
        in.getline(a, 100, '\n');
        while (a[i] != '\0')
        {
            if (!islower((int)a[i]) && !isupper((int)a[i]) && a[i] != ' ')
                break;
            cnt++;
            i++;
        }

        while (a[cnt] != '\0')
        {
            str[j] = a[cnt];
            cnt++;
            j++;
        }
        str[j] = '\0';
        j = 0;

        if (sscanf(str, "%lf%lf%lf", &x, &y, &z) == 3)
        {
            /*point3d point(x, y, z, 1.0);
            points.push_back(point);*/
        }
        pCnt++;
    } while (!in.eof());

    return true;
}

bool ReadBinary(const char* cfilename)
{
    char str[80];
    std::ifstream in(cfilename, std::ifstream::in | std::ifstream::binary);

    if (!in)
        return false;

    in.read(str, 80);

    //number of triangles  
    int triangles;
    in.read((char*)&triangles, sizeof(int));

    if (triangles == 0)
        return false;

    for (int i = 0; i < triangles; i++)
    {
        float coorXYZ[12];
        in.read((char*)coorXYZ, 12 * sizeof(float));

        normal _normal(coorXYZ[0], coorXYZ[1], coorXYZ[2]);
        
        BSTree pos;
        bool taller;
        point *p1 = new point(coorXYZ[3], coorXYZ[4], coorXYZ[5], false);
        if (!FindNode(root, *p1, &pos)) {
            // 没找到的话
            p1 = new point(coorXYZ[3], coorXYZ[4], coorXYZ[5]);
            InsertAVL(&root, *p1, &taller);
            mesh.points.push_back(*p1);
        }
        else {
            // 找到了的话
            delete p1;
            p1 = &pos->data;
        }
        point *p2 = new point(coorXYZ[6], coorXYZ[7], coorXYZ[8], false);
        if (!FindNode(root, *p2, &pos)) {
            p2 = new point(coorXYZ[6], coorXYZ[7], coorXYZ[8]);
            InsertAVL(&root, *p2, &taller);
            mesh.points.push_back(*p2);
        }
        else {
            delete p2;
            p2 = &pos->data;
        }
        point *p3 = new point(coorXYZ[9], coorXYZ[10], coorXYZ[11], false);
        if (!FindNode(root, *p3, &pos)) {
            p3 = new point(coorXYZ[9], coorXYZ[10], coorXYZ[11]);
            InsertAVL(&root, *p3, &taller);
            mesh.points.push_back(*p3);
        }
        else {
            delete p3;
            p3 = &pos->data;
        }
        facet _facet(p1->GetID(), p2->GetID(), p3->GetID(), _normal);
        mesh.facets.push_back(_facet);

        // 边操作
        eBSTree ePos; // BSTree---
        bool eTaller; // BSTree---
        edge* e1;
        if (*p1 < *p2) {
            e1 = new edge(p2, p1);
        }
        else {
            e1 = new edge(p1, p2);
        }
        if(!eFindNode(eRoot, *e1, &ePos)) { // BSTree---
            // 没有找到
            e1->putNearTri(_facet);
            eInsertAVL(&eRoot, *e1, &eTaller);
            mesh.edges.push_back(*e1);
        }
        else {
            // 找到了
            edge* temp = e1;
            e1 = &ePos->data;
            delete temp;
            e1->putNearTri(_facet);
        }
        edge* e2;
        if (*p3 < *p2) {
            e2 = new edge(p2, p3);
        }
        else {
            e2 = new edge(p3, p2);
        }
        if (!eFindNode(eRoot, *e2, &ePos)) {
            e2->putNearTri(_facet);
            eInsertAVL(&eRoot, *e2, &eTaller);
            mesh.edges.push_back(*e2);
        }
        else {
            edge* temp = e2;
            e2 = &ePos->data;
            delete temp;
            e2->putNearTri(_facet);
        }
        edge* e3;
        if (*p1 < *p3) {
            e3 = new edge(p3, p1);
        }
        else {
            e3 = new edge(p1, p3);
        }
        //AVLTree--- if (edgeAVLTree.Insert(*e3, *e3)) {
        if (!eFindNode(eRoot, *e3, &ePos)) { // BSTree---
            e3->putNearTri(_facet);
            eInsertAVL(&eRoot, *e3, &eTaller);
            mesh.edges.push_back(*e3);
        }
        else {
            edge* temp = e3;
            e3 = &ePos->data;
            delete temp;
            e3->putNearTri(_facet);
        }
        in.read((char*)coorXYZ, 2);
    }

    in.close();

    return true;
}

bool ReadSTLFile(const char* cfilename)
{
    if (cfilename == NULL)
        return false;

    std::ifstream in(cfilename, std::ifstream::in);

    if (!in)
        return false;

    std::string headStr;
    getline(in, headStr, ' ');
    in.close();

    if (headStr.empty())
        return false;

    if (headStr[0] == 's')
    {
        ReadASCII(cfilename);
    }
    else
    {
        ReadBinary(cfilename);
    }
    return true;
}

//int main()
//{
//    // std::string filename = "H:\\YJ\\bysj\\rec\\DInlay1.stl";
//    if (ReadSTLFile("H:\\YJ\\bysj\\rec\\DInlay1.stl")) {
//        std::cout << "成功" << std::endl;
//        std::cout << points.size();
//        for (int i = 0; i < points.size(); ++i) {
//            std::cout << i << " x:" << points[i].x << " y:" << points[i].y << " z:" << points[i].z << std::endl;
//        }
//    }
//}
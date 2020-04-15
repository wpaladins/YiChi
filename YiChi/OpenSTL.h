#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DataStruct.h"
#include "AVL.h"
#include "EdgeAVL.h" // EdgeAVL
//AVLTree--- #include "AVLTree.h"

bool ReadSTLFile(const char* cfilename);

extern triangle_mesh mesh;
extern BSTree root;
//AVLTree--- extern AVLTree<edge, edge> edgeAVLTree;
extern eBSTree eRoot;
#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DataStruct.h"
#include "AVL.h"

bool ReadSTLFile(const char* cfilename);

extern triangle_mesh mesh;
extern BSTree root;
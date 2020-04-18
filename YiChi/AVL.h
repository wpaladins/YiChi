#pragma once
#include <cstdio>
#include <cstdlib>
#include "DataStruct.h"

//�ֱ���ƽ��������
#define LH +1
#define EH  0
#define RH -1

typedef point ElemType;

//�������������
typedef struct BSTNode {
    ElemType data;
    point* address;
    int bf;//balance flag
    struct BSTNode* lchild, * rchild;
}*BSTree, BSTNode;

void R_Rotate(BSTree* p);
void L_Rotate(BSTree* p);
void LeftBalance(BSTree* T);
void RightBalance(BSTree* T);
int InsertAVL(BSTree* T, ElemType e, point* add, bool* taller);
bool FindNode(BSTree root, ElemType e, BSTree* pos);
void InorderTra(BSTree root);

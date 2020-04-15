#pragma once
#include <cstdio>
#include <cstdlib>
#include "DataStruct.h"

//�ֱ���ƽ��������
#define LH +1
#define EH  0
#define RH -1

typedef edge eElemType;

//�������������
typedef struct eBSTNode {
    eElemType data;
    int bf;//balance flag
    struct eBSTNode* lchild, * rchild;
    eBSTNode():lchild(NULL),rchild(NULL),bf(0){}
}*eBSTree, eBSTNode;

void eR_Rotate(eBSTree* p);
void eL_Rotate(eBSTree* p);
void eLeftBalance(eBSTree* T);
void eRightBalance(eBSTree* T);
int eInsertAVL(eBSTree* T, eElemType e, bool* taller);
bool eFindNode(eBSTree root, eElemType e, eBSTree* pos);
void eInorderTra(eBSTree root);
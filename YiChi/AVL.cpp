#include "AVL.h"

//���� p Ϊ�����Ķ����������������� p ָ��ָ���µ��������
void R_Rotate(BSTree* p)
{
    //���������е�ͼ 5 ��ʾ������⣬���н�� A Ϊ p ָ��ָ��ĸ����
    BSTree lc = (*p)->lchild;
    (*p)->lchild = lc->rchild;
    lc->rchild = *p;
    *p = lc;
}

//���� p Ϊ�����Ķ����������������� p ָ��ָ���µ��������
void L_Rotate(BSTree* p)
{
    //���������е�ͼ 6 ��ʾ������⣬���н�� A Ϊ p ָ��ָ��ĸ����
    BSTree rc = (*p)->rchild;
    (*p)->rchild = rc->lchild;
    rc->lchild = *p;
    *p = rc;
}

//����ָ�� T ��ָ����Ϊ�����Ķ���������������ƽ�⴦����ָ�� T ָ���µĸ����
void LeftBalance(BSTree* T)
{
    BSTree lc, rd;
    lc = (*T)->lchild;
    //�鿴�� T ��������Ϊ������������ʧȥƽ���ԭ����� bf ֵΪ 1 ����˵�������������Ϊ�������������У���Ҫ�����������������֮����� bf ֵΪ -1��˵���������������Ϊ�������������У���Ҫ����˫���������������Ĵ���
    switch (lc->bf)
    {
    case LH:
        (*T)->bf = lc->bf = EH;
        R_Rotate(T);
        break;
    case RH:
        rd = lc->rchild;
        switch (rd->bf)
        {
        case LH:
            (*T)->bf = RH;
            lc->bf = EH;
            break;
        case EH:
            (*T)->bf = lc->bf = EH;
            break;
        case RH:
            (*T)->bf = EH;
            lc->bf = LH;
            break;
        }
        rd->bf = EH;
        L_Rotate(&(*T)->lchild);
        R_Rotate(T);
        break;
    }
}

//��������ƽ�⴦��ͬ��������ƽ�⴦����ȫ����
void RightBalance(BSTree* T)
{
    BSTree lc, rd;
    lc = (*T)->rchild;
    switch (lc->bf)
    {
    case RH:
        (*T)->bf = lc->bf = EH;
        L_Rotate(T);
        break;
    case LH:
        rd = lc->lchild;
        switch (rd->bf)
        {
        case LH:
            (*T)->bf = EH;
            lc->bf = RH;
            break;
        case EH:
            (*T)->bf = lc->bf = EH;
            break;
        case RH:
            (*T)->bf = EH;
            lc->bf = LH;
            break;
        }
        rd->bf = EH;
        R_Rotate(&(*T)->rchild);
        L_Rotate(T);
        break;
    }
}

int InsertAVL(BSTree* T, ElemType e, bool* taller)
{
    //�������Ϊ��������ֱ����� e Ϊ�����
    if ((*T) == NULL)
    {
        (*T) = (BSTree)malloc(sizeof(BSTNode));
        if ((*T) != NULL){
            (*T)->bf = EH;
            (*T)->data = e;
            (*T)->lchild = NULL;
            (*T)->rchild = NULL;
            *taller = true;
        }
    }
    //����������������Ѿ����� e �������κδ���
    else if (e == (*T)->data)
    {
        *taller = false;
        return 0;
    }
    //��� e С�ڽ�� T ������������뵽 T ����������
    else if (e < (*T)->data)
    {
        //���������̣�����Ӱ���������ƽ�⣬��ֱ�ӽ���
        if (!InsertAVL(&(*T)->lchild, e, taller))
            return 0;
        //�жϲ�������Ƿ�ᵼ������������� +1
        if (*taller)
        {
            //�жϸ���� T ��ƽ�������Ƕ��٣���������������������½��Ĺ����е���ʧȥƽ�⣬���Ե� T ����ƽ�����ӱ���Ϊ 1 ʱ����Ҫ������������ƽ�⴦������������и�����ƽ��������
            switch ((*T)->bf)
            {
            case LH:
                LeftBalance(T);
                *taller = false;
                break;
            case  EH:
                (*T)->bf = LH;
                *taller = true;
                break;
            case RH:
                (*T)->bf = EH;
                *taller = false;
                break;
            }
        }
    }
    //ͬ������ e>T->data ʱ����Ҫ���뵽�� T Ϊ�����������������У�ͬ����Ҫ��������ͬ���Ĳ���
    else
    {
        if (!InsertAVL(&(*T)->rchild, e, taller))
            return 0;
        if (*taller)
        {
            switch ((*T)->bf)
            {
            case LH:
                (*T)->bf = EH;
                *taller = false;
                break;
            case EH:
                (*T)->bf = RH;
                *taller = true;
                break;
            case  RH:
                RightBalance(T);
                *taller = false;
                break;
            }
        }
    }
    return 1;
}

//�ж�����ƽ����������Ƿ��Ѿ�����������Ϊ e �Ľ��
bool FindNode(BSTree root, ElemType e, BSTree* pos)
{
    BSTree pt = root;
    (*pos) = NULL;
    while (pt)
    {
        if (pt->data == e)
        {
            //�ҵ��ڵ㣬posָ��ýڵ㲢����true
            (*pos) = pt;
            return true;
        }
        else if (e < pt->data)
        {
            pt = pt->lchild;
        }
        else
            pt = pt->rchild;
    }
    return false;
}

//�������ƽ�������
void InorderTra(BSTree root)
{
    if (root->lchild)
        InorderTra(root->lchild);

    // printf("%d ", root->data);

    if (root->rchild)
        InorderTra(root->rchild);
}

//int main()
//{
//    int i, nArr[] = { 1,23,45,34,98,9,4,35,23 };
//    BSTree root = NULL, pos;
//    bool taller;
//    //�� nArr���ұ���ƽ������������ϲ������ݵĹ��̣�
//    for (i = 0; i < 9; i++)
//    {
//        InsertAVL(&root, nArr[i], &taller);
//    }
//    //����������
//    InorderTra(root);
//    //�ж�ƽ����������Ƿ���������Ϊ 103 ������
//    if (FindNode(root, 103, &pos))
//        printf("\n%d\n", pos->data);
//    else
//        printf("\nNot find this Node\n");
//    return 0;
//}
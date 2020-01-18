#ifndef _MAT_H_
#define _MAT_H_


#define full 0
#define same 1
#define valid 2

typedef struct Mat2DSize{
    int c; // �У���
    int r; // �У��ߣ�
}nSize;

void addmat(float** res, float** mat1, nSize matSize1, float** mat2, nSize matSize2); // �������
float** cov(float** map, nSize mapSize, float** in_Data, nSize inSize, int type);

#endif

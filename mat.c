#include "mat.h"
#include <stdio.h>
#include <stdlib.h>

void addmat(float** ress, float** mat1, nSize matSize1, float** mat2, nSize matSize2)// �������
{
    
	int i,j;
	float bb[1][40];

    if(matSize1.c!=matSize2.c||matSize1.r!=matSize2.r)
	{
        printf("ERROR: Size is not same!");
	}

    for(i=0;i<matSize1.r;i++)
	{
        for(j=0;j<matSize1.c;j++)
		{
            ress[i][j]=mat1[i][j]+mat2[i][j];
			bb[i][j]=ress[i][j];
		}
	}
}

// map�������ģ�������, mapSize����˴�С��inputDataͼ�����ݣ�inSize������ͼ���С
float** cov(float** map,nSize mapSize,float** inputData,nSize inSize,int type) // �������
{
    float** outputData;
	float** inputData_same;
    int i,j,c,r;
	//float aa[1][40];
	int addc=2;//����ĸ���
    outputData=(float**)malloc(inSize.r*sizeof(float*)); //�еĴ�С1
    for(i=0;i<inSize.r;i++)
	{
        outputData[i]=(float*)calloc(inSize.c,sizeof(float));//����40��ע������calloc����Ҫ�Ĳ�ͬ��malloc����ʼ��������ڴ�,calloc��ʼ���ѷ�����ڴ�Ϊ0��
	}
//######################################Ϊ�˷�����㣬��inputData����һȦ, //ǰ�油������##########################//
    inputData_same=(float**)malloc((inSize.r)*sizeof(float*)); // ����ĳ�ʼ��  
    for(i=0;i<inSize.r;i++)
	{
		inputData_same[i]=(float*)malloc((inSize.c+addc)*sizeof(float));
	}
  //��������
    for(i=0;i<inSize.r;i++)
	{
        for(j=0;j<inSize.c+addc;j++)
		{
            if(j<1||j>inSize.c)//0��[1������������������40]��41��Ҳ����39����������������0�Ϳ����ˣ�����ǰ1���㣬���油һ����
			{
                inputData_same[i][j]=(float)0.0;
			}
            else
			{
                inputData_same[i][j]=inputData[i][j-1]; // ����ԭ���������ݣ���inputData[0][0]����inputData_same[0][2]
			}
        }
    }
	//��ʼÿ��ͨ���ڵ�������
    for(i=0;i<inSize.r;i++)
	{	
        for(j=0;j<inSize.c;j++)
		{
            for(r=0;r<mapSize.r;r++)
			{
                for(c=0;c<mapSize.c;c++)
				{
					outputData[i][j]=outputData[i][j]+map[r][c]*inputData_same[i+r][j+c];
                }
			}
			//aa[i][j]=outputData[i][j];//�������飬Ϊ�˿��÷���
		}
	}
   //�ͷŲ����inputData_same
	for(i=0;i<inSize.r;i++)
	{
		free(inputData_same[i]);
	}
	free(inputData_same);
   switch(type)
   { // ���ݲ�ͬ����������ز�ͬ�Ľ��
    case same: // ��ȫ��С�����
        return outputData;
    default:
        return outputData;
   }
}

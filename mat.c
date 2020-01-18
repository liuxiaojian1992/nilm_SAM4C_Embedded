#include "mat.h"
#include <stdio.h>
#include <stdlib.h>

void addmat(float** ress, float** mat1, nSize matSize1, float** mat2, nSize matSize2)// 矩阵相加
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

// map存放特征模块的数据, mapSize卷积核大小，inputData图像数据，inSize，输入图像大小
float** cov(float** map,nSize mapSize,float** inputData,nSize inSize,int type) // 卷积操作
{
    float** outputData;
	float** inputData_same;
    int i,j,c,r;
	//float aa[1][40];
	int addc=2;//加零的个数
    outputData=(float**)malloc(inSize.r*sizeof(float*)); //行的大小1
    for(i=0;i<inSize.r;i++)
	{
        outputData[i]=(float*)calloc(inSize.c,sizeof(float));//再列40，注意区别calloc，主要的不同是malloc不初始化分配的内存,calloc初始化已分配的内存为0。
	}
//######################################为了方便计算，将inputData扩大一圈, //前面补两个零##########################//
    inputData_same=(float**)malloc((inSize.r)*sizeof(float*)); // 结果的初始化  
    for(i=0;i<inSize.r;i++)
	{
		inputData_same[i]=(float*)malloc((inSize.c+addc)*sizeof(float));
	}
  //补两个零
    for(i=0;i<inSize.r;i++)
	{
        for(j=0;j<inSize.c+addc;j++)
		{
            if(j<1||j>inSize.c)//0，[1，————————40]，41，也就是39必须卷积到，补两个0就可以了，这是前1个零，后面补一个零
			{
                inputData_same[i][j]=(float)0.0;
			}
            else
			{
                inputData_same[i][j]=inputData[i][j-1]; // 复制原向量的数据，从inputData[0][0]传到inputData_same[0][2]
			}
        }
    }
	//开始每个通道内的运算了
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
			//aa[i][j]=outputData[i][j];//传给数组，为了看得方便
		}
	}
   //释放补零后inputData_same
	for(i=0;i<inSize.r;i++)
	{
		free(inputData_same[i]);
	}
	free(inputData_same);
   switch(type)
   { // 根据不同的情况，返回不同的结果
    case same: // 完全大小的情况
        return outputData;
    default:
        return outputData;
   }
}

#ifndef _CNN_H_
#define _CNN_H_

#include "mat.h"
#include <stdlib.h>
#include <stdio.h>

#define bool int
#define false 0
#define true 1

typedef struct MinstImg{
    int c;           // 图像宽
    int r;           // 图像高
    float** ImgData; // 图像数据二维动态数组
}MinstImg;

typedef struct MinstImgArr{
    int ImgNum;        // 存储图像的通道数
    MinstImg* ImgPtr;  // 存储图像数组指针
}*ImgArr;   

typedef struct MinstLabel{
    int l;            // 输出标记的长
    float* LabelData; // 输出标记数据
}MinstLabel;

typedef struct MinstLabelArr{
    int LabelNum;
    MinstLabel* LabelPtr;
}*LabelArr;              // 存储图像标记的数组

#define AvePool 0
#define MaxPool 1
#define MinPool 2

// 卷积层
typedef struct convolutional_layer{
	int inputWidth;   //输入图像的宽
	int inputHeight;  //输入图像的长
	int mapSize;      //特征模板的大小，模板一般都是正方形

	int inChannels;   //输入图像的数目
	int outChannels;  //输出图像的数目

	// 关于特征模板的权重分布，这里是一个四维数组
	// 其大小为inChannels*outChannels*mapSize*mapSize大小
	// 这里用四维数组，主要是为了表现全连接的形式，实际上卷积层并没有用到全连接的形式
	// 这里的例子是DeapLearningToolboox里的CNN例子，其用到就是全连接
	float**** mapData;     //存放特征模块的数据

	//float* basicData;   //偏置，偏置的大小，为outChannels
	bool isFullConnect; //是否为全连接
	bool* connectModel; //连接模式（默认为全连接）

	// 下面三者的大小同输出的维度相同
	float*** v; // 进入激活函数的输入值
	float*** y; // 激活函数后神经元的输出

}CovLayer;

// 采样层 pooling
typedef struct pooling_layer{
	int inputWidth;   //输入图像的宽
	int inputHeight;  //输入图像的长
	int mapSize;      //特征模板的大小

	int inChannels;   //输入图像的数目
	int outChannels;  //输出图像的数目

	int poolType;     //Pooling的方法
	//float* basicData;   //偏置

	float*** y; // 采样函数后神经元的输出,无激活函数
}PoolLayer;

typedef struct cnn_network{
	int layerNum;
	CovLayer* C1;
	PoolLayer* S2;
	CovLayer* C3;
	PoolLayer* S4;
}CNN;

//void cnnsetup(CNN* cnn,nSize inputSize,int outputSize);



// 初始化卷积层
CovLayer* initCovLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels);
// 初始化采样层
PoolLayer* initPoolLayer(int inputWidth,int inputHeigh,int mapSize,int inChannels,int outChannels,int poolType);

// 激活函数 input是数据，inputNum说明数据数目，bas表明偏置
float activation_Relu(float input,float bas); // sigma激活函数
//void cnnff(CNN* cnn,ImgArr inputData); // 网络的前向传播

void avgPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize); // 求平均值
void maxPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize); // 求平均值
#endif

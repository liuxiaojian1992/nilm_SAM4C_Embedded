#ifndef _CNN_H_
#define _CNN_H_

#include "mat.h"
#include <stdlib.h>
#include <stdio.h>

#define bool int
#define false 0
#define true 1

typedef struct MinstImg{
    int c;           // ͼ���
    int r;           // ͼ���
    float** ImgData; // ͼ�����ݶ�ά��̬����
}MinstImg;

typedef struct MinstImgArr{
    int ImgNum;        // �洢ͼ���ͨ����
    MinstImg* ImgPtr;  // �洢ͼ������ָ��
}*ImgArr;   

typedef struct MinstLabel{
    int l;            // �����ǵĳ�
    float* LabelData; // ����������
}MinstLabel;

typedef struct MinstLabelArr{
    int LabelNum;
    MinstLabel* LabelPtr;
}*LabelArr;              // �洢ͼ���ǵ�����

#define AvePool 0
#define MaxPool 1
#define MinPool 2

// �����
typedef struct convolutional_layer{
	int inputWidth;   //����ͼ��Ŀ�
	int inputHeight;  //����ͼ��ĳ�
	int mapSize;      //����ģ��Ĵ�С��ģ��һ�㶼��������

	int inChannels;   //����ͼ�����Ŀ
	int outChannels;  //���ͼ�����Ŀ

	// ��������ģ���Ȩ�طֲ���������һ����ά����
	// ���СΪinChannels*outChannels*mapSize*mapSize��С
	// ��������ά���飬��Ҫ��Ϊ�˱���ȫ���ӵ���ʽ��ʵ���Ͼ���㲢û���õ�ȫ���ӵ���ʽ
	// �����������DeapLearningToolboox���CNN���ӣ����õ�����ȫ����
	float**** mapData;     //�������ģ�������

	//float* basicData;   //ƫ�ã�ƫ�õĴ�С��ΪoutChannels
	bool isFullConnect; //�Ƿ�Ϊȫ����
	bool* connectModel; //����ģʽ��Ĭ��Ϊȫ���ӣ�

	// �������ߵĴ�Сͬ�����ά����ͬ
	float*** v; // ���뼤���������ֵ
	float*** y; // ���������Ԫ�����

}CovLayer;

// ������ pooling
typedef struct pooling_layer{
	int inputWidth;   //����ͼ��Ŀ�
	int inputHeight;  //����ͼ��ĳ�
	int mapSize;      //����ģ��Ĵ�С

	int inChannels;   //����ͼ�����Ŀ
	int outChannels;  //���ͼ�����Ŀ

	int poolType;     //Pooling�ķ���
	//float* basicData;   //ƫ��

	float*** y; // ������������Ԫ�����,�޼����
}PoolLayer;

typedef struct cnn_network{
	int layerNum;
	CovLayer* C1;
	PoolLayer* S2;
	CovLayer* C3;
	PoolLayer* S4;
}CNN;

//void cnnsetup(CNN* cnn,nSize inputSize,int outputSize);



// ��ʼ�������
CovLayer* initCovLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels);
// ��ʼ��������
PoolLayer* initPoolLayer(int inputWidth,int inputHeigh,int mapSize,int inChannels,int outChannels,int poolType);

// ����� input�����ݣ�inputNum˵��������Ŀ��bas����ƫ��
float activation_Relu(float input,float bas); // sigma�����
//void cnnff(CNN* cnn,ImgArr inputData); // �����ǰ�򴫲�

void avgPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize); // ��ƽ��ֵ
void maxPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize); // ��ƽ��ֵ
#endif

/* device identification */
#include"string.h"
#include"stdlib.h"
#include "math.h"
#include "stdio.h"
//
#include "cnn.h"
#include "model.h"
#include "data.h"
#include "mat.h"
#include "shuju.h"

/*
函数功能： 负荷识别
参数说明：
            gFeatureCycBuff             // 输入值，归一化后6个特征
*/
/*
函数功能： 负荷识别
参数说明：
            gFeatureCycBuff             // 输入值，归一化后6个特征
*/

int NILM_DiClassification_Recognition()
{   
    int i,j,r,c;
    int channel=6;
    int hang=1;
    int FEATURE_SIZE=40;
	/*float xx_test[240];*/
    ImgArr InData;//数据的输入，按照通道数，长，宽定义好
    nSize inputSize={FEATURE_SIZE,1};//40 1 6
	int outSize=10;//10
	float ** mapout;
	float out_max_value=0;//最后10分类最大值，
	nSize mapSize={3,1};//卷积核大小3*1，c=3,r=1
	// 第一层输出数据
	nSize inSize1={40,1};
	nSize outSize1={40,1};
	//第二层输出数据
	nSize inSize2={40,1};
	nSize outSize2={20,1};
	//第三层输出数据
	nSize inSize3={20,1};
	nSize outSize3={20,1};
	//第四层输出数据
	nSize inSize4={20,1};
	nSize outSize4={10,1};
    //flatten  dense 放在外面，减少malloc 
    float O5inData[100];//flatten
    float O5outData[10];
	int cnn_O5_inputNum=100;
	int cnn_O5_outputNum=10;
	int out_max_k=0;//分类情况
	// CNN结构的初始化（卷积层的参数）
	CNN* cnn=(CNN*)malloc(sizeof(CNN));
 //   //输入数据的初始化
	//memset(xx_test,0,sizeof(FP32)*(FEATURE_SIZE*channel));
	//flatten  dense 层的初始化
	memset(O5outData,0,sizeof(float)*(10));
	memset(O5inData,0,sizeof(float)*(100));
// #####################################################输入的数据按要求调整好###############################################//
    //for (i = 0; i < FEATURE_SIZE; i++)//长度
    //{
    //    xx_test[FEATURE_SIZE*0+i]=pFeature->ActPwr[39-i];
    //    xx_test[FEATURE_SIZE*1+i]=pFeature->reactPwr[39-i];
    //    xx_test[FEATURE_SIZE*2+i]=pFeature->fundActPwr[39-i];
    //    xx_test[FEATURE_SIZE*3+i]=pFeature->fundReactPwr[39-i];
    //    xx_test[FEATURE_SIZE*4+i]=pFeature->hd2[39-i];
    //    xx_test[FEATURE_SIZE*5+i]=pFeature->hd3[39-i];
    //}
    // 图像数组的初始化
	InData=(ImgArr)malloc(sizeof(ImgArr));
    InData->ImgPtr=(MinstImg*)malloc(channel*sizeof(MinstImg));
	InData->ImgNum=channel;//通道数传递
    for(j= 0; j <channel; j++)//通道数
    {
        InData->ImgPtr[j].r=hang;//行
		InData->ImgPtr[j].c=FEATURE_SIZE;//列
        InData->ImgPtr[j].ImgData=(float**)malloc(hang*sizeof(float*));
        for(r= 0; r <hang; r++)//行
        {
           InData->ImgPtr[j].ImgData[r]=(float*)malloc(FEATURE_SIZE*sizeof(float));
            for (c = 0; c < FEATURE_SIZE; c++)//列
            {
                InData->ImgPtr[j].ImgData[r][c]= xx_test[j*FEATURE_SIZE+c];
            }
        }
    }
//####################################################第一卷积层###########################################################################//
	//#################C1初始化#############################//
    cnn->C1=initCovLayer(40,1,3,6,20);//输入长度40，宽度1，卷积核大小3，通道数6，滤波器个数20。//输出长度40，宽带1，通道数20
	//################C1网络导入参数######################//
    for(r=0;r<outSize1.r;r++)//高1，卷积大小为1
    {
        for(c=0;c<cnn->C1->mapSize;c++)//宽40，卷积大小为3
        {
            for(i=0;i<cnn->C1->inChannels;i++)//6个通道数
            {
                for(j=0;j<cnn->C1->outChannels;j++)//20个滤波器
				{
					cnn->C1->mapData[i][j][r][c]=cov1_w[c][i][j];
				}
			}
		}
	}
	//################C1卷积运算######################//
	for(i=0;i<(cnn->C1->outChannels);i++)//20
	{
		for(j=0;j<(cnn->C1->inChannels);j++)//6
		{
			//其实输出为单通道的输入一次情况  
  			mapout=cov(cnn->C1->mapData[j][i],mapSize,InData->ImgPtr[j].ImgData,inSize1,same);//float**** mapData;//存放参数的数据,
			//每次输出通道都是输入通道的叠加
			addmat(cnn->C1->v[i],cnn->C1->v[i],outSize1,mapout,outSize1);//每层通道数卷积后的mapout的叠加
			for(r=0;r<outSize1.r;r++)
			{
				free(mapout[r]);//这里是不是就释放了输出的cov中的outputData？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
			}
			free(mapout);
		}
		//这是通道叠加完，加偏置
		//应该得到的输出对应的单通道的长度为40的数据，
		for(r=0;r<outSize1.r;r++)
		{
			for(c=0;c<outSize1.c;c++)
			{
				cnn->C1->y[i][r][c]=activation_Relu(cnn->C1->v[i][r][c],cov1_b[i]);//i通道数，r输出长的大小，c输出宽的大小[20][1][40]
			}
		}
	}
	//释放输入InData->ImgPtr数据
	for(j= 0; j <channel; j++)
	{
		 for(r= 0; r <hang; r++)//行
		 {
			free(InData->ImgPtr[j].ImgData[r]);
		 } 
		 free(InData->ImgPtr[j].ImgData);
	}
	free(InData->ImgPtr);
	//释放激活函数前的保存变量cnn->C1->v
	for(i=0;i<(cnn->C1->outChannels);i++)
	{
		for(r=0;r<outSize1.r;r++)
		{
			free(cnn->C1->v[i][r]);
        }
		free(cnn->C1->v[i]);
    }
	free(cnn->C1->v);
	//释放卷积权重cnn->C1->mapData
    for(i=0;i<(cnn->C1->inChannels);i++)
	{
        for(j=0;j<(cnn->C1->outChannels);j++)
		{
			free(cnn->C1->mapData[i][j]);
		}
		free(cnn->C1->mapData[i]);
	}
	free(cnn->C1->mapData);
//####################################################第二层###########################################################################//
	//#################C2初始化#############################//
    cnn->S2=initPoolLayer(40,1,2,20,20,MaxPool);//输入长度40，宽度1，卷积核大小2，通道数20，滤波器个数20。//输出长度20，宽带1，通道数20
	//################C2池化运算######################//
	for(i=0;i<(cnn->S2->outChannels);i++)//20通道数
	{
		if(cnn->S2->poolType==MaxPool)
		{
			//输出，输出长度（20，1），上一层输入，输入长度（40，1），池化大小
			maxPooling(cnn->S2->y[i],outSize2,cnn->C1->y[i],inSize2,cnn->S2->mapSize);
		}
	}
	//释放激活函数前的保存变量cnn->C1->y
	for(i=0;i<(cnn->C1->outChannels);i++)
	{
		for(r=0;r<outSize1.r;r++)
		{
			free(cnn->C1->y[i][r]);
        }
		free(cnn->C1->y[i]);
    }
	free(cnn->C1->y);
//####################################################第三层###########################################################################//
	//#################C3初始化#############################//
    cnn->C3=initCovLayer(20,1,3,20,10);//输入长度20，宽度1，卷积核大小3，通道数20，滤波器个数10。//输出长度20，宽带1，通道数10
	//################C3网络导入参数######################//
	for(r=0;r<outSize3.r;r++)//高1，卷积大小为1
    {
        for(c=0;c<cnn->C3->mapSize;c++)//卷积大小为3
        {
            for(i=0;i<cnn->C3->inChannels;i++)//20个通道数
            {
                for(j=0;j<cnn->C3->outChannels;j++)//10个滤波器
				{
					cnn->C3->mapData[i][j][r][c]=cov3_w[c][i][j];
				}
			}
		}
	}
	//################C3卷积运算######################//
	for(i=0;i<(cnn->C3->outChannels);i++)
	{
		for(j=0;j<(cnn->C3->inChannels);j++)
		{
			mapout=cov(cnn->C3->mapData[j][i],mapSize,cnn->S2->y[j],inSize3,valid);
			addmat(cnn->C3->v[i],cnn->C3->v[i],outSize3,mapout,outSize3);
			for(r=0;r<outSize3.r;r++)
			{
				free(mapout[r]);
			}
			free(mapout);
		}
		for(r=0;r<outSize3.r;r++)
		{
			for(c=0;c<outSize3.c;c++)
			{
				cnn->C3->y[i][r][c]=activation_Relu(cnn->C3->v[i][r][c],cov3_b[i]);
			}
		}
	}
	//释放输入cnn->S2->y[j]数据
	free(cnn->S2->y);//这里需要特别注意
	//释放激活函数前的保存变量cnn->C1->v
	for(i=0;i<(cnn->C3->outChannels);i++)
	{
		for(r=0;r<outSize1.r;r++)
		{
			free(cnn->C3->v[i][r]);
        }
		free(cnn->C3->v[i]);
    }
	free(cnn->C3->v);
	//释放卷积权重cnn->C1->mapData
    for(i=0;i<(cnn->C3->inChannels);i++)
	{
        for(j=0;j<(cnn->C3->outChannels);j++)
		{
			free(cnn->C3->mapData[i][j]);
		}
		free(cnn->C3->mapData[i]);
	}
	free(cnn->C3->mapData);

//####################################################第四层###########################################################################//
	//#################C4初始化#############################//
    cnn->S4=initPoolLayer(20,1,2,10,10,MaxPool);//输入长度20，宽度1，卷积核大小2，通道数10，滤波器个数10。//输出长度10，宽带1，通道数10
	//#################C4池化运算###########################//
	for(i=0;i<(cnn->S4->outChannels);i++)
	{
		if(cnn->S4->poolType==MaxPool)
		{
			maxPooling(cnn->S4->y[i],outSize4,cnn->C3->y[i],inSize4,cnn->S4->mapSize);
		}
	}
	//释放激活函数前的保存变量cnn->C3->y
	for(i=0;i<(cnn->C3->outChannels);i++)
	{
		for(r=0;r<outSize3.r;r++)
		{
			free(cnn->C3->y[i][r]);
        }
		free(cnn->C3->y[i]);
    }
	free(cnn->C3->y);
//####################################################输出层O5的Flatten处理###########################################################################//
	// 首先需要将前面的多维输出展开成一维向量1*100
	for(i=0;i<(cnn->S4->outChannels);i++)
	{
		for(r=0;r<outSize4.r;r++)
		{
			for(c=0;c<outSize4.c;c++)
			{
				O5inData[c*(cnn->S4->outChannels)+i]=cnn->S4->y[i][r][c];
			}
		}
	}
	//释放cnn->S4->y
	free(cnn->S4->y);

//####################################################全连接层###########################################################################//
	// 首先需要将前面的多维输出展开成一维向量1*100
	for(i=0;i<cnn_O5_outputNum;i++)//10
	{
		float ss=0;
		for(j=0;j<cnn_O5_inputNum;j++)//100
		{
			ss=ss+O5inData[j]*dense1_w[j][i];//全连接
		}
		O5outData[i]=ss+dense1_b[i];
	}
    //分类结果，直接取最大值对应的位置，
	out_max_value=O5outData[0];
	for(i=0;i<cnn_O5_outputNum;i++)
	{
		
		if((O5outData[i])-out_max_value>0)
		{
			out_max_value=O5outData[i];
			out_max_k=i;

		}
	}
	free(cnn);
	return out_max_k;
}

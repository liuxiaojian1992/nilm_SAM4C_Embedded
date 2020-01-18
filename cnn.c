#include "cnn.h"
#include "data.h"
//mapSize为卷积核大小，outChannels滤波器的个数
CovLayer* initCovLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels)
{
    CovLayer* covL=(CovLayer*)malloc(sizeof(CovLayer));
    int i,j,r;
    int outW=inputWidth;//输出大小
    int outH=inputHeight;

    covL->inputHeight=inputHeight;
    covL->inputWidth=inputWidth;
    covL->mapSize=mapSize;

    covL->inChannels=inChannels;
    covL->outChannels=outChannels;

    covL->isFullConnect=true; // 默认为全连接

    // 权重空间的初始化，先行再列调用，[r][c]
    
    covL->mapData=(float****)malloc(inChannels*sizeof(float***));
    for(i=0;i<inChannels;i++)
	{
        covL->mapData[i]=(float***)malloc(outChannels*sizeof(float**));
        for(j=0;j<outChannels;j++)
		{
            covL->mapData[i][j]=(float**)malloc(mapSize*sizeof(float*));
            for(r=0;r<1;r++)
			{
                covL->mapData[i][j][r]=(float*)malloc(sizeof(float));
    //            for(c=0;c<mapSize;c++)
				//{
    //                randnum=(((float)rand()/(float)RAND_MAX)-0.5)*2; // 产生一个-1到1的随机数
    //                covL->mapData[i][j][r][c]=randnum*sqrt((float)6.0/(float)(mapSize*mapSize*(inChannels+outChannels)));
    //            }
            }
        }
    }

    //
    covL->v=(float***)malloc(outChannels*sizeof(float**));
    covL->y=(float***)malloc(outChannels*sizeof(float**));
    for(j=0;j<outChannels;j++)
	{
        covL->v[j]=(float**)malloc(outH*sizeof(float*));
        covL->y[j]=(float**)malloc(outH*sizeof(float*));
        for(r=0;r<outH;r++)
		{
            covL->v[j][r]=(float*)calloc(outW,sizeof(float));
            covL->y[j][r]=(float*)calloc(outW,sizeof(float));
        }
    }

    return covL;
}

PoolLayer* initPoolLayer(int inputWidth,int inputHeight,int mapSize,int inChannels,int outChannels,int poolType)
{
	PoolLayer* poolL=(PoolLayer*)malloc(sizeof(PoolLayer));
    int outW=inputWidth/mapSize;
    int outH=inputHeight/mapSize;

    int j,r;

	poolL->inputHeight=inputHeight;
	poolL->inputWidth=inputWidth;
	poolL->mapSize=mapSize;
	poolL->inChannels=inChannels;
	poolL->outChannels=outChannels;
	poolL->poolType=poolType; 

	poolL->y=(float***)malloc(outChannels*sizeof(float**));
	for(j=0;j<outChannels;j++)
	{
		poolL->y[j]=(float**)malloc(outH*sizeof(float*));
		for(r=0;r<outH;r++)
		{
			poolL->y[j][r]=(float*)calloc(outW,sizeof(float));
		}
	}

	return poolL;
}


float activation_Relu(float input,float bas) // Relu激活函数
{
	float temp=input+bas;
	if(temp>0)
	{

		
		
		return temp;
	}
	else
	{	    
	    return 0;
	}
}
void avgPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize) // 求平均值
{
	int outputW=inputSize.c/mapSize;//20
	int outputH=inputSize.r;//1
    int i,j,n;
	//memset(&output[0],0,sizeof(float)*(outputW*outputH));
	if(outputSize.c!=outputW||outputSize.r!=outputH)
		printf("ERROR: output size is wrong!!");
	for(i=0;i<outputH;i++)
	{
		output[i]=(float*)malloc(outputW*sizeof(float));
		for(j=0;j<outputW;j++)
		{	
			float sum=0.0;
			for(n=j*mapSize;n<j*mapSize+mapSize;n++)
			{
				sum=sum+input[i][n];
			}
			output[i][j]=sum/(float)(mapSize);
		}
	}
}
void maxPooling(float** output,nSize outputSize,float** input,nSize inputSize,int mapSize) // 求最大值
{
	int outputW=inputSize.c/mapSize;//20
	int outputH=inputSize.r;//1
    int i,j,n;
	float aa[40];
	//memset(aa,0,sizeof(float)*(inputSize.c));
	if(outputSize.c!=outputW||outputSize.r!=outputH)
		printf("ERROR: output size is wrong!!");
	//output=(float**)malloc(outputH*sizeof(float*));
	for(i=0;i<outputH;i++)
	{
		output[i]=(float*)malloc(outputW*sizeof(float));
		for(j=0;j<outputW;j++)//20
			{
				for(n=j*mapSize;n<j*mapSize+mapSize;n++)
				{
					aa[n]=input[i][n];//先全部读出开，因为一个一个读出开的

				}			
				if(aa[j*mapSize] - aa[j*mapSize+1] > 0)//两个数据读出来后，比较大小
				{
					output[i][j]=aa[j*mapSize];
				}
				else
				{
					output[i][j]=aa[j*mapSize+1];
				}
			}
	}


}
#include "Image_Processing.h"

/* 曝光设置 */
uint16 exp_time = 100;

/* 图像变量 */
uint8 Image_Use[Image_H][Image_W]; //压缩后的图像
uint8 image_two_value[Image_H][Image_W]; //二值化后的图像

int Adapt_Threshold = 0;

/*************************************************************************
 *  函数名称：void Get_Use_Image (void)
 *  功能说明：把摄像头采集到原始图像，缩放到赛道识别所需大小
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年10月28日
 *  备    注：  IMAGEW为原始图像的宽度，神眼为188，OV7725为320
 *       IMAGEH为原始图像的高度，神眼为120，OV7725为240
 *************************************************************************/
void Get_Use_Image(void)
{
    short i = 0, j = 0, row = 0, line = 0;

    for (i = 0; i < MT9V03X_H; i ++)          //50，
    {
        for (j = 0; j < MT9V03X_W; j ++)     //94，
        {
            image_copy_out[row][line] = mt9v03x_image[i][j];
            line++;
        }
        line = 0;
        row++;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     图像二值化处理函数
  @param     二值化阈值
  @return    NULL
  Sample     Image_Binarization(Threshold);//图像二值化
  @note      二值化后直接访问image_two_value[i][j]这个数组即可
-------------------------------------------------------------------------------------------------------------------*/
void Image_Binarization(int threshold, uint16 col, uint16 row)//图像二值化
{
    uint16 i,j;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)//灰度图的数据只做判断，不进行更改，二值化图像放在了新数组中
        {
            if(Image_Use[i][j] >= threshold)
                image_two_value[i][j] = IMG_WHITE;//白
            else
                image_two_value[i][j] = IMG_BLACK;//黑
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     快速大津求阈值，来自山威
  @param     image       图像数组
             col         列 ，宽度
             row         行，长度
  @return    null
  Sample     threshold=my_adapt_threshold(mt9v03x_image[0],MT9V03X_W, MT9V03X_H);//山威快速大津
  @note      据说比传统大津法快一点，实测使用效果差不多
-------------------------------------------------------------------------------------------------------------------*/
int My_Adapt_Threshold(uint8 *image, uint16 col, uint16 row)   //注意计算阈值的一定要是原图像
{
	#define GrayScale 256
	uint16 width = col;
	uint16 height = row;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	uint16 i, j;
	int pixelSum = width * height/4;
	int threshold = 0;
	uint8* img_data = image;  //指向像素数据的指针
	uint32 gray_sum=0;
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}
	//统计灰度级中每个像素在整幅图像中的个数
	for (i = 0; i < height; i+=2)
	{
		for (j = 0; j < width; j+=2)
		{
			pixelCount[(int)img_data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
			gray_sum+=(int)img_data[i * width + j];       //灰度值总和
		}
	}
	//计算每个像素值的点在整幅图像中的比例
	for (i = 0; i < GrayScale; i++)
	{
	pixelPro[i] = (float)pixelCount[i] / pixelSum;
	}
	//遍历灰度级[0,255]
	for (j = 0; j < GrayScale; j++)
	{
	w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
	u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值
	w1=1-w0;
	u1tmp=gray_sum/pixelSum-u0tmp;
	u0 = u0tmp / w0;              //背景平均灰度
	u1 = u1tmp / w1;              //前景平均灰度
	u = u0tmp + u1tmp;            //全局平均灰度
	deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
	if (deltaTmp > deltaMax)
	{
		deltaMax = deltaTmp;
		threshold = j;
	}
	if (deltaTmp < deltaMax)
	{
		break;
	}
	}
	return threshold;
}

/*---------------------------------------------------------------
 【函    数】Image_Blur
 【功    能】腐蚀滤波
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Image_Blur (void)
{
    int nr; //行
    int nc; //列

    for (nr = 1; nr < Image_H - 1; nr++)
    {
        for (nc = 1; nc < Image_W - 1; nc++)
        {
            if ((image_two_value[nr][nc] == IMG_BLACK)
                    && (image_two_value[nr - 1][nc] + image_two_value[nr + 1][nc] + image_two_value[nr][nc + 1] + image_two_value[nr][nc - 1] > 255*2))
            {
                image_two_value[nr][nc] = IMG_WHITE;
            }
            else if ((image_two_value[nr][nc] == IMG_WHITE)
                    && (image_two_value[nr - 1][nc] + image_two_value[nr + 1][nc] + image_two_value[nr][nc + 1] + image_two_value[nr][nc - 1] < 255*2))
            {
                image_two_value[nr][nc] = IMG_BLACK;
            }
        }
    }
}

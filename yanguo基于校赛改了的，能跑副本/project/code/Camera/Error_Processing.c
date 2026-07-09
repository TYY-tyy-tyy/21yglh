#include "Error_Processing.h"

/* 中线误差 */
int16 count = 0;  //加权个数
int16 Image_error = 0;  //中线误差

///* 权重1 */
int16 Image_Weight1[60] =
{   1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,3 ,3 ,5 ,7 ,9 ,
    11,13,15,17,19,21,21,19,17,15,
	13,11,9 ,7 ,5 ,3 ,3 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1
};

/* 权重2 */
int16 Image_Weight2[60] =
{   1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	3 ,3 ,5 ,7 ,9 ,11,13,15,17,19,
	21,21,19,17,15,13,11,9 ,7 ,5 ,
	3 ,3 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1
};

/* 权重3 */
int16 Image_Weight3[60] =
{   
	1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,
	1 ,1 ,1 ,1 ,1 ,3 ,3 ,5 ,7 ,9 ,
	11,13,15,17,19,21,21,19,17,15,
	13,11,9 ,7 ,5 ,3 ,3 ,1 ,1 ,1 ,
    1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1
};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     最后处理中点误差,包含了对元素的处理
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Mid_Error_Processing(void)
{
    /* 对中间白列的误差进行加权求和 */
	int16 i,j;
    count = 0;
    for(i = MT9V03X_H - 10,j = 59; i >= reference_col_farthest; i-=2,j--)
    {
		if(White_Column_MID > 100)
		{
			Image_error += Image_Weight1[j] * Road_Wide[i];
			count += Image_Weight3[j];
		}
        else if(White_Column_MID > 60)
		{
			Image_error += Image_Weight2[j] * Road_Wide[i];
			count += Image_Weight2[j];
		}
		else
		{
			Image_error += Image_Weight3[j] * Road_Wide[i];
			count += Image_Weight1[j];
		}
    }
	if(count == 0){count = 1;}
    Image_error /= count;//左正右负
	
	if(Find_Left_FLAG == Left_1)
	{
		if(Image_error > 0)
		{
			Image_error = -Image_error;
		}
	}
	
	if(Find_Left_FLAG >= Left_2 && Find_Left_FLAG < Left_6)
	{
		if(Image_error > 5)
		{
			Image_error = -Image_error;
		}
		else if(Image_error > 0 && Image_error <= 5)
		{
			Image_error = -5;
		}
	}
}

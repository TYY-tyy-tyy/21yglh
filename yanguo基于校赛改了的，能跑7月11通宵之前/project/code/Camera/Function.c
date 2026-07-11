#include "Function.h"

ContinueLine_NUM = 10;

//判断是否连续的函数
uint8 Right_ContinueLine(void)
{
	int16 i;
	uint8 bad_cnt = 0;
	uint8 valid_cnt = 0;
	for(i = MT9V03X_H-10; i > 60; i -= 2)
	{
		uint8 up = Right_Line[i + 4];
		uint8 now = Right_Line[i];
		uint8 drow = Right_Line[i - 4];
		if(abs(now-drow)>ContinueLine_NUM || abs(up-drow)>ContinueLine_NUM)
		{
			return 0;
		}
	}
	return 1;
}

//判断是否连续的函数
uint8 Left_ContinueLine(void)
{
	int16 i;
	uint8 bad_cnt = 0;
	uint8 valid_cnt = 0;
	for(i = MT9V03X_H-10; i > 60; i -= 2)
	{
		uint8 up = Left_Line[i + 4];
		uint8 now = Left_Line[i];
		uint8 drow = Left_Line[i - 4];
		if(abs(now-drow)>ContinueLine_NUM || abs(up-drow)>ContinueLine_NUM)
		{
			return 0;
		}
	}
	return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     功能函数--得到某区域的左/右边的丢线数
// 参数说明
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
int Count_Left_Lost(int start, int end)
{
    int i, t;
    int Left_lost_nums = 0;
    //自动调整位置
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (i = start; i >= end; i-=2)
    {
        //只找第一个符合条件的点
        if(Left_Line [i] == 8)
        {
            Left_lost_nums++;
        }
    }
    return Left_lost_nums;
}

int Count_Right_Lost(int start, int end)
{
    int i, t;
    int Right_lost_nums = 0;
    //自动调整位置
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (i = start; i >= end; i-=2)
    {
        //只找第一个符合条件的点
        if(Right_Line [i] == MT9V03X_W - 8)
        {
            Right_lost_nums++;
        }
    }
    return Right_lost_nums;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     功能函数--得到一维数组里的最大/最小值
// 参数说明     image
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8 find_extreme_Value(uint8* arr, uint8 start, uint8 end, int mode)
{

	uint8 extremeValue;
	uint8 extremeIndex = (uint8)start;
	uint8 i;
	// 查找最小值
	if (mode == 0)
	{
		extremeValue = MT9V03X_H;
		for (i = start + 1; i < end; i ++)
		{
			if (arr[i] < extremeValue)
			{
				 extremeValue = arr[i];
				 extremeIndex = i;
			}
		}
	}
	// 查找最大值
	else if (mode == 1)
	{
		extremeValue = 0;
		for (i = start + 1; i < end; i ++)
		{
			if (arr[i] > extremeValue)
			{
				 extremeValue = arr[i];
				 extremeIndex = i;
			}
		}
	}

	return extremeIndex;
}

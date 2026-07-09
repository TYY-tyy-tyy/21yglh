#include "Find_Patch.h"

/* 左右上角点位置 */
int16 Left_up_Patch = 0;
int16 Right_up_Patch = 0;

/* 左右下角点位置 */
int16 Left_dowm_Patch = 0;
int16 Left_dowm_Patch_x = 0;

int16 Right_dowm_Patch = 0;
int16 Right_dowm_Patch_x = 0;

/* 某行黑点的数量，用于丢线保护 */
int16 Black_counts;

/* 丢线保护标志位 */
int16 lost_line_protect = 0;

/*丢线数量*/
int16 Lost_Left_lost_nums = 0;
int16 Lost_Right_lost_nums = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找丢线数量
// 参数说明
// 返回参数     丢线的行数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Lost_connection_left(int16 start, int16 end)
{
	int16 i, t;
	Lost_Left_lost_nums = 0;
	Lost_Right_lost_nums = 0;
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
		if((Right_Line[i] - Left_Line[i]) >= Road_Width_Table[i]*12/10)
		{
			//左在设定范围外，右边在设定范围内
			if(Left_Line [i] <= Left_Coordinates[i]*8/10 && Right_Line [i] >= Left_Coordinates[i] && Right_Line [i] <= Right_Coordinates[i]*12/10)
			{
				Left_Line[i] = Right_Line[i] - Road_Width_Table[i];
				image_copy_out[i][Left_Line[i]] = 0;
			}
			//右在设定范围外，左边在设定范围内
			else if(Left_Line [i] >= Left_Coordinates[i]*8/10 && Left_Line [i] <= Right_Coordinates[i] && Right_Line [i] >= Right_Coordinates[i]*12/10)
			{
				Right_Line[i] = Left_Line[i] + Road_Width_Table[i];
				image_copy_out[i][Right_Line[i]] = 0;
			}
			//两边都在设定范围外
			else if(Left_Line [i] <= Left_Coordinates[i]*8/10 && Right_Line [i] >= Right_Coordinates[i]*12/10)
			{
				//两边丢线数量没有到达阈值
				if(Left_Lost_Line_count <= 20 && Right_Lost_Line_count <= 20)
				{
					Left_Line  [i] = Left_Coordinates[i];
					Right_Line [i] = Right_Coordinates[i];
					image_copy_out[i][Left_Line[i]] = 0;
					image_copy_out[i][Right_Line[i]] = 0;
				}
				//一边丢线数量到达阈值，且左边的多
				else if(Left_Lost_Line_count >= 20 && Left_Lost_Line_count >= Right_Lost_Line_count)
				{
					Right_Line[i] = Left_Line[i] + Road_Width_Table[i];
					image_copy_out[i][Right_Line[i]] = 0;
				}
				//一边丢线数量到达阈值，且右边的多
				else if(Right_Lost_Line_count >= 20 && Right_Lost_Line_count >= Left_Lost_Line_count)
				{
					Left_Line[i] = Right_Line[i] - Road_Width_Table[i];
					image_copy_out[i][Left_Line[i]] = 0;
				}
			}
		}
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找左上角点
// 参数说明
// 返回参数     角点位置
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 Find_left_up_point(int16 start, int16 end)//但是起始列和终止列不要用距离边界小于5的线
{
    int16 i, t;
    int16 Left_up_jiaodian_line = 0;

    //自动调整位置
    if (start > end)
    {
        t = start;
        start = end;
        end = t;
    }

    for (i = start; i <= end; i++)
    {
        if ((Left_up_jiaodian_line == 0) &&
           ((Left_Line[i] - Left_Line[i + 10]) >= 20) &&
           ((Left_Line[i] - Left_Line[i + 12]) >= 20) &&
           ((Left_Line[i] - Left_Line[i + 14]) >= 20))
        {
            Left_up_jiaodian_line = i;//获取行数即可
            break;
        }
    }
    return Left_up_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找右上角点
// 参数说明
// 返回参数     角点位置
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 Find_Right_up_point(int16 start, int16 end)//但是起始列和终止列不要用距离边界小于5的线
{
    int16 i, t;
    int16 Right_up_jiaodian_line = 0;

    //自动调整位置
    if (start > end)
    {
        t = start;
        start = end;
        end = t;
    }

    for (i = start; i <= end; i+=2)
    {
        //只找第一个符合条件的点
        if ((Right_up_jiaodian_line == 0) &&
           ((Right_Line[i + 10] - Right_Line[i]) >= 20) &&
           ((Right_Line[i + 12] - Right_Line[i]) >= 20) &&
           ((Right_Line[i + 14] - Right_Line[i]) >= 20))
        {
            Right_up_jiaodian_line = i;//获取行数即可
            break;
        }
    }
    return Right_up_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找左下角点
// 参数说明
// 返回参数     角点位置
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 Find_left_dowm_point(int16 start, int16 end)//但是起始列和终止列不要用距离边界小于5的线
{
    int16 i, t;
    int16 Left_down_jiaodian_line = 0;
    Left_dowm_Patch_x = 0;

    //自动调整位置
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (i = start; i >= end; i-=2)
    {
//        if ((Left_down_jiaodian_line == 0) &&
//             (Left_Line[i] > Left_Line[i - 1]) &&
//             (Left_Line[i] > Left_Line[i - 2]) &&
//             (Left_Line[i] > Left_Line[i - 3]))
        if ((Left_down_jiaodian_line == 0) &&
                   ((Left_Line[i] - Left_Line[i - 10]) >= 20) &&
                   ((Left_Line[i] - Left_Line[i - 12]) >= 20) &&
                   ((Left_Line[i] - Left_Line[i - 14]) >= 20))
        {
            Left_down_jiaodian_line = i;//获取行数即可
            /* 得到x */
            Left_dowm_Patch_x = Left_Line[Left_down_jiaodian_line];
            break;
        }
    }

    return Left_down_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找右下角点
// 参数说明
// 返回参数     角点位置
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 Find_Right_dowm_point(int16 start, int16 end)//但是起始列和终止列不要用距离边界小于5的线
{
    int16 i, t;
    int16 Right_down_jiaodian_line = 0;
    Right_dowm_Patch_x = 0;
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
//        if ((Right_down_jiaodian_line == 0) &&
//             (Right_Line[i] < Right_Line[i - 1]) &&
//             (Right_Line[i] < Right_Line[i - 2]) &&
//             (Right_Line[i] < Right_Line[i - 3]))
        if ((Right_down_jiaodian_line == 0) &&
                 ((Right_Line[i - 10] - Right_Line[i]) >= 20) &&
                 ((Right_Line[i - 12] - Right_Line[i]) >= 20) &&
                 ((Right_Line[i - 14] - Right_Line[i]) >= 20))
        {
            Right_down_jiaodian_line = i;//获取行数即可
            Right_dowm_Patch_x = Right_Line[Right_down_jiaodian_line];
            break;
        }
    }
    return Right_down_jiaodian_line;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算某行的白点数量
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 White_counts_weight(int16 p)
{
    int16 White_counts_weight = 0;
	
	White_counts_weight = Right_Line[p] - Left_Line[p];

	if(White_counts_weight < 40 && count1 > 1000)
	{
		COM_QY = 0;
	}
	
	return White_counts_weight;
}

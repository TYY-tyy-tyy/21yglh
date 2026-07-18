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
uint16 Black_counts;

/* 丢线保护标志位 */
int16 lost_line_protect = 0;

/*丢线数量*/
int16 Lost_Left_lost_nums = 0;
int16 Lost_Right_lost_nums = 0;

uint16 bmxian = 60;

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
           ((Left_Line[i] - Left_Line[i + 10]) >= 10) &&
           ((Left_Line[i] - Left_Line[i + 12]) >= 10) &&
           ((Left_Line[i] - Left_Line[i + 14]) >= 10))
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
           ((Right_Line[i + 10] - Right_Line[i]) >= 10) &&
           ((Right_Line[i + 12] - Right_Line[i]) >= 10) &&
           ((Right_Line[i + 14] - Right_Line[i]) >= 10))
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
                   ((Left_Line[i] - Left_Line[i - 10]) >= 10) &&
                   ((Left_Line[i] - Left_Line[i - 12]) >= 10) &&
                   ((Left_Line[i] - Left_Line[i - 14]) >= 10))
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
                 ((Right_Line[i - 10] - Right_Line[i]) >= 10) &&
                 ((Right_Line[i - 12] - Right_Line[i]) >= 10) &&
                 ((Right_Line[i - 14] - Right_Line[i]) >= 10))
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
// 备注信息		p=1为左，p=2为右
//------------------------------------------------------------------------------------------------------------------
int16 White_counts_h(int16 p)
{
    int16 h = 0;
	
	uint8 row;
	uint8 qyy = 0;
	
	int16 gray_point_1,gray_point_2,compare_value;
	
	if(p == 1)
	{
		for(row = MT9V03X_H - 10; row > 2; row -= 2)
		{
			if(h == 0)
			{
				//获取当前灰度值
				gray_point_1 = image_copy_out[row][Left_Line[110]];
				//获取对比度灰度值
				gray_point_2 = image_copy_out[row - 2][Left_Line[110]];
				//若对比点是白点
				if(gray_point_2 > white_max_point)
				{
					continue;
				}
				//若当前点是黑点，则直接找到最远端的距离
				if(gray_point_1 < white_min_point)
				{
					h = 1;
					break;
				}

				//计算对比度
				gray_point_2 = image_copy_out[row - 1][Left_Line[110]];
				compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

				if(compare_value > REFERENCE_CONTRAST)
				{
					h = 1;
					break;
				}
			}
			else
			{
				//获取当前灰度值
				gray_point_1 = image_copy_out[row][Left_Line[110]];
				//获取对比度灰度值
				gray_point_2 = image_copy_out[row - 2][Left_Line[110]];
				//若对比点是黑点
				if(gray_point_2 < white_min_point)
				{
					continue;
				}
				//若当前点是白点，则直接找到最远端的距离
				if(gray_point_1 > white_max_point)
				{
					h = 2;
					return h;
				}

				//计算对比度
				gray_point_2 = image_copy_out[row - 1][Left_Line[110]];
				compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

				if(abs(compare_value) > REFERENCE_CONTRAST)
				{
					h = 2;
					return h;
				}
			}
		}
	}
	else if(p == 2)
	{
		for(row = MT9V03X_H - 10; row > 2; row -= 2)
		{
			if(h == 0)
			{
				//获取当前灰度值
				gray_point_1 = image_copy_out[row][Right_Line[110]];
				gray_point_2 = image_copy_out[row - 2][Right_Line[110]];
				//若对比点是白点
				if(gray_point_2 > white_max_point)
				{
					continue;
				}
				//若当前点是黑点，则直接找到最远端的距离
				if(gray_point_1 < white_min_point)
				{
					h = 1;
					break;
				}

				//计算对比度
				gray_point_2 = image_copy_out[row - 1][Right_Line[110]];
				compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

				if(compare_value > REFERENCE_CONTRAST)
				{
					h = 1;
					break;
				}
			}
			else
			{
				//获取当前灰度值
				gray_point_1 = image_copy_out[row][Right_Line[110]];
				gray_point_2 = image_copy_out[row - 2][Right_Line[110]];
				//若对比点是黑点
				if(gray_point_2 < white_min_point)
				{
					continue;
				}
				//若当前点是白点，则直接找到最远端的距离
				if(gray_point_1 > white_max_point)
				{
					h = 2;
					return h;
				}

				//计算对比度
				gray_point_2 = image_copy_out[row - 1][Right_Line[110]];
				compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

				if(abs(compare_value) > REFERENCE_CONTRAST)
				{
					h = 2;
					return h;
				}
			}
		}
	}
	return h;
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

//	if(White_counts_weight < 40 && count1 > 1000)
//	{
//		COM_QY = 0;
//	}
	
	return White_counts_weight;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算某行的黑点数量
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于丢线保护
//------------------------------------------------------------------------------------------------------------------
void Black_counts_weight(int16 p)
{
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	int16 i;
	Black_counts = 0;
	for (i = 4; i < MT9V03X_W-4; i+=2)
	{
		//获取当前灰度值
		gray_point_1 = image_copy_out[p][i];
		//获取对比度灰度值
		gray_point_2 = image_copy_out[p][i+1];
		image_copy_out[p][i] = 0;
		
		//若当前点是黑点
		if(gray_point_1 < white_min_point)
		{
			Black_counts++;
			continue;
		}

		//计算对比度
		compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);
		if(compare_value > REFERENCE_CONTRAST)
		{
			Black_counts++;
			continue;
		}
	}

	if(Black_counts > 60 && ((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0)) && angle_pitch < 15 && angle_pitch > -15)
	{
		COM_QY = 3;
	}
}

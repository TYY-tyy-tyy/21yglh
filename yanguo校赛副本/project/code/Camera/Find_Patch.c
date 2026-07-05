#include "Find_Patch.h"

/* 左右上角点位置 */
int Left_up_Patch = 0;
int Right_up_Patch = 0;

/* 左右下角点位置 */
int Left_dowm_Patch = 0;
int Left_dowm_Patch_x = 0;

int Right_dowm_Patch = 0;
int Right_dowm_Patch_x = 0;

/* 某行黑点的数量，用于丢线保护 */
int Black_counts;

/* 丢线保护标志位 */
int lost_line_protect = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找左上角点
// 参数说明
// 返回参数     角点位置
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int Find_left_up_point(int start, int end)//但是起始列和终止列不要用距离边界小于5的线
{
    int i, t;
    int Left_up_jiaodian_line = 0;

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
int Find_Right_up_point(int start, int end)//但是起始列和终止列不要用距离边界小于5的线
{
    int i, t;
    int Right_up_jiaodian_line = 0;

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
int Find_left_dowm_point(int start, int end)//但是起始列和终止列不要用距离边界小于5的线
{
    int i, t;
    int Left_down_jiaodian_line = 0;
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
int Find_Right_dowm_point(int start, int end)//但是起始列和终止列不要用距离边界小于5的线
{
    int i, t;
    int Right_down_jiaodian_line = 0;
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
int White_counts_weight(int p)
{
	int16 gray_point_1 = 0, gray_point_2 = 0, gray_point_3 = 0,compare_value = 0;
    int White_counts_weight = 0;
	int16 i;
    //中往右数
    for (i = 94; i < MT9V03X_W - 4; i+=2)
    {
		//获取当前灰度值
		gray_point_1 = image_copy_out[p][i];
		//获取对比度灰度值
		gray_point_2 = image_copy_out[p][i+2];
		gray_point_3 = image_copy_out[p][i+4];
//		image_copy_out[row][col] = 255;
		//若对比点是白点
		if(gray_point_1 > white_min_point)
		{
			White_counts_weight++;
		}
		//若当前点是黑点，则直接找到最远端的距离
		else if(gray_point_2 < white_min_point && gray_point_3 < white_min_point)
		{
//			image_copy_out[row][col] = 0;
			break;
		}
    }
    //中往左数
    for (i = 94; i > 4; i-=2)
    {
		//获取当前灰度值
		gray_point_1 = image_copy_out[p][i];
		//获取对比度灰度值
		gray_point_2 = image_copy_out[p][i-2];
		gray_point_3 = image_copy_out[p][i-4];
//		image_copy_out[row][col] = 255;
		//若对比点是白点
		if(gray_point_1 > white_min_point)
		{
			White_counts_weight++;
		}
		//若当前点是黑点，则直接找到最远端的距离
		else if(gray_point_2 < white_min_point && gray_point_3 < white_min_point)
		{
//			image_copy_out[row][col] = 0;
			break;
		}
    }
    return White_counts_weight;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     计算某行的黑点数量
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于丢线保护
//------------------------------------------------------------------------------------------------------------------
void Black_counts_weight(int p)
{
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	int16 i;
	Black_counts = 0;
	for (i = 2; i < MT9V03X_W; i+=2)
	{
		//获取当前灰度值
		gray_point_1 = image_copy_out[p][i];
		//获取对比度灰度值
		gray_point_2 = image_copy_out[p][i+2];
//		image_copy_out[row][col] = 255;
		
		//若当前点是黑点
		if(gray_point_1 < white_min_point)
		{
			Black_counts++;
//			image_copy_out[row][col] = 0;
			continue;
		}

		//计算对比度
		gray_point_2 = image_copy_out[p][i-1];
		compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);
		if(abs(compare_value) > REFERENCE_CONTRAST)
		{
			Black_counts++;
//			image_copy_out[row][col] = 0;
			continue;
		}
	}

	if(Black_counts > 60)
	{
//		lost_line_protect = 1;
		COM_QY = 0;
	}
}

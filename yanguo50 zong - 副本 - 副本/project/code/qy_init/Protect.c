#include "Protect.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     丢线保护
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于丢线保护
//------------------------------------------------------------------------------------------------------------------
void Lost_Line_Protect(int p)
{
    int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	int16 i;
	Black_counts = 0;
	for (i = 4; i < MT9V03X_W-4; i+=2)
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
		/* 无刷标志位置0 */
        Blds_flag = 0;
		COM_QY = 0;
	}
}

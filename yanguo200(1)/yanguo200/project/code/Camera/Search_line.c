#include "Search_line.h"

/* 本图像的参考点的灰度值 */
uint8 reference_point[3] = {0};
/* 参考列的列数和每一行的数组 */
//uint8 reference_col = 0;
uint8 reference_col_line[MT9V03X_H] = {0};

/* 动态白点的最大值、最小值 */
uint8 white_max_point = 0;
uint8 white_min_point = 0;

int16 REFERENCE_CONTRAST = 5;

/* 存放每列最远端的数组 */
//uint8 remote_distance[MT9V03X_W] = {0};

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取参考点位
// 参数说明     image
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void get_reference_point(void)
{
	int H;
	int W;
    uint16 reference_point_Num = 0;     //统计点的总数量
    uint16 reference_point_Sum[2] = {0};     //统计点的合
    //计算总数量
    reference_point_Num = REFRENCE_ROW * 54;
    //60~56
    for(H = MT9V03X_H-15; H > (MT9V03X_H - 15 - REFRENCE_ROW*2); H-=2)
    {
        //20~74
        for(W = 40; W < 148; W+=2)
        {
            reference_point_Sum[0] +=  image_copy_out[H][W];
			if(reference_point_Sum[0] > 10000)
			{
				reference_point_Sum[0] = reference_point_Sum[0] - 10000;
				reference_point_Sum[1] += 1;
			}
        }
    }
    //求平均值
    reference_point[0] = (uint8) (reference_point_Sum[0] / reference_point_Num);
	reference_point[1] = (uint8) (reference_point_Sum[1]*1000 / reference_point_Num *10);
	reference_point[2] = reference_point[0] + reference_point[1];
    //限幅
    white_max_point = (uint8)func_limit_ab((uint16)reference_point[2] * WHITEMAXMUL / 100, BLACKPOINT, WHITEPOINT); //平均值 * 130%
    white_min_point = (uint8)func_limit_ab((uint16)reference_point[2] * WHITEMINMUL / 100, BLACKPOINT, WHITEPOINT); //平均值 * 70%

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     搜索图像参考列
// 参数说明     image
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void search_reference_col(void)
{
	int col,row;
	int H;
	uint8 qqy = 0;
	int16 last_row = 0;
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	//参考列清零
	for(col = 0; col < MT9V03X_W; col++)
	{
		//还原到最近端
		remote_distance[col] = MT9V03X_H - 1;
	}
	/* 右圆环特殊扫线 */
	if(Find_Right_FLAG == Right_4)
	{
		/* 寻找最长白列 */
		for(col = 94; col < (MT9V03X_W - 4); col += 6)
		{
			if(qqy == 0)
			{
				for(row = MT9V03X_H - 10; row > 0; row -= 2)
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[row][col];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[row - 2][col];
//					image_copy_out[row][col] = 255;
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point)
					{
//						image_copy_out[row][col] = 0;
						remote_distance[col] = (uint8)row;
						qqy = 1;
						break;
					}

					//计算对比度
					gray_point_2 = image_copy_out[row - 1][col];
					compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

					if(compare_value > REFERENCE_CONTRAST)
					{
//						image_copy_out[row][col] = 0;
						remote_distance[col] = (uint8)row+1;
						qqy = 1;
						break;
					}
				}
				last_row = row;
			}
			else 
			{
				row = last_row + 10;
				if(row > MT9V03X_H - 10)
				{
					row = MT9V03X_H - 10;
				}
				if(image_copy_out[row][col] > white_min_point)
				{
					for(; row > 0; row -= 2)
					{
						//获取当前灰度值
						gray_point_1 = image_copy_out[row][col];
						//获取对比度灰度值
						gray_point_2 = image_copy_out[row - 2][col];
//						image_copy_out[row][col] = 255;
						//若对比点是白点
						if(gray_point_2 > white_max_point)
						{
							continue;
						}
						//若当前点是黑点，则直接找到最远端的距离
						if(gray_point_1 < white_min_point)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row;
							break;
						}

						//计算对比度
						gray_point_2 = image_copy_out[row - 1][col];
						compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

						if(compare_value > REFERENCE_CONTRAST)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row+1;
							qqy = 1;
							break;
						}
					}
					last_row = row;
				}
				else
				{
					for(row = MT9V03X_H - 10; row > 0; row -= 2)
					{
						//获取当前灰度值
						gray_point_1 = image_copy_out[row][col];
						//获取对比度灰度值
						gray_point_2 = image_copy_out[row - 2][col];
//						image_copy_out[row][col] = 255;
						//若对比点是白点
						if(gray_point_2 > white_max_point)
						{
							continue;
						}
						//若当前点是黑点，则直接找到最远端的距离
						if(gray_point_1 < white_min_point)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row;
							break;
						}

						//计算对比度
						gray_point_2 = image_copy_out[row - 1][col];
						compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

						if(compare_value > REFERENCE_CONTRAST)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row+1;
							qqy = 1;
							break;
						}
					}
					last_row = row;
				}
			}
		}
	}
	else if(Find_Left_FLAG == Left_4)
	{
		for(col = 4; col < (MT9V03X_W - 94); col += 6)
		{
			if(qqy == 0)
			{
				for(row = MT9V03X_H - 10; row > 0; row -= 2)
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[row][col];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[row - 2][col];
//					image_copy_out[row][col] = 255;
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point)
					{
//						image_copy_out[row][col] = 0;
						remote_distance[col] = (uint8)row;
						qqy = 1;
						break;
					}

					//计算对比度
					gray_point_2 = image_copy_out[row - 1][col];
					compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

					if(compare_value > REFERENCE_CONTRAST)
					{
//						image_copy_out[row][col] = 0;
						remote_distance[col] = (uint8)row+1;
						qqy = 1;
						break;
					}
				}
				last_row = row;
			}
			else 
			{
				row = last_row + 10;
				if(row > MT9V03X_H - 10)
				{
					row = MT9V03X_H - 10;
				}
				if(image_copy_out[row][col] > white_min_point)
				{
					for(; row > 0; row -= 2)
					{
						//获取当前灰度值
						gray_point_1 = image_copy_out[row][col];
						//获取对比度灰度值
						gray_point_2 = image_copy_out[row - 2][col];
//						image_copy_out[row][col] = 255;
						//若对比点是白点
						if(gray_point_2 > white_max_point)
						{
							continue;
						}
						//若当前点是黑点，则直接找到最远端的距离
						if(gray_point_1 < white_min_point)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row;
							break;
						}

						//计算对比度
						gray_point_2 = image_copy_out[row - 1][col];
						compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

						if(compare_value > REFERENCE_CONTRAST)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row+1;
							qqy = 1;
							break;
						}
					}
					last_row = row;
				}
				else
				{
					for(row = MT9V03X_H - 10; row > 0; row -= 2)
					{
						//获取当前灰度值
						gray_point_1 = image_copy_out[row][col];
						//获取对比度灰度值
						gray_point_2 = image_copy_out[row - 2][col];
//						image_copy_out[row][col] = 255;
						//若对比点是白点
						if(gray_point_2 > white_max_point)
						{
							continue;
						}
						//若当前点是黑点，则直接找到最远端的距离
						if(gray_point_1 < white_min_point)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row;
							break;
						}

						//计算对比度
						gray_point_2 = image_copy_out[row - 1][col];
						compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

						if(compare_value > REFERENCE_CONTRAST)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row+1;
							qqy = 1;
							break;
						}
					}
					last_row = row;
				}
			}
		}
	}
	/* 常规取列 */
	else
	{
		for(col = 20; col < (MT9V03X_W-20); col += 4)
		{
			if(qqy == 0)
			{
				for(row = MT9V03X_H - 10; row > 2; row -= 2)
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[row][col];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[row - 2][col];
//					image_copy_out[row][col] = 255;
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point)
					{
//						image_copy_out[row][col] = 0;
						remote_distance[col] = (uint8)row;
						qqy = 1;
						break;
					}

					//计算对比度
					gray_point_2 = image_copy_out[row - 1][col];
					compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

					if(compare_value > REFERENCE_CONTRAST)
					{
//						image_copy_out[row][col] = 0;
						remote_distance[col] = (uint8)row+1;
						qqy = 1;
						break;
					}
				}
				last_row = row;
			}
			else 
			{
				row = last_row + 10;
				if(row > MT9V03X_H - 10)
				{
					row = MT9V03X_H - 10;
				}
				if(image_copy_out[row][col] > white_min_point)
				{
					for(; row > 2; row -= 2)
					{
						//获取当前灰度值
						gray_point_1 = image_copy_out[row][col];
						//获取对比度灰度值
						gray_point_2 = image_copy_out[row - 2][col];
//						image_copy_out[row][col] = 255;
						//若对比点是白点
						if(gray_point_2 > white_max_point)
						{
							continue;
						}
						//若当前点是黑点，则直接找到最远端的距离
						if(gray_point_1 < white_min_point)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row;
							break;
						}

						//计算对比度
						gray_point_2 = image_copy_out[row - 1][col];
						compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

						if(compare_value > REFERENCE_CONTRAST)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row+1;
							qqy = 1;
							break;
						}
					}
					last_row = row;
				}
				else
				{
					for(row = MT9V03X_H - 10; row > 2; row -= 2)
					{
						//获取当前灰度值
						gray_point_1 = image_copy_out[row][col];
						//获取对比度灰度值
						gray_point_2 = image_copy_out[row - 2][col];
//						image_copy_out[row][col] = 255;
						//若对比点是白点
						if(gray_point_2 > white_max_point)
						{
							continue;
						}
						//若当前点是黑点，则直接找到最远端的距离
						if(gray_point_1 < white_min_point)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row;
							break;
						}

						//计算对比度
						gray_point_2 = image_copy_out[row - 1][col];
						compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);

						if(compare_value > REFERENCE_CONTRAST)
						{
//							image_copy_out[row][col] = 0;
							remote_distance[col] = (uint8)row+1;
							qqy = 1;
							break;
						}
					}
					last_row = row;
				}
			}
		}
	}
	
	/* 得到参考列所在的列数和最远端 */
    reference_col = find_extreme_Value(remote_distance,2,MT9V03X_W - 4,0);

	//赋值
	reference_col_farthest = remote_distance[reference_col];
	for(H = reference_col_farthest; H < MT9V03X_H; H++)
	{
		reference_col_line[H] = reference_col;
//		image_copy_out[H][reference_col] = 0;
	}
	
	
}

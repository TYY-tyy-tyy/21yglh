#include "Find_Boundry_LongWhiteCol.h"

/* 边界相关变量声明 */
int16 Left_Line[MT9V03X_H]={0};
int16 Right_Line[MT9V03X_H]={0};
int16 Road_Wide[MT9V03X_H]={0};

/* 找最长白列相关变量声明 */
int16 White_Column_MID;

/* 丢线相关变量声明 */
int16 Right_Lost_Line_count;
int16 Left_Lost_Line_count;

/* 存放每列最远端的数组 */
uint8 remote_distance[MT9V03X_W] = {0};

/* 参考列所在的列数 */
uint8 reference_col = 0;
/* 参考列的最远端 */
uint8 reference_col_farthest = 0;

uint16 ALL_reference_col_farthest,ALL_White_Column_MID;


void Find_Boundry_LongWhiteCol(void)
{
	static int16 left_border = 0, right_border = 0, mid = MT9V03X_W / 2, last_mid = MT9V03X_W / 2;
	int16 H;
	int16 i;
	int16 j;
	int16 r_last_col = 0;//右
	int16 l_last_col = 0;//左
	uint8 r_qqy = 0,l_qqy = 0;
	int16 gray_point_1 = 0, gray_point_2 = 0,gray_point_3 = 0,gray_point_4 = 0,
	compare_value1 = 0,compare_value2 = 0,compare_value3 = 0,compare_value4 = 0;
	/* 变量初始化 */
	Right_Lost_Line_count = 0;
	Left_Lost_Line_count = 0;

	White_Column_MID = 0;

	/* 计算最长白列数量 */
	White_Column_MID = MT9V03X_H - reference_col_farthest;
	
	ALL_White_Column_MID = MT9V03X_H - ALL_reference_col_farthest;

	/* 左右边界数组初始化 */
	for(H = MT9V03X_H - 1; H >= 0; H--)
	{
		//还原到最近端,存放每列最远点的值都变回最近端点，即值都比较大
		Left_Line[H] = 0;
		Right_Line[H] = MT9V03X_W;
	}

	/* 根据参考列扫线 */
	for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
	{
		right_border = MT9V03X_W - 8;
		left_border = 8;
		//中线往右边扫
		if(r_qqy == 0)
		{
			for(j = reference_col; j < MT9V03X_W - 7; j+=2) //91
			{
				//获取当前灰度值
				gray_point_1 = image_copy_out[i][j];
				//获取对比度灰度值
				gray_point_2 = image_copy_out[i][j+1];
				gray_point_3 = image_copy_out[i][j+2];
				gray_point_4 = image_copy_out[i][j+3];
//				image_copy_out[i][j] = 255;
				right_border = j;
				if(j == MT9V03X_W - 8)
				{
					right_border = j;
					Right_Lost_Line_count++;  //右丢线数
				}
				//若对比点是白点
				if(gray_point_2 > white_max_point)
				{
					 continue;
				}
				//若当前点是黑点，则直接找到最远端的距离
				if(gray_point_1 < white_min_point 
					&& gray_point_2 < white_min_point 
					&& gray_point_3 < white_min_point
					&& gray_point_4 < white_min_point)
				{
//					image_copy_out[i][j] = 0;
					r_qqy = 1;
					right_border = j;
					break;
				}
				compare_value1 = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);
				compare_value2 = (gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3 + 1);
				compare_value3 = (gray_point_1 - gray_point_4) * 200 / (gray_point_1 + gray_point_4 + 1);
				compare_value4 = (gray_point_3 - gray_point_4) * 200 / (gray_point_3 + gray_point_4 + 1);
				if(compare_value1 > REFERENCE_CONTRAST 
					&& compare_value2 > REFERENCE_CONTRAST 
					&& compare_value3 > REFERENCE_CONTRAST
					&& compare_value4 < REFERENCE_CONTRAST)
				{
//					image_copy_out[i][j] = 0;
					r_qqy = 1;
					right_border = j;
					break;
				}
			}
			r_last_col = right_border;
		}
		else
		{
			j = r_last_col - 10;
			if(j < reference_col)
			{
				j = reference_col;
			}
			if(image_copy_out[i][j] > white_min_point)
			{
				for(; j < MT9V03X_W - 7; j+=2) //91
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[i][j];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[i][j+1];
					gray_point_3 = image_copy_out[i][j+2];
					gray_point_4 = image_copy_out[i][j+3];
//					image_copy_out[i][j] = 255;
					right_border = j;
					if(j == MT9V03X_W - 8)
					{
						right_border = j;
						Right_Lost_Line_count++;  //右丢线数
					}
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						 continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point 
						&& gray_point_2 < white_min_point 
						&& gray_point_3 < white_min_point
						&& gray_point_4 < white_min_point)
					{
//						image_copy_out[i][j] = 0;
						r_qqy = 1;
						right_border = j;
						break;
					}
					compare_value1 = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);
					compare_value2 = (gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3 + 1);
					compare_value3 = (gray_point_1 - gray_point_4) * 200 / (gray_point_1 + gray_point_4 + 1);
					compare_value4 = (gray_point_3 - gray_point_4) * 200 / (gray_point_3 + gray_point_4 + 1);
					if(compare_value1 > REFERENCE_CONTRAST 
						&& compare_value2 > REFERENCE_CONTRAST 
						&& compare_value3 > REFERENCE_CONTRAST
						&& compare_value4 < REFERENCE_CONTRAST)
					{
//						image_copy_out[i][j] = 0;
						r_qqy = 1;
						right_border = j;
						break;
					}
				}
				r_last_col = right_border;
			}
			else
			{
				for(j = reference_col; j < MT9V03X_W - 7; j+=2) //91
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[i][j];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[i][j+1];
					gray_point_3 = image_copy_out[i][j+2];
					gray_point_4 = image_copy_out[i][j+3];
					right_border = j;
//					image_copy_out[i][j] = 255;
					if(j == MT9V03X_W - 8)
					{
						right_border = j;
						Right_Lost_Line_count++;  //右丢线数
					}
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						 continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point 
						&& gray_point_2 < white_min_point 
						&& gray_point_3 < white_min_point
						&& gray_point_4 < white_min_point)
					{
//						image_copy_out[i][j] = 0;
						r_qqy = 1;
						right_border = j;
						break;
					}
					compare_value1 = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);
					compare_value2 = (gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3 + 1);
					compare_value3 = (gray_point_1 - gray_point_4) * 200 / (gray_point_1 + gray_point_4 + 1);
					compare_value4 = (gray_point_3 - gray_point_4) * 200 / (gray_point_3 + gray_point_4 + 1);
					if(compare_value1 > REFERENCE_CONTRAST 
						&& compare_value2 > REFERENCE_CONTRAST 
						&& compare_value3 > REFERENCE_CONTRAST
						&& compare_value4 < REFERENCE_CONTRAST)
					{
//						image_copy_out[i][j] = 0;
						r_qqy = 1;
						right_border = j;
						break;
					}
				}
				r_last_col = right_border;
			}
		}
		//中线往左扫
		if(l_qqy == 0)
		{
			for(j = reference_col; j > 6; j-=2)
			{
				//获取当前灰度值
				gray_point_1 = image_copy_out[i][j];
				//获取对比度灰度值
				gray_point_2 = image_copy_out[i][j-1];
				gray_point_3 = image_copy_out[i][j-2];
				gray_point_4 = image_copy_out[i][j-3];
//				image_copy_out[i][j] = 255;
				left_border = j;
				if(j == 8)
				{
					left_border = j;
					Left_Lost_Line_count++;  //左丢线数
				}
				//若对比点是白点
				if(gray_point_2 > white_max_point)
				{
					continue;
				}
				//若当前点是黑点，则直接找到最远端的距离
				if(gray_point_1 < white_min_point 
					&& gray_point_2 < white_min_point 
					&& gray_point_3 < white_min_point
					&& gray_point_4 < white_min_point)
				{
//					image_copy_out[i][j] = 0;
					l_qqy = 1;
					left_border = j;
					break;
				}
				compare_value1 = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);
				compare_value2 = (gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3 + 1);
				compare_value3 = (gray_point_1 - gray_point_4) * 200 / (gray_point_1 + gray_point_4 + 1);
				compare_value4 = (gray_point_3 - gray_point_4) * 200 / (gray_point_3 + gray_point_4 + 1);
				if(compare_value1 > REFERENCE_CONTRAST 
					&& compare_value2 > REFERENCE_CONTRAST 
					&& compare_value3 > REFERENCE_CONTRAST
					&& compare_value4 < REFERENCE_CONTRAST)
				{
//					image_copy_out[i][j] = 0;
					l_qqy = 1;
					left_border = j;
					break;
				}
			}
			l_last_col = left_border;
		}
		else 
		{
			j = l_last_col + 10;
			if(j > reference_col)
			{
				j = reference_col;
			}
			if(image_copy_out[i][j] > white_min_point)
			{
				for(; j > 6; j-=2) //91
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[i][j];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[i][j-1];
					gray_point_3 = image_copy_out[i][j-2];
					gray_point_4 = image_copy_out[i][j-3];
//					image_copy_out[i][j] = 255;
					left_border = j;
					if(j == 8)
					{
						left_border = j;
						Left_Lost_Line_count++;  //左丢线数
					}
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point 
						&& gray_point_2 < white_min_point 
						&& gray_point_3 < white_min_point
						&& gray_point_4 < white_min_point)
					{
//						image_copy_out[i][j] = 0;
						l_qqy = 1;
						left_border = j;
						break;
					}
					compare_value1 = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);
					compare_value2 = (gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3 + 1);
					compare_value3 = (gray_point_1 - gray_point_4) * 200 / (gray_point_1 + gray_point_4 + 1);
					compare_value4 = (gray_point_3 - gray_point_4) * 200 / (gray_point_3 + gray_point_4 + 1);
					if(compare_value1 > REFERENCE_CONTRAST 
						&& compare_value2 > REFERENCE_CONTRAST 
						&& compare_value3 > REFERENCE_CONTRAST
						&& compare_value4 < REFERENCE_CONTRAST)
					{
//						image_copy_out[i][j] = 0;
						l_qqy = 1;
						left_border = j;
						break;
					}
				}
				l_last_col = left_border;
			}
			else
			{
				for(j = reference_col; j > 6; j-=2)
				{
					//获取当前灰度值
					gray_point_1 = image_copy_out[i][j];
					//获取对比度灰度值
					gray_point_2 = image_copy_out[i][j-1];
					gray_point_3 = image_copy_out[i][j-2];
					gray_point_4 = image_copy_out[i][j-3];
//					image_copy_out[i][j] = 255;
					left_border = j;
					if(j == 8)
					{
						left_border = j;
						Left_Lost_Line_count++;  //左丢线数
					}
					//若对比点是白点
					if(gray_point_2 > white_max_point)
					{
						continue;
					}
					//若当前点是黑点，则直接找到最远端的距离
					if(gray_point_1 < white_min_point 
						&& gray_point_2 < white_min_point 
						&& gray_point_3 < white_min_point
						&& gray_point_4 < white_min_point)
					{
//						image_copy_out[i][j] = 0;
						l_qqy = 1;
						left_border = j;
						break;
					}
					compare_value1 = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2 + 1);
					compare_value2 = (gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3 + 1);
					compare_value3 = (gray_point_1 - gray_point_4) * 200 / (gray_point_1 + gray_point_4 + 1);
					compare_value4 = (gray_point_3 - gray_point_4) * 200 / (gray_point_3 + gray_point_4 + 1);
					if(compare_value1 > REFERENCE_CONTRAST 
						&& compare_value2 > REFERENCE_CONTRAST 
						&& compare_value3 > REFERENCE_CONTRAST
						&& compare_value4 < REFERENCE_CONTRAST)
					{
//						image_copy_out[i][j] = 0;
						l_qqy = 1;
						left_border = j;
						break;
					}
				}
				l_last_col = left_border;
			}
		}
		if(left_border > right_border - 10)
		{
			if(left_border > 94)
			{
				left_border = 8;
			}
			else if(right_border < 94)
			{
				right_border = MT9V03X_W - 8;
			}
		}
		else if(left_border > 168 || left_border < 0)
		{
			left_border = 8;
		}
		else if(right_border < 10 || right_border < 0)
		{
			right_border = MT9V03X_W - 8;
		}
		if(COM_QY == 0)
		{
//			image_copy_out[i  ][right_border] = 255;
//			image_copy_out[i-1][right_border] = 255;
//			image_copy_out[i  ][right_border+1] = 255;
//			image_copy_out[i-1][right_border+1] = 255;
//			image_copy_out[i  ][left_border] = 0;
//			image_copy_out[i-1][left_border] = 0;
//			image_copy_out[i  ][left_border-1] = 0;
//			image_copy_out[i-1][left_border-1] = 0;
		}
		//存放每行的边界
		Left_Line [i  ] = left_border ;		//左边线线数组
		Left_Line [i-1] = left_border ;     
		Right_Line[i  ] = right_border;
		Right_Line[i-1] = right_border;
	}
	
	/* 元素部分 */
    if((White_Column_MID >= 100) && (Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
    {
        Find_Target2(45, 60);
    }
    Find_Ring();
    if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
    {
        Find_Crossroads();
    }
	
    //根据前面的元素判断传入改变后的左右边界的x坐标，从而二次得到真实的中点坐标
	if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			mid = (Left_Line [i] + Right_Line[i]) / 2.0;                                                                //中线坐标
			last_mid = mid;

			//计算误差
			Road_Wide[i] = 94 - last_mid;//47

			//把中线像素点输出到图像
			if(COM_QY == 0)
			{
//				image_copy_out[i+1][last_mid+1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][last_mid+1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i+1][last_mid] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][last_mid] = 0; // i是第几行，mid是第几列
//				image_copy_out[i+1][last_mid-1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][last_mid-1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i+1][reference_col+1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col+1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i+1][reference_col] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col] = 0; // i是第几行，mid是第几列
//				image_copy_out[i+1][reference_col-1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col-1] = 0; // i是第几行，mid是第几列
			}
		}
	}
	else if((Find_Left_FLAG == Left_1))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			//计算误差
			Road_Wide[i] = Right_Coordinates[i] - Right_Line[i];//47
			
			last_mid = 94 - Road_Wide[i];

			//把中线像素点输出到图像
			if(COM_QY == 0)
			{
				image_copy_out[i+1][last_mid+1] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid+1] = 0; // i是第几行，mid是第几列
				image_copy_out[i+1][last_mid] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid] = 0; // i是第几行，mid是第几列
				image_copy_out[i+1][last_mid-1] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid-1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col] = 0;
			}
		}
	}
	else if((Find_Right_FLAG == Right_1))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			//计算误差
			Road_Wide[i] = Left_Coordinates[i] - Left_Line[i];//47
			
			last_mid = 94 - Road_Wide[i];

			//把中线像素点输出到图像
			if(COM_QY == 0)
			{
				image_copy_out[i+1][last_mid+1] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid+1] = 0; // i是第几行，mid是第几列
				image_copy_out[i+1][last_mid] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid] = 0; // i是第几行，mid是第几列
				image_copy_out[i+1][last_mid-1] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid-1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col] = 0;
			}
		}
	}
//	else if((Find_Left_FLAG == Left_4))
//	{
//		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
//		{
//			//计算误差
//			Road_Wide[i] = Right_Coordinates[i] - Right_Line[i];//47
//			
//			last_mid = 94 - Road_Wide[i];

//			//把中线像素点输出到图像
//			if(COM_QY == 0)
//			{
//				image_copy_out[i+1][last_mid] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][last_mid] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col] = 0;
//			}
//		}
//	}
//	else if((Find_Right_FLAG == Right_4))
//	{
//		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
//		{
//			//计算误差
//			Road_Wide[i] = Left_Coordinates[i] - Left_Line[i];//47
//			
//			last_mid = 94 - Road_Wide[i];

//			//把中线像素点输出到图像
//			if(COM_QY == 0)
//			{
//				image_copy_out[i+1][last_mid] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][last_mid] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col] = 0;
//			}
//		}
//	}
	else
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			mid = (Left_Line [i] + Right_Line[i]) / 2.0;                                                                //中线坐标
			last_mid = mid;

			//计算误差
			Road_Wide[i] = 94 - last_mid;//47

			//把中线像素点输出到图像
			if(COM_QY == 0)
			{
				image_copy_out[i+1][last_mid+1] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid+1] = 0; // i是第几行，mid是第几列
				image_copy_out[i+1][last_mid] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid] = 0; // i是第几行，mid是第几列
				image_copy_out[i+1][last_mid-1] = 0; // i是第几行，mid是第几列
				image_copy_out[i][last_mid-1] = 0; // i是第几行，mid是第几列
//				image_copy_out[i][reference_col] = 0;
			}
		}
	}

}

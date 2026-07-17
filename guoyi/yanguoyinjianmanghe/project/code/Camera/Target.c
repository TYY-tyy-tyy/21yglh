#include "Target.h"

uint8 remote_target[63] = {0};
int8 Find_Target_NUMS = 0;

void Find_Target1(void)
{
	int col,row,b_num = 0;
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	int8 i , j , eer_col1 = 0 ,eer_col2 = 0 , eer_col3 = 0, Target_NUM = 0 , laser_eer = 0 , start = 0, end = 0;
	//参考列清零
	for(col = 0; col < 36; col++)
	{
		//还原到最近端
		remote_target[col] = 0;
	}
	for(col = 31 ,j = 0; col < MT9V03X_W - 31; col += 2,j++)
	{
		for(row = MT9V03X_H - 10; row > 40; row -= 2)
		{
			remote_target[j] = (uint8)MT9V03X_H - row - 8;
			//获取当前灰度值
			gray_point_1 = image_copy_out[row][col];
			//获取对比度灰度值
			gray_point_2 = image_copy_out[row - 2][col];
//			image_copy_out[row][col] = 0;
			
			//若当前点是黑点，则直接找到最远端的距离
			if(gray_point_1 < white_min_point)
			{
				image_copy_out[row][col] = 255;
				break;
			}

			//计算对比度
			gray_point_2 = image_copy_out[row - 1][col];
			compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);

			if(compare_value > REFERENCE_CONTRAST)
			{
				remote_target[j] = (uint8)MT9V03X_H - row - 6;
				image_copy_out[row][col] = 255;
				break;
			}
		}
	}
//	if(remote_target[0] <= 15 || remote_target[41] <= 15)
//	{
//		return;
//	}
	Target_NUM = 0;
	for(i = 3;i < 59; i++)
	{
		if(Target_NUM == 0)
		{
			image_copy_out[41][i *2+31] = 255;
			eer_col1 = remote_target[i - 1] - remote_target[i];
			eer_col2 = remote_target[i - 2] - remote_target[i];
			eer_col3 = remote_target[i - 3] - remote_target[i];
			if((eer_col1 >= Target_EER || eer_col2 >= Target_EER) && eer_col3 >= Target_EER && remote_target[i] >= remote_target[i+1])
			{
				start = i *2+31;
				Target_NUM += 1;
				image_copy_out[40][start] = 255;
				i += 3;
				continue;
			}
		}
		else if(Target_NUM == 1)
		{
//			image_copy_out[41][i *2+31] = 0;
			eer_col1 = remote_target[i + 1] - remote_target[i];
			eer_col2 = remote_target[i + 2] - remote_target[i];
			eer_col3 = remote_target[i + 3] - remote_target[i];
			if((eer_col1 >= Target_EER || eer_col2 >= Target_EER) && eer_col3 >= Target_EER && remote_target[i] >= remote_target[i-1])
			{
				end = i *2+31;
				Target_NUM += 1;
				image_copy_out[40][end] = 255;
				continue;
			}
		}
		
		if(Target_NUM == 2)
		{
			laser_eer = (start + end)/2.0;
			image_copy_out[40][laser_eer] = 255;
			break;
		}
	}
	if(laser_eer!=0)
	{
		for(Target_NUM = 0,row = MT9V03X_H - 10; row > 40; row --)
		{
			//获取当前灰度值
			gray_point_1 = image_copy_out[row][laser_eer];
			//获取对比度灰度值
			gray_point_2 = image_copy_out[row - 1][laser_eer];
//			image_copy_out[row][laser_eer] = 255;
			
//			//若对比点是白点
//			if(gray_point_2 > white_max_point)
//			{
//				continue;
//			}
			//若当前点是黑点，则直接找到最远端的距离
			if(gray_point_1 < white_min_point)
			{
				image_copy_out[row][laser_eer] = 255;
				Target_NUM ++;
				row -= 4;
			}

			//计算对比度
			compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);

			if(compare_value > REFERENCE_CONTRAST)
			{
				image_copy_out[row][laser_eer] = 255;
				Target_NUM ++;
				row -= 4;
			}
		}
		if(Target_NUM >= 2)
		{
//			if(laser_eer >= 26 && laser_eer <= 35)
//			{
				laser_on(LASER_PIN_3);
//			}
//			else
//			{
//				all_off();
//			}
		}
		else
		{
			all_off();
		}
	}
	else
	{
		all_off();
	}
}

void Find_Target2(int p1,int p2)
{
	uint8 remote_target2[10] = {0};
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	int16 i,p,j = 0;
	int16 tar_eer = 0;
	Black_counts = 0;
	if(p1>p2)
	{
		p = p1;
		p1 = p2;
		p2 = p;
	}
	for(p = 80;p<81;p++)
	{
//	p = 80;
		for (i = Left_Line[p]+4; i < Right_Line[p]-4; i+=2)
		{
			if(i <= 4)
			{
				i = 4;
			}
			else if(i>=MT9V03X_W-4)
			{
				break;
			}
			//获取当前灰度值
			gray_point_1 = image_copy_out[p][i];
			//获取对比度灰度值
			gray_point_2 = image_copy_out[p][i+2];
	//		image_copy_out[row][col] = 255;
			
			//若当前点是黑点
			if(gray_point_1 < white_min_point)
			{
				remote_target2[j] = i;
				j++;
				Black_counts++;
				i+=6;
	//			image_copy_out[row][col] = 0;
				continue;
			}

			//计算对比度
			gray_point_2 = image_copy_out[p][i-1];
			compare_value = (gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);
			if(abs(compare_value) > REFERENCE_CONTRAST)
			{
				remote_target2[j] = i;
				j++;
				Black_counts++;
				i+=6;
	//			image_copy_out[row][col] = 0;
				continue;
			}
		}
	}

	if(Black_counts >= 2 && Black_counts < 6 && Find_Target_NUMS <= 5)
	{
		Find_Target_NUMS ++;
		if(remote_target2[2] == 0)
		{
			remote_target2[2] = remote_target2[0];
		}
		if(remote_target2[3] == 0)
		{
			remote_target2[3] = remote_target2[1];
		}
		if(remote_target2[4] == 0)
		{
			remote_target2[4] = remote_target2[0];
		}
		if(Find_Target_NUMS >= 0)
		{
			tar_eer = (remote_target2[0] + remote_target2[1] + remote_target2[2] + remote_target2[3] + remote_target2[4])/5;
			if(tar_eer >= 89 &&  tar_eer<= 99)
			{
				laser_on(LASER_PIN_3);
			}
//			else if((remote_target2[0] + remote_target2[1] + remote_target2[2] + remote_target2[3])/4 >= 80)
//			{
//				laser_on(LASER_PIN_5);
//			}
//			else if((remote_target2[0] + remote_target2[1] + remote_target2[2] + remote_target2[3])/4 <= 80)
//			{
//				laser_on(LASER_PIN_1);
//			}
//			else
//			{
//				all_off();
//			}
		}
	}
	else
	{
		Find_Target_NUMS = -2;
		all_off();
	}
}

#include "Target.h"

uint8 Target_REFERENCE_CONTRAST = 2;            

uint8 remote_target[63] = {0};

uint8 late_laser = 0;

void Find_Target1(void)
{
	int col,row,b_num = 0;
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	int8 i , j , eer_col1 = 0 ,eer_col2 = 0 , eer_col3 = 0, Target_NUM = 0 , start = 0, end = 0;
	int16 laser_eer = 0;
	//?ο???????
	for(col = 0; col < 63; col++)
	{
		//??????????
		remote_target[col] = 0;
	}
	for(col = 31 ,j = 0; col < MT9V03X_W - 31; col += 2,j++)
	{
		for(row = MT9V03X_H - 10; row > 40; row -= 2)
		{
			remote_target[j] = (uint8)MT9V03X_H - row - 8;
			//??????????
			gray_point_1 = image_copy_out[row][col];
			//??????????
			gray_point_2 = image_copy_out[row - 2][col];
//			image_copy_out[row][col] = 0;
			
			//????????????????????????????
			if(gray_point_1 < white_min_point)
			{
				image_copy_out[row][col] = 255;
				break;
			}

			//???????
			gray_point_2 = image_copy_out[row - 1][col];
			if(gray_point_1 + gray_point_2 == 0) continue;
			compare_value = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);

			if(compare_value > Target_REFERENCE_CONTRAST)
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
			//??????????
			gray_point_1 = image_copy_out[row][laser_eer];
			//??????????
			gray_point_2 = image_copy_out[row - 1][laser_eer];
//			image_copy_out[row][laser_eer] = 255;
			
//			//??????????
//			if(gray_point_2 > white_max_point)
//			{
//				continue;
//			}
			//????????????????????????????
			if(gray_point_1 < white_min_point)
			{
				image_copy_out[row][laser_eer] = 255;
				Target_NUM ++;
				row -= 4;
			}

			//???????
			if(gray_point_1 + gray_point_2 == 0) continue;
			compare_value = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);

			if(compare_value > Target_REFERENCE_CONTRAST)
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
	static int8 Find_Target_time = 0;
	uint8 remote_target[4][5] = {0};
	
	int16 gray_point_1 = 0, gray_point_2 = 0,compare_value = 0;
	
	int16 p,eer_p = 0;
	
	uint8 i,j,k;
	
	uint8 tar_eer[4] = {0};//靶的中心点坐标
	
	uint8 Black_counts[4] = {0};//当行寻到的边界
	
	uint8 Black_p[5] = {0};//0到3为判断有无靶（有1无0），4为寻到靶的行数
	
	uint8 Find_Target_oad[4] = {0};//当行寻到靶的位置
	
	uint8 Target_num[4] = {0};//当行寻靶中心点划分（5块区域）
	
	uint8 ADD_Target_v = 0;//所有靶坐标和
	
	uint8 l_edge;
	uint8 r_edge;
	
	if(p1>p2)
	{
		p = p1;
		p1 = p2;
		p2 = p;
	}
	eer_p = (p2-p1)/3;
	for(p = p1,k = 0;p <= p2 && k < 4;p += eer_p,k ++)
	{
		Target_num[k] = (Right_Line[p] - Left_Line[p])/5;
		l_edge=Left_Line[p]+4;
		r_edge=Right_Line[p]-4;
		j = 0;
		//从左向右
		for(i=l_edge;i<r_edge;i+=1)
		{
			gray_point_1 = image_copy_out[p][i];
			gray_point_2 = image_copy_out[p][i+1];
			if(gray_point_1 < white_min_point)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 255;
				break;
			}
			compare_value = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);
			if(compare_value > Target_REFERENCE_CONTRAST)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 255;
				break;
			}
		}
		//从右向左
		for(i=r_edge;i>l_edge;i-=1)
		{
			gray_point_1 = image_copy_out[p][i];
			gray_point_2 = image_copy_out[p][i-1];
			if(gray_point_1 < white_min_point)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 255;
				break;
			}
			compare_value = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);
			if(compare_value > Target_REFERENCE_CONTRAST)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				i+=4;
				image_copy_out[p][i] = 255;
				break;
			}
		}
	}
	
	for(p = 0;p < 4;p++)
	{
		if(Black_counts[p] == 2)
		{
			Black_p[p] = 1;
			Black_p[4]++;
		}
	}
	
	if(Black_p[4] >= 2)
	{
		if(Find_Target_time < 3) Find_Target_time ++;
		for(p = p1,k = 0;p <= p2 && k < 4;p += eer_p,k ++)
		{
			if(Black_p[k] != 0)
			{
				tar_eer[k] = (remote_target[k][0] + remote_target[k][1])/2;
				ADD_Target_v += tar_eer[k];
				if(tar_eer[k] >= Left_Line[p]+Target_num[k]*2 && tar_eer[k] <= Right_Line[p]-Target_num[k]*2)
				{
					Find_Target_oad[k] = 3;
				}
				else if(tar_eer[k] >= Left_Line[p]+Target_num[k]*1.5 && tar_eer[k] <= Left_Line[p]+Target_num[k]*2)
				{
					Find_Target_oad[k] = 2;
				}
				else if(tar_eer[k] >= Right_Line[p]-Target_num[k]*2 && tar_eer[k] <= Right_Line[p]-Target_num[k]*1.5)
				{
					Find_Target_oad[k] = 4;
				}
				else if(tar_eer[k] >= Left_Line[p]+8 && tar_eer[k] <= Left_Line[p]+Target_num[k]*1.5)
				{
					Find_Target_oad[k] = 1;
				}
				else if(tar_eer[k] >= Right_Line[p]-Target_num[k]*1.5 && tar_eer[p] <= Right_Line[p]-8)
				{
					Find_Target_oad[k] = 5;
				}
			}
		}
		if(Find_Target_time >= 0)
		{
			/* count votes from 4 scan lines per position */
			uint8 pos_votes[6] = {0};
			ADD_Target_v = ADD_Target_v/Black_p[4];
			for(k = 0; k < 4; k++)
			{
				if(Find_Target_oad[k] >= 1 && Find_Target_oad[k] <= 5)
					pos_votes[Find_Target_oad[k]]++;
			}
			/* priority: 3(mid) > 2(L-mid) > 4(R-mid) > 1(L) > 5(R) */
			if(pos_votes[3] >= 2)           
			{
				if(ADD_Target_v >= Left_Line[p2]+Target_num[p2]*2 && ADD_Target_v <= Right_Line[p2]-Target_num[p2]*2)
				{
					all_off(); 
					laser_on(LASER_PIN_3); 
					late_laser = 3;
				}
			}
			else if(pos_votes[2] >= 2)      
			{ 
				if(ADD_Target_v >= Left_Line[p2]+Target_num[p2]*1.5 && ADD_Target_v <= Left_Line[p2]+Target_num[p2]*2)
				{
					all_off(); 
					laser_on(LASER_PIN_2); 
					late_laser = 2;
				}
			}
			else if(pos_votes[4] >= 2)      
			{ 
				if(ADD_Target_v >= Right_Line[p2]-Target_num[p2]*2 && ADD_Target_v <= Right_Line[p2]-Target_num[p2]*1.5)
				{
					all_off(); 
					laser_on(LASER_PIN_4); 
					late_laser = 4;
				}
			}
			else if(pos_votes[1] >= 2)      
			{ 
				if(ADD_Target_v >= Left_Line[p2]+8 && ADD_Target_v <= Left_Line[p2]+Target_num[p2]*1.5)
				{
					all_off(); 
					laser_on(LASER_PIN_1); 
					late_laser = 1;
				}
			}
			else if(pos_votes[5] >= 2)      
			{ 
				if(ADD_Target_v >= Right_Line[p2]-Target_num[p2]*1.5 && ADD_Target_v <= Right_Line[p2]-8)
				{
					all_off(); 
					laser_on(LASER_PIN_5); 
					late_laser = 5;
				}
			}
			else 
			{ 
				all_off(); 
				late_laser = 0;
			}
		}
	}
	else
	{
		Find_Target_time = -3;
		all_off();
		late_laser = 0;
	}
}


//如果上一次有激光打出会产生曝光，故激光点位要降低曝光度
void LowerCameraExposure(void)
{
	uint8 col,row;
	if(late_laser == 1)
	{
		
	}
	if(late_laser == 2)
	{
		
	}
	if(late_laser == 3)
	{
		for(col = 72 ; col < 110 ;col ++ )
		{
			for(row = 72 ;row < 106 ;row ++)
			{
				if(image_copy_out[row][col] > 250)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.8;
				}
				if(image_copy_out[row][col] > 230)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.85;
				}
				if(image_copy_out[row][col] > 210)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.9;
				}
				if(image_copy_out[row][col] > 190)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.95;
				}
			}
		}
	}
	if(late_laser == 4)
	{
		
	}
	if(late_laser == 5)
	{
		for(col = 72 ; col < 110 ;col ++ )
		{
			for(row = 102 ;row < 128 ;row ++)
			{
				if(image_copy_out[row][col] > 250)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.8;
				}
				if(image_copy_out[row][col] > 230)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.85;
				}
				if(image_copy_out[row][col] > 210)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.9;
				}
				if(image_copy_out[row][col] > 190)
				{
					image_copy_out[row][col] = (uint8)image_copy_out[row][col]*0.95;
				}
			}
		}
	}
}

#include "Target.h"

/* Target gradient: 1/10 of main threshold for subtle target edges */
#define TARGET_GRADIENT_THRESHOLD  (GRADIENT_THRESHOLD / 8)

uint8 remote_target[63] = {0};

uint8 late_laser = 0;

void Find_Target1(void)
{
	int col,row,b_num = 0;
	int16 v_diff = 0;
	int8 i , j , eer_col1 = 0 ,eer_col2 = 0 , eer_col3 = 0, Target_NUM = 0 , start = 0, end = 0;
	int16 laser_eer = 0;

	for(col = 0; col < 63; col++)
	{
		remote_target[col] = 0;
	}
	for(col = 31 ,j = 0; col < MT9V03X_W - 31; col += 2,j++)
	{
		for(row = MT9V03X_H - 10; row > 40; row -= 2)
		{
			remote_target[j] = (uint8)MT9V03X_H - row - 8;

			// �ݶȼ��: �����ҵ�ǿ�Ҷȱ仯(Ŀ��߽�)
			v_diff = (int16)image_copy_out[row][col] - (int16)image_copy_out[row-2][col];
			if(MY_ABS(v_diff) > TARGET_GRADIENT_THRESHOLD)
			{
				remote_target[j] = (uint8)MT9V03X_H - row - 6;
				image_copy_out[row][col] = 255;
				break;
			}
		}
	}

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
			v_diff = (int16)image_copy_out[row][laser_eer] - (int16)image_copy_out[row-1][laser_eer];

			// �ݶȼ��: ǿ�Ҷȱ仯 = Ŀ��
			if(MY_ABS(v_diff) > TARGET_GRADIENT_THRESHOLD)
			{
				image_copy_out[row][laser_eer] = 255;
				Target_NUM ++;
				row -= 4;
			}
		}
		if(Target_NUM >= 2)
		{
				laser_on(LASER_PIN_3);
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

	int16 g = 0;

	int16 p,eer_p = 0;

	uint8 i,j,k;

	uint8 tar_eer[4] = {0};

	uint8 Black_counts[4] = {0};

	uint8 Find_Target_oad[4] = {0};

	uint8 Target_num[4] = {0};

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
		// ��������ɨ��: �ø�ˮƽ�ݶȼ��Ŀ��
		for(i=l_edge;i<r_edge;i+=1)
		{
			g = get_gradient_x(p, i);
			if(MY_ABS(g) > TARGET_GRADIENT_THRESHOLD)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 255;
				break;
			}
		}
		// ��������ɨ��
		for(i=r_edge;i>l_edge;i-=1)
		{
			g = get_gradient_x(p, i);
			if(MY_ABS(g) > TARGET_GRADIENT_THRESHOLD)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 255;
				break;
			}
		}
	}

	// ͶƱ�ж���(����)
	for(p = 0;p < 4;p++)
	{
		if(Black_counts[p] >= 2)
		{
			if(remote_target[p][1] - remote_target[p][0] > 1)
			{
				Black_counts[p]++;
			}
		}
		if(Black_counts[p] == 2)
		{
			Find_Target_oad[p] = 0;
		}
	}

	if(Black_counts[0] >= 2 || Black_counts[1] >= 2 || Black_counts[2] >= 2 || Black_counts[3] >= 2)
	{
		if(Find_Target_time < 3) Find_Target_time ++;
		for(p = p1,k = 0;p <= p2 && k < 4;p += eer_p,k ++)
		{
			if(Black_counts[k] >= 2)
			{
				tar_eer[k] = (remote_target[k][0] + remote_target[k][1])/2;
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
				else if(tar_eer[k] >= Right_Line[p]-Target_num[k]*1.5 && tar_eer[k] <= Right_Line[p]-8)
				{
					Find_Target_oad[k] = 5;
				}
			}
		}
		if(Find_Target_time >= 0)
		{
			uint8 pos_votes[6] = {0};
			for(k = 0; k < 4; k++)
			{
				if(Find_Target_oad[k] >= 1 && Find_Target_oad[k] <= 5)
					pos_votes[Find_Target_oad[k]]++;
			}
			if(pos_votes[3] >= 2)           { all_off(); laser_on(LASER_PIN_3); late_laser = 3;}
			else if(pos_votes[2] >= 2)      { all_off(); laser_on(LASER_PIN_2); late_laser = 2;}
			else if(pos_votes[4] >= 2)      { all_off(); laser_on(LASER_PIN_4); late_laser = 4;}
			else if(pos_votes[1] >= 2)      { all_off(); laser_on(LASER_PIN_1); late_laser = 1;}
			else if(pos_votes[5] >= 2)      { all_off(); laser_on(LASER_PIN_5); late_laser = 5;}
			else { all_off(); late_laser = 0;}
		}
	}
	else
	{
		Find_Target_time = -3;
		all_off();
		late_laser = 0;
	}
}

//�����һ���м�����������ع⣬�ʼ����λҪ�����ع��
void LowerCameraExposure(void)
{
	uint8 col,row;
	uint16 gray_point1 = 0,gray_point2 = 0;
	if(late_laser == 1)
	{
		for(col = 40 ; col < 84 ;col ++ )
		{
			for(row = 72 ;row < 110 ;row ++)
			{
				if(image_copy_out[row][col] > 240)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*80/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 220)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*85/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 200)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*90/100;
					image_copy_out[row][col] = gray_point2;
				}
			}
		}
	}
	if(late_laser == 2)
	{
		for(col = 66 ; col < 100 ;col ++ )
		{
			for(row = 72 ;row < 110 ;row ++)
			{
				if(image_copy_out[row][col] > 240)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*80/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 220)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*85/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 200)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*90/100;
					image_copy_out[row][col] = gray_point2;
				}
			}
		}
	}
	if(late_laser == 3)
	{
		for(col = 72 ; col < 116 ;col ++ )
		{
			for(row = 72 ;row < 110 ;row ++)
			{
				if(image_copy_out[row][col] > 240)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*80/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 220)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*85/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 200)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*90/100;
					image_copy_out[row][col] = gray_point2;
				}
			}
		}
	}
	if(late_laser == 4)
	{
		for(col = 88 ; col < 132 ;col ++ )
		{
			for(row = 72 ;row < 110 ;row ++)
			{
				if(image_copy_out[row][col] > 240)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*80/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 220)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*85/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 200)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*90/100;
					image_copy_out[row][col] = gray_point2;
				}
			}
		}
	}
	if(late_laser == 5)
	{
		for(col = 104 ; col < 148 ;col ++ )
		{
			for(row = 72 ;row < 72 ;row ++)
			{
				if(image_copy_out[row][col] > 240)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*80/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 220)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*85/100;
					image_copy_out[row][col] = gray_point2;
				}
				if(image_copy_out[row][col] > 200)
				{
					gray_point1 = image_copy_out[row][col];
					gray_point2 = gray_point1*90/100;
					image_copy_out[row][col] = gray_point2;
				}
			}
		}
	}
}

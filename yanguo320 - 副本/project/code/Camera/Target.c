#include "Target.h"

uint8 Target_REFERENCE_CONTRAST = 2;            

uint8 remote_target[63] = {0};

uint8 late_laser = 0;

uint8 Target_EER = 3;

int16 TARGET_BLACK_WIDTH_MAX = 15;
int16 TARGET_CENTER_DIFF_MAX = 5;

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
	static uint8 confirmed_pos = 0;   // 专做连续帧确认
	static int8 Find_Target_time = 0;
	uint8 remote_target[4][5] = {0};
	
	int16 gray_point_1 = 0, gray_point_2 = 0, gray_point_3 = 0,compare_value1 = 0,compare_value2 = 0;
	
	int16 p,eer_p = 0;
	
	uint8 i,j,k;
	
	uint8 tar_eer[4] = {0};
	
	uint8 Black_counts[4] = {0};
	
	uint8 Black_p[5] = {0};
	
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
	for(p = p1,k = 0;p <= p2 && k < 4;p += eer_p,k +=2)
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
			gray_point_3 = image_copy_out[p][i+2];
			if(gray_point_1 < white_min_point && gray_point_2 < white_min_point)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 255;
				break;
			}
			compare_value1 = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2+1);
			compare_value2 = (int16)(gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3+1);
			if(compare_value1 > Target_REFERENCE_CONTRAST && compare_value2 > Target_REFERENCE_CONTRAST)
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
			gray_point_3 = image_copy_out[p][i-2];
			if(gray_point_1 < white_min_point && gray_point_2 < white_min_point)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 0;
				break;
			}
			compare_value1 = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2+1);
			compare_value2 = (int16)(gray_point_1 - gray_point_3) * 200 / (gray_point_1 + gray_point_3+1);
			if(compare_value1 > Target_REFERENCE_CONTRAST && compare_value2 > Target_REFERENCE_CONTRAST)
			{
				remote_target[k][j] = i;
				j++;
				Black_counts[k]++;
				image_copy_out[p][i] = 0;
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
				if(tar_eer[k] >= Left_Line[p]+Target_num[k]*2 && tar_eer[k] <= Right_Line[p]-Target_num[k]*2)
				{
					Find_Target_oad[k] = 3;
				}
				else if(tar_eer[k] <= Left_Line[p]+Target_num[k]*12/10)
				{
					Find_Target_oad[k] = 1;
				}
				else if(tar_eer[k] >= Right_Line[p]-Target_num[k]*12/10)
				{
					Find_Target_oad[k] = 5;
				}
				else if(tar_eer[k] >= Left_Line[p]+Target_num[k]*12/10 && tar_eer[k] <= Left_Line[p]+Target_num[k]*2)
				{
					Find_Target_oad[k] = 2;
				}
				else if(tar_eer[k] >= Right_Line[p]-Target_num[k]*2 && tar_eer[k] <= Right_Line[p]-Target_num[k]*12/10)
				{
					Find_Target_oad[k] = 4;
				}
			}
		}
		if(Find_Target_time >= 0)
        {
            /* ===== ④ 投票 ===== */
            uint8 pos_votes[6] = {0};
            uint8 win_pos = 0;
			/* ===== ⑤ 投票平均中心 ===== */
            uint16 sum_center = 0;
            uint8  cnt_center = 0;
			uint8 avg_center;
            for(k = 0; k < 4; k++)
            {
                if(Find_Target_oad[k] >= 1 && Find_Target_oad[k] <= 5)
                    pos_votes[Find_Target_oad[k]]++;
            }
            if(pos_votes[3] >= 2)           win_pos = 3;
            else if(pos_votes[1] >= 2)      win_pos = 1;
            else if(pos_votes[5] >= 2)      win_pos = 5;
            else if(pos_votes[2] >= 2)      win_pos = 2;
            else if(pos_votes[4] >= 2)      win_pos = 4;
            for(k = 0; k < 4; k++)
            {
                if(Black_p[k] != 0) { sum_center += tar_eer[k]; cnt_center++; }
            }
            avg_center = (uint8)(sum_center / cnt_center);

            all_off();

            /* ===== ⑥ 空心靶验证 + 横向扫边界修正中心 + 一致性校验 ===== */
            if(verify_hollow_target_by_column(avg_center))
            {
                uint8 mid_k = 1;
                uint8 ref_p = p1 + mid_k * eer_p;
                int16 left_edge, right_edge;
				
				uint8 valid = 0;
                uint8 new_center = 0;

                /* 从 avg_center 向左右扫黑→白跳变，找实际边界 */
                for(left_edge = avg_center; left_edge > 4; left_edge--)
                {
                    if(image_copy_out[ref_p][left_edge] > white_min_point
                    && image_copy_out[ref_p][left_edge - 1] > white_min_point)
                        break;
                }
                for(right_edge = avg_center; right_edge < MT9V03X_W - 4; right_edge++)
                {
                    if(image_copy_out[ref_p][right_edge] > white_min_point
                    && image_copy_out[ref_p][right_edge + 1] > white_min_point)
                        break;
                }

                /* 宽度合理 + 和投票中心接近 → 采用修正中心；否则丢弃 */
                new_center = avg_center;
                if(right_edge - left_edge <= TARGET_BLACK_WIDTH_MAX)
                {
					uint8 diff;
                    new_center = (uint8)((left_edge + right_edge) / 2);
                    diff = (new_center > avg_center) ? (new_center - avg_center) : (avg_center - new_center);
                    if(diff <= TARGET_CENTER_DIFF_MAX) valid = 1;
                }

                if(valid)
                {
                    /* ===== ⑦ 用修正中心分位置 ===== */
                    uint8 cur_pos = 0;
					if(COM_QY == 0)
					{
						uint16 col = (p1 + p2)/2;
						image_copy_out[col-1][new_center-1] = 0;
						image_copy_out[col-1][new_center] = 0;
						image_copy_out[col-1][new_center+1] = 0;
						image_copy_out[col][new_center-1] = 0;
						image_copy_out[col][new_center] = 0;
						image_copy_out[col][new_center+1] = 0;
						image_copy_out[col+1][new_center-1] = 0;
						image_copy_out[col+1][new_center] = 0;
						image_copy_out[col+1][new_center+1] = 0;
					}
                    if(new_center >= Left_Line[ref_p] + Target_num[mid_k]*2
                    && new_center <= Right_Line[ref_p] - Target_num[mid_k]*2)
                        cur_pos = 3;
                    else if(new_center <= Left_Line[ref_p] + Target_num[mid_k]*12/10)
                        cur_pos = 1;
                    else if(new_center >= Right_Line[ref_p] - Target_num[mid_k]*12/10)
                        cur_pos = 5;
                    else if(new_center >= Left_Line[ref_p] + Target_num[mid_k]*12/10
                          && new_center <= Left_Line[ref_p] + Target_num[mid_k]*2)
                        cur_pos = 2;
                    else if(new_center >= Right_Line[ref_p] - Target_num[mid_k]*2
                          && new_center <= Right_Line[ref_p] - Target_num[mid_k]*12/10)
                        cur_pos = 4;

                    /* ===== ⑧ 三重确认：cur_pos==win_pos + 连续两帧一致 ===== */
                    if(cur_pos > 0 && cur_pos == win_pos && cur_pos == confirmed_pos)
                    {
                        all_off();
                        if(cur_pos == 3)      { laser_on(LASER_PIN_3); late_laser = 3; }
                        else if(cur_pos == 1) { laser_on(LASER_PIN_1); late_laser = 1; }
                        else if(cur_pos == 5) { laser_on(LASER_PIN_5); late_laser = 5; }
                        else if(cur_pos == 2) { laser_on(LASER_PIN_2); late_laser = 2; }
                        else if(cur_pos == 4) { laser_on(LASER_PIN_4); late_laser = 4; }
                    }
                    else { all_off(); late_laser = 0; }
                    confirmed_pos = cur_pos;
                }
                else { all_off(); late_laser = 0; confirmed_pos = 0; }
            }
            else { all_off(); late_laser = 0; confirmed_pos = 0; }
        }
    }
    else
    {
        Find_Target_time = -3;
        all_off();
        confirmed_pos = 0;
        late_laser = 0;
    }
}


//如果上一次有激光打出会产生曝光，故激光点位要降低曝光度
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
}

uint8 verify_hollow_target_by_column(uint8 col)
{
    uint8 row;
    uint8 state = 0;
    /* state: 0=等第一段黑, 1=在黑里等白（进入空心）, 2=在空心白里等第二段黑, 3=在黑里等白（离开靶） */
    int16 gray_point_1, gray_point_2, compare_value;

    for(row = MT9V03X_H - 10; row > reference_col_farthest + 10; row -= 2)
    {
        gray_point_1 = image_copy_out[row][col];
        gray_point_2 = image_copy_out[row - 1][col];

        if(gray_point_1 + gray_point_2 == 0) continue;
        compare_value = (int16)(gray_point_1 - gray_point_2) * 200 / (gray_point_1 + gray_point_2);

        if(state == 0)
        {
            /* 找白到黑的跳变：进入第一段黑（近边框） */
            if(gray_point_1 < white_min_point || compare_value > Target_REFERENCE_CONTRAST)
                state = 1;
        }
        else if(state == 1)
        {
            /* 找黑到白的跳变：离开边框，进入空心内部 */
            if(gray_point_1 > white_min_point || compare_value < -Target_REFERENCE_CONTRAST)
                state = 2;
        }
        else if(state == 2)
        {
            /* 找白到黑的跳变：进入第二段黑（远边框） */
            if(gray_point_1 < white_min_point || compare_value > Target_REFERENCE_CONTRAST)
                state = 3;
        }
        else /* state == 3 */
        {
            /* 找黑到白的跳变：离开靶 */
            if(gray_point_1 > white_min_point || compare_value < -Target_REFERENCE_CONTRAST)
                return 1;  /* 四段全找到：白→黑→白→黑→白，确认是空心靶 */
        }
    }
    return 0;  /* 没找齐四段 */
}


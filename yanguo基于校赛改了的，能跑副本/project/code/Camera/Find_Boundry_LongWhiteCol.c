#include "Find_Boundry_LongWhiteCol.h"

/* �߽���ر������� */
int16 Left_Line[MT9V03X_H]={0};
int16 Right_Line[MT9V03X_H]={0};
int16 Road_Wide[MT9V03X_H]={0};

/* ���������ر������� */
int16 White_Column_MID;

/* ������ر������� */
int16 Right_Lost_Line_count;
int16 Left_Lost_Line_count;

/* ���ÿ����Զ�˵����� */
uint8 remote_distance[MT9V03X_W] = {0};

/* �ο������ڵ����� */
uint8 reference_col = 0;
/* �ο��е���Զ�� */
uint8 reference_col_farthest = 0;


void Find_Boundry_LongWhiteCol(void)
{
	static int16 left_border = 0, right_border = 0, mid = MT9V03X_W / 2, last_mid = MT9V03X_W / 2;
	int16 H;
	int16 i;
	int16 j;
	int16 r_last_col = 0;//��
	int16 l_last_col = 0;//��
	uint8 r_qqy = 0,l_qqy = 0;
	int16 grad1 = 0, grad2 = 0;
	/* ������ʼ�� */
	Right_Lost_Line_count = 0;
	Left_Lost_Line_count = 0;

	White_Column_MID = 0;

	/* ������������� */
	White_Column_MID = MT9V03X_H - reference_col_farthest;

	/* ���ұ߽������ʼ�� */
	for(H = MT9V03X_H - 1; H >= 0; H--)
	{
		//��ԭ�������,���ÿ����Զ���ֵ���������˵㣬��ֵ���Ƚϴ�
		Left_Line[H] = 0;
		Right_Line[H] = MT9V03X_W;
	}

	/* �ݶȱ߽���: Sobelˮƽ�ݶȼ���滻ԭ���ĻҶ���ֵ/�Աȶȼ�� */
	for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
	{
		//==================== �ұ߽�ɨ�� ====================
		if(r_qqy == 0)
		{
			for(j = reference_col; j < MT9V03X_W - 7; j+=2)
			{
				if(j == MT9V03X_W - 8)
				{
					right_border = j;
					Right_Lost_Line_count++;
				}
				// �ݶȼ��: �ұ߽�Ϊ��������(�Ҷȱ䰵)
				grad1 = get_gradient_x(i, j);
				grad2 = get_gradient_x(i, j+2);
				if(grad1 < -GRADIENT_THRESHOLD && grad2 < -GRADIENT_THRESHOLD)
				{
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
			// �жϵ�ǰλ���Ƿ������
			grad1 = get_gradient_x(i, j);
			if(grad1 > -GRADIENT_THRESHOLD)
			{
				for(; j < MT9V03X_W - 7; j+=2)
				{
					if(j == MT9V03X_W - 8)
					{
						right_border = j;
						Right_Lost_Line_count++;
					}
					grad1 = get_gradient_x(i, j);
					grad2 = get_gradient_x(i, j+2);
					if(grad1 < -GRADIENT_THRESHOLD && grad2 < -GRADIENT_THRESHOLD)
					{
						r_qqy = 1;
						right_border = j;
						break;
					}
				}
				r_last_col = right_border;
			}
			else
			{
				for(j = reference_col; j < MT9V03X_W - 7; j+=2)
				{
					if(j == MT9V03X_W - 8)
					{
						right_border = j;
						Right_Lost_Line_count++;
					}
					grad1 = get_gradient_x(i, j);
					grad2 = get_gradient_x(i, j+2);
					if(grad1 < -GRADIENT_THRESHOLD && grad2 < -GRADIENT_THRESHOLD)
					{
						r_qqy = 1;
						right_border = j;
						break;
					}
				}
				r_last_col = right_border;
			}
		}

		//==================== ��߽�ɨ�� ====================
		if(l_qqy == 0)
		{
			for(j = reference_col; j > 6; j-=2)
			{
				if(j == 8)
				{
					left_border = j;
					Left_Lost_Line_count++;
				}
				// �ݶȼ��: ��߽�Ϊ�������(�Ҷȱ���)
				grad1 = get_gradient_x(i, j);
				grad2 = get_gradient_x(i, j-2);
				if(grad1 > GRADIENT_THRESHOLD && grad2 > GRADIENT_THRESHOLD)
				{
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
			grad1 = get_gradient_x(i, j);
			if(grad1 < GRADIENT_THRESHOLD)
			{
				for(; j > 6; j-=2)
				{
					if(j == 8)
					{
						left_border = j;
						Left_Lost_Line_count++;
					}
					grad1 = get_gradient_x(i, j);
					grad2 = get_gradient_x(i, j-2);
					if(grad1 > GRADIENT_THRESHOLD && grad2 > GRADIENT_THRESHOLD)
					{
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
					if(j == 8)
					{
						left_border = j;
						Left_Lost_Line_count++;
					}
					grad1 = get_gradient_x(i, j);
					grad2 = get_gradient_x(i, j-2);
					if(grad1 > GRADIENT_THRESHOLD && grad2 > GRADIENT_THRESHOLD)
					{
						l_qqy = 1;
						left_border = j;
						break;
					}
				}
				l_last_col = left_border;
			}
		}

		if(COM_QY == 0)
		{
			image_copy_out[i  ][right_border] = 0;
			image_copy_out[i-1][right_border] = 0;
			image_copy_out[i  ][left_border] = 0;
			image_copy_out[i-1][left_border] = 0;
		}
		//���ÿ�еı߽�
		Left_Line [i  ] = left_border ;
		Left_Line [i-1] = left_border ;
		Right_Line[i  ] = right_border;
		Right_Line[i-1] = right_border;
	}

	/* Ԫ�ز��� */
    if((White_Column_MID >= 100) && (Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
    {
        Find_Target2(52, 70);
    }
    Find_Ring();
    if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
    {
        Find_Crossroads();
    }

    //����ǰ���Ԫ���жϴ���ı������ұ߽��x���꣬�Ӷ����εõ���ʵ���е�����
	if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			mid = (Left_Line [i] + Right_Line[i]) / 2.0;
			last_mid = mid;
			Road_Wide[i] = 94 - last_mid;
			if(COM_QY == 0)
			{
				image_copy_out[i][last_mid] = 0;
			}
		}
	}
	else if((Find_Left_FLAG == Left_1) || (Find_Left_FLAG == Left_6))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			Road_Wide[i] = Right_Coordinates[i] - Right_Line[i];
			last_mid = 94 - Road_Wide[i];
			image_copy_out[i][last_mid] = 0;
			image_copy_out[i][reference_col] = 0;
		}
	}
	else if((Find_Right_FLAG == Right_1) || (Find_Right_FLAG == Right_6))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			Road_Wide[i] = Left_Coordinates[i] - Left_Line[i];
			last_mid = 94 - Road_Wide[i];
			image_copy_out[i][last_mid] = 0;
			image_copy_out[i][reference_col] = 0;
		}
	}
	else if((Find_Left_FLAG == Left_4))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			Road_Wide[i] = Right_Coordinates[i] - Right_Line[i];
			last_mid = 94 - Road_Wide[i];
			image_copy_out[i][last_mid] = 0;
			image_copy_out[i][reference_col] = 0;
		}
	}
	else if((Find_Right_FLAG == Right_4))
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			Road_Wide[i] = Left_Coordinates[i] - Left_Line[i];
			last_mid = 94 - Road_Wide[i];
			image_copy_out[i][last_mid] = 0;
			image_copy_out[i][reference_col] = 0;
		}
	}
	else
	{
		for(i = MT9V03X_H - 10; i >= reference_col_farthest; i-=2)
		{
			mid = (Left_Line [i] + Right_Line[i]) / 2.0;
			last_mid = mid;
			Road_Wide[i] = 94 - last_mid;
			if(COM_QY == 0)
			{
				image_copy_out[i][last_mid] = 0;
			}
		}
	}

}

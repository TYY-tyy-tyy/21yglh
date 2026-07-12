#include "Search_line.h"

/* ��ͼ��Ĳο���ĻҶ�ֵ */
uint8 reference_point[3] = {0};
/* �ο��е�������ÿһ�е����� */
//uint8 reference_col = 0;
uint8 reference_col_line[MT9V03X_H] = {0};

/* ��̬�׵�����ֵ����Сֵ */
uint8 white_max_point = 0;
uint8 white_min_point = 0;

int16 REFERENCE_CONTRAST = 5;
int16 GRADIENT_THRESHOLD = GRADIENT_THRESHOLD_DEF;

/* ���ÿ����Զ�˵����� */
//uint8 remote_distance[MT9V03X_W] = {0};

//-------------------------------------------------------------------------------------------------------------------
// �������     Sobelˮƽ�ݶȼ��� (3x3����)
// ����˵��     row, col -- ��������
// ���ز���     �ݶ�ֵ����ֵ=���ұ���, ��ֵ=���ұ䰵
//-------------------------------------------------------------------------------------------------------------------
int16 get_gradient_x(uint8 row, uint8 col)
{
    int16 g = 0;
    if(row > 0 && row < MT9V03X_H - 1 && col > 0 && col < MT9V03X_W - 1)
    {
        g -= image_copy_out[row-1][col-1];
        g += image_copy_out[row-1][col+1];
        g -= (int16)image_copy_out[row][col-1] * 2;
        g += (int16)image_copy_out[row][col+1] * 2;
        g -= image_copy_out[row+1][col-1];
        g += image_copy_out[row+1][col+1];
    }
    return g;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ�ο���λ
// ����˵��     image
// ����˵��
// ���ز���
// ʹ��ʾ��
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void get_reference_point(void)
{
	int H;
	int W;
    uint16 reference_point_Num = 0;     //ͳ�Ƶ��������
    uint16 reference_point_Sum[2] = {0};     //ͳ�Ƶ�ĺ�
    //60~56
    for(H = MT9V03X_H-5; H > (MT9V03X_H - 5 - REFRENCE_ROW); H-=1)
    {
        //20~74
//		if((Left_Lost_Line_count < 10 && Right_Lost_Line_count < 10) || (Left_Lost_Line_count >= 10 && Right_Lost_Line_count >= 10))
//		{
			for(W = 85; W < 103; W+=1)
			{
				if(image_copy_out[H][W] > WHITEPOINT)
				{
					image_copy_out[H][W] = WHITEPOINT;
				}
				else if(image_copy_out[H][W] < BLACKPOINT)
				{
					break;
				}
				reference_point_Sum[0] +=  image_copy_out[H][W];
				if(reference_point_Sum[0] > 10000)
				{
					reference_point_Sum[0] = reference_point_Sum[0] - 10000;
					reference_point_Sum[1] += 1;
				}
				reference_point_Num ++ ;
			}
//		}
//		else if((Left_Lost_Line_count < 10 && Right_Lost_Line_count >= 10))
//		{
//			for(W = 110; W < 138; W+=1)
//			{
//				reference_point_Sum[0] +=  image_copy_out[H][W];
//				if(reference_point_Sum[0] > 10000)
//				{
//					reference_point_Sum[0] = reference_point_Sum[0] - 10000;
//					reference_point_Sum[1] += 1;
//				}
//			}
//		}
//		else if((Left_Lost_Line_count >= 10 && Right_Lost_Line_count < 10))
//		{
//			for(W = 50; W < 78; W+=1)
//			{
//				reference_point_Sum[0] +=  image_copy_out[H][W];
//				if(reference_point_Sum[0] > 10000)
//				{
//					reference_point_Sum[0] = reference_point_Sum[0] - 10000;
//					reference_point_Sum[1] += 1;
//				}
//			}
//		}
    }
    //��ƽ��ֵ
    reference_point[0] = (uint8) (reference_point_Sum[0] / (reference_point_Num + 1));
	reference_point[1] = (uint8) (reference_point_Sum[1]*1000 / (reference_point_Num *10) + 1);
	reference_point[2] = reference_point[0] + reference_point[1];
    //�޷�
    white_max_point = (uint8)((uint16)reference_point[2] * WHITEMAXMUL / 100); //ƽ��ֵ * 130%
    white_min_point = (uint8)((uint16)reference_point[2] * WHITEMINMUL / 100); //ƽ��ֵ * 70%
	if(white_max_point > WHITEPOINT)
	{
		white_max_point = WHITEPOINT;
	}
	else if(white_max_point < BLACKPOINT)
	{
		white_max_point = BLACKPOINT;
	}
	if(white_min_point > WHITEPOINT)
	{
		white_min_point = WHITEPOINT;
	}
	else if(white_min_point < BLACKPOINT)
	{
		white_min_point = BLACKPOINT;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����ݶ�ɨ����ο���
// ����˵��     ʹ�ô�ֱ�ݶȲ��滻ԭ���ĻҶ���ֵ/�Աȶȼ��
//-------------------------------------------------------------------------------------------------------------------
void search_reference_col(void)
{
	int col, row, H;
	int16 v_diff;

	/* ��ʼ�� remote_distance ΪĬ��ֵ(�ײ�) */
	for(col = 0; col < MT9V03X_W; col++)
	{
		remote_distance[col] = MT9V03X_H - 1;
	}

	/* �����ݶ�ɨ�裺ÿ�д��µ�����ɨ�裬��ֱ�ݶȱ仯 = �����յ� */
	if(Find_Right_FLAG == Right_4)
	{
		for(col = 94; col < (MT9V03X_W - 4); col += 6)
		{
			for(row = MT9V03X_H - 10; row > 10; row -= 2)
			{
				v_diff = MY_ABS((int16)image_copy_out[row][col] - (int16)image_copy_out[row-4][col]);
				if(v_diff > GRADIENT_THRESHOLD)
				{
					remote_distance[col] = (uint8)row;
					break;
				}
			}
		}
	}
	else if(Find_Left_FLAG == Left_4)
	{
		for(col = 4; col < (MT9V03X_W - 94); col += 6)
		{
			for(row = MT9V03X_H - 10; row > 10; row -= 2)
			{
				v_diff = MY_ABS((int16)image_copy_out[row][col] - (int16)image_copy_out[row-4][col]);
				if(v_diff > GRADIENT_THRESHOLD)
				{
					remote_distance[col] = (uint8)row;
					break;
				}
			}
		}
	}
	/* Ĭ��ȫ������ */
	else
	{
		for(col = 20; col < (MT9V03X_W - 20); col += 4)
		{
			for(row = MT9V03X_H - 10; row > 10; row -= 2)
			{
				v_diff = MY_ABS((int16)image_copy_out[row][col] - (int16)image_copy_out[row-4][col]);
				if(v_diff > GRADIENT_THRESHOLD)
				{
					remote_distance[col] = (uint8)row;
					break;
				}
			}
		}
	}

	/* �õ��ο���: ѡȡ��Ұ��Զ(remote_distance��С)���� */
    reference_col = find_extreme_Value(remote_distance, 2, MT9V03X_W - 4, 0);

	reference_col_farthest = remote_distance[reference_col];
	for(H = reference_col_farthest; H < MT9V03X_H; H++)
	{
		reference_col_line[H] = reference_col;
	}
}

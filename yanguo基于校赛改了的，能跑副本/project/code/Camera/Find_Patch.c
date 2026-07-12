#include "Find_Patch.h"

/* �����Ͻǵ�λ�� */
int16 Left_up_Patch = 0;
int16 Right_up_Patch = 0;

/* �����½ǵ�λ�� */
int16 Left_dowm_Patch = 0;
int16 Left_dowm_Patch_x = 0;

int16 Right_dowm_Patch = 0;
int16 Right_dowm_Patch_x = 0;

/* ĳ�кڵ�����������ڶ��߱��� */
int16 Black_counts;

/* ���߱�����־λ */
int16 lost_line_protect = 0;

/*��������*/
int16 Lost_Left_lost_nums = 0;
int16 Lost_Right_lost_nums = 0;

//-------------------------------------------------------------------------------------------------------------------
// �������     �Ҷ�������
// ����˵��
// ���ز���     ���ߵ�����
// ʹ��ʾ��
// ��ע��Ϣ
//------------------------------------------------------------------------------------------------------------------
void Lost_connection_left(int16 start, int16 end)
{
	int16 i, t;
	Lost_Left_lost_nums = 0;
	Lost_Right_lost_nums = 0;
    //�Զ�����λ��
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
	for (i = start; i >= end; i-=2)
    {
        //ֻ�ҵ�һ�����������ĵ�
		if((Right_Line[i] - Left_Line[i]) >= Road_Width_Table[i]*12/10)
		{
			//�����趨��Χ�⣬�ұ����趨��Χ��
			if(Left_Line [i] <= Left_Coordinates[i]*8/10 && Right_Line [i] >= Left_Coordinates[i] && Right_Line [i] <= Right_Coordinates[i]*12/10)
			{
				Left_Line[i] = Right_Line[i] - Road_Width_Table[i];
				image_copy_out[i][Left_Line[i]] = 0;
			}
			//�����趨��Χ�⣬������趨��Χ��
			else if(Left_Line [i] >= Left_Coordinates[i]*8/10 && Left_Line [i] <= Right_Coordinates[i] && Right_Line [i] >= Right_Coordinates[i]*12/10)
			{
				Right_Line[i] = Left_Line[i] + Road_Width_Table[i];
				image_copy_out[i][Right_Line[i]] = 0;
			}
			//���߶����趨��Χ��
			else if(Left_Line [i] <= Left_Coordinates[i]*8/10 && Right_Line [i] >= Right_Coordinates[i]*12/10)
			{
				//���߶�������û�е�����ֵ
				if(Left_Lost_Line_count <= 20 && Right_Lost_Line_count <= 20)
				{
					Left_Line  [i] = Left_Coordinates[i];
					Right_Line [i] = Right_Coordinates[i];
					image_copy_out[i][Left_Line[i]] = 0;
					image_copy_out[i][Right_Line[i]] = 0;
				}
				//һ�߶�������������ֵ������ߵĶ�
				else if(Left_Lost_Line_count >= 20 && Left_Lost_Line_count >= Right_Lost_Line_count)
				{
					Right_Line[i] = Left_Line[i] + Road_Width_Table[i];
					image_copy_out[i][Right_Line[i]] = 0;
				}
				//һ�߶�������������ֵ�����ұߵĶ�
				else if(Right_Lost_Line_count >= 20 && Right_Lost_Line_count >= Left_Lost_Line_count)
				{
					Left_Line[i] = Right_Line[i] - Road_Width_Table[i];
					image_copy_out[i][Left_Line[i]] = 0;
				}
			}
		}
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����Ͻǵ�
//------------------------------------------------------------------------------------------------------------------
int16 Find_left_up_point(int16 start, int16 end)
{
    int16 i, t;
    int16 Left_up_jiaodian_line = 0;
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
            Left_up_jiaodian_line = i;
            break;
        }
    }
    return Left_up_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����Ͻǵ�
//------------------------------------------------------------------------------------------------------------------
int16 Find_Right_up_point(int16 start, int16 end)
{
    int16 i, t;
    int16 Right_up_jiaodian_line = 0;
    if (start > end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (i = start; i <= end; i+=2)
    {
        if ((Right_up_jiaodian_line == 0) &&
           ((Right_Line[i + 10] - Right_Line[i]) >= 20) &&
           ((Right_Line[i + 12] - Right_Line[i]) >= 20) &&
           ((Right_Line[i + 14] - Right_Line[i]) >= 20))
        {
            Right_up_jiaodian_line = i;
            break;
        }
    }
    return Right_up_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����½ǵ�
//------------------------------------------------------------------------------------------------------------------
int16 Find_left_dowm_point(int16 start, int16 end)
{
    int16 i, t;
    int16 Left_down_jiaodian_line = 0;
    Left_dowm_Patch_x = 0;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (i = start; i >= end; i-=2)
    {
        if ((Left_down_jiaodian_line == 0) &&
                   ((Left_Line[i] - Left_Line[i - 10]) >= 20) &&
                   ((Left_Line[i] - Left_Line[i - 12]) >= 20) &&
                   ((Left_Line[i] - Left_Line[i - 14]) >= 20))
        {
            Left_down_jiaodian_line = i;
            Left_dowm_Patch_x = Left_Line[Left_down_jiaodian_line];
            break;
        }
    }
    return Left_down_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �����½ǵ�
//------------------------------------------------------------------------------------------------------------------
int16 Find_Right_dowm_point(int16 start, int16 end)
{
    int16 i, t;
    int16 Right_down_jiaodian_line = 0;
    Right_dowm_Patch_x = 0;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (i = start; i >= end; i-=2)
    {
        if ((Right_down_jiaodian_line == 0) &&
                 ((Right_Line[i - 10] - Right_Line[i]) >= 20) &&
                 ((Right_Line[i - 12] - Right_Line[i]) >= 20) &&
                 ((Right_Line[i - 14] - Right_Line[i]) >= 20))
        {
            Right_down_jiaodian_line = i;
            Right_dowm_Patch_x = Right_Line[Right_down_jiaodian_line];
            break;
        }
    }
    return Right_down_jiaodian_line;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ��ֱ�ݶȼ��ĳ�еİ׵���� (�滻ԭ����ֵ/�Աȶȼ��)
// ����˵��     p = 1: ɨ������, p = 2: ɨ���ұ�
// ���ز���     h=1 �ҵ��ڵ�(����), h=2 �ҵ���(���)
//------------------------------------------------------------------------------------------------------------------
int16 White_counts_h(int16 p)
{
    int16 h = 0;
    uint8 row;
    int16 v_diff;
    int16 col;

    if(p == 1)
        col = Left_Line[110];
    else
        col = Right_Line[110];

    for(row = MT9V03X_H - 10; row > 2; row -= 2)
    {
        v_diff = (int16)image_copy_out[row][col] - (int16)image_copy_out[row-2][col];

        if(h == 0)
        {
            // Ѱ�ҵ�һ�����ɣ���������(����->�ڵ�)
            if(my_abs(v_diff) > GRADIENT_THRESHOLD && v_diff > 0)
            {
                h = 1;
                break;
            }
        }
        else
        {
            // Ѱ�ҵڶ������ɣ��ɰ�����(�ڵ�->����)
            if(my_abs(v_diff) > GRADIENT_THRESHOLD && v_diff < 0)
            {
                h = 2;
                return h;
            }
        }
    }
    return h;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ����ĳ�еİ׵�����
//------------------------------------------------------------------------------------------------------------------
int16 White_counts_weight(int16 p)
{
    int16 White_counts_weight = 0;
    White_counts_weight = Right_Line[p] - Left_Line[p];
    return White_counts_weight;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     �ݶȼ��ĳ�еĺڵ����� (�滻ԭ����ֵ/�Աȶȼ��)
// ����˵��     �����ݶȼ�⣬|grad| > GRADIENT_THRESHOLD ��Ϊ�ҵ��߽�
//------------------------------------------------------------------------------------------------------------------
void Black_counts_weight(int p)
{
    int16 i;
    int16 g;
    Black_counts = 0;
    for (i = 4; i < MT9V03X_W-4; i+=2)
    {
        g = get_gradient_x(p, i);
        if(my_abs(g) > GRADIENT_THRESHOLD)
        {
            Black_counts++;
            continue;
        }
    }

    if(Black_counts > 60 && ((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0)))
    {
        COM_QY = 0;
    }
}

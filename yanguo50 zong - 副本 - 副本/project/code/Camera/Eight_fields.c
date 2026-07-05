//  ���?������ͼ����

//------------------------------------------------------------------------------------------------------------------
#include "Eight_fields.h"

/*
�������ƣ�int my_abs(int value)
����˵����������?
����˵����
�������أ�����ֵ
�޸�ʱ�䣺2022��9��8��
��    ע��
example��  my_abs( x)��
 */
int my_abs(int value)
{
    if(value>=0) return value;
    else return -value;
}

int16 limit_a_b(int16 x, int16 a, int16 b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return x;
}

/*
�������ƣ�int16 limit(int16 x, int16 y)
����˵������x,y�е���Сֵ
����˵����
�������أ�������ֵ�е���Сֵ
�޸�ʱ�䣺2022��9��8��
��    ע��
example��  limit( x,  y)
 */
int16 limit1(int16 x, int16 y)
{
    if (x > y)             return y;
    else if (x < -y)       return -y;
    else                return x;
}


/*��������*/
uint8 far original_image[image_h][image_w];
uint8 image_thereshold;//ͼ��ָ����?
//------------------------------------------------------------------------------------------------------------------
//  @brief      ���һ���Ҷ�ͼ��?
//  @since      v1.0
//------------------------------------------------------------------------------------------------------------------
void Get_image(uint8(*mt9v03x_image)[image_w])
{
#define use_num     1   //1���ǲ�ѹ����2����ѹ��һ��
    uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num)          //
    {
        for (j = 0; j <image_w; j += use_num)     //
        {
            original_image[row][line] = mt9v03x_image[i][j];//����Ĳ�����д�������ͷ�ɼ�����ͼ��
            line++;
        }
        line = 0;
        row++;
    }
}
//------------------------------------------------------------------------------------------------------------------
//  @brief     ��̬��ֵ
//  @since      v1.0
//------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    uint16 X; uint16 Y;
    uint8* img_data = image;
    int HistGram[GrayScale] = {0};

    uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // ��䷽��?
    uint16 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;


	for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height��ΪY =Image_Height���Ա����?�ж�ֵ��
	{
		//Y=Image_Height;
		for (X = 0; X < Image_Width; X++)
		{
			HistGram[(int)img_data[Y*Image_Width + X]]++; //ͳ��ÿ���Ҷ�ֵ�ĸ�����Ϣ
		}
	}




    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
    {
        return MaxValue;          // ͼ����ֻ��һ����ɫ
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // ͼ����ֻ�ж�����ɫ
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  ��������
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //ǰ�����ص���
          PixelFore = Amount - PixelBack;         //�������ص���
          OmegaBack = (double)PixelBack / Amount;//ǰ�����ذٷֱ�
          OmegaFore = (double)PixelFore / Amount;//�������ذٷֱ�
          PixelIntegralBack += HistGram[Y] * Y;  //ǰ���Ҷ�ֵ
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
          MicroBack = (double)PixelIntegralBack / PixelBack;//ǰ���ҶȰٷֱ�
          MicroFore = (double)PixelIntegralFore / PixelFore;//�����ҶȰٷֱ�
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//����������䷽��g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}
//------------------------------------------------------------------------------------------------------------------
//  @brief      ͼ���ֵ���������õ��Ǵ�򷨶�ֵ����
//  @since      v1.0
//------------------------------------------------------------------------------------------------------------------
uint8 far bin_image[image_h][image_w];//ͼ������
void turn_to_bin(void)
{
  uint8 i,j;
 image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
  for(i = 0;i<image_h;i++)
  {
      for(j = 0;j<image_w;j++)
      {
          if(original_image[i][j]>image_thereshold)bin_image[i][j] = white_pixel;
          else bin_image[i][j] = black_pixel;
      }
  }
}


/*
�������ƣ�void get_start_point(uint8 start_row)
����˵����Ѱ�������߽�ı߽����Ϊ������ѭ������ʼ��
����˵����������������
�������أ���
�޸�ʱ�䣺2022��9��8��
��    ע��
example��  get_start_point(image_h-2)
 */
uint8 start_point_l[2] = { 0 };//�������x��yֵ
uint8 start_point_r[2] = { 0 };//�ұ�����x��yֵ
uint8 get_start_point(uint8 start_row)
{
    uint8 i = 0,l_found = 0,r_found = 0;
    //����
    start_point_l[0] = 0;//x
    start_point_l[1] = 0;//y

    start_point_r[0] = 0;//x
    start_point_r[1] = 0;//y

        //���м�����ߣ��������
    for (i = image_w / 2; i > border_min; i--)
    {
        start_point_l[0] = i;//x
        start_point_l[1] = start_row;//y
        if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0)
        {
            //printf("�ҵ�������image[%d][%d]\n", start_row,i);
            l_found = 1;
            break;
        }
    }

    for (i = image_w / 2; i < border_max; i++)
    {
        start_point_r[0] = i;//x
        start_point_r[1] = start_row;//y
        if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0)
        {
            //printf("�ҵ��ұ����image[%d][%d]\n",start_row, i);
            r_found = 1;
            break;
        }
    }

    if(l_found&&r_found)return 1;
    else {
        //printf("δ�ҵ����\n");
        return 0;
    }
}

/*
�������ƣ�void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
                            uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

����˵������������ʽ��ʼ���ұߵ�ĺ�������������е�࣬���õ�ʱ��Ҫ©�ˣ������������һ�������ꡣ
����˵����
break_flag_r            �������Ҫѭ���Ĵ���?
(*image)[image_w]       ����Ҫ�����ҵ��ͼ�����飬�����Ƕ�ֵ�?�����������Ƽ���
                       �ر�ע�⣬��Ҫ�ú궨��������Ϊ����������������ݿ����޷����ݹ���?
*l_stastic              ��ͳ��������ݣ����������ʼ�����Ա����ź�ȡ��ѭ������
*r_stastic              ��ͳ���ұ����ݣ����������ʼ�����Ա����ź�ȡ��ѭ������?
l_start_x               �������������?
l_start_y               ��������������
r_start_x               ���ұ���������
r_start_y               ���ұ����������?
hightest                ��ѭ���������õ�����߸߶�?
�������أ���
�޸�ʱ�䣺2022��9��25��
��    ע��
example��
    search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
                start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
 */
#define USE_num image_h*3   //�����ҵ�������Ա��������˵300�����ܷ��£�������Щ�������ȷʵ�Ѷ����ඨ����һ��?

 //��ŵ��x��y����
uint8 points_l[(uint16)USE_num][2] = { {  0 } };//����
uint8 points_r[(uint16)USE_num][2] = { {  0 } };//����
uint8 dir_r[(uint16)USE_num] = { 0 };//�����洢�ұ���������
uint8 dir_l[(uint16)USE_num] = { 0 };//�����洢�����������?
uint8 data_stastics_l = 0;//ͳ������ҵ���ĸ���
uint8 data_stastics_r = 0;//ͳ���ұ��ҵ���ĸ���?
uint8 hightest = 0;//��ߵ�?
void search_l_r(uint16 break_flag, uint8(*image)[image_w], uint8 *l_stastic, uint8 *r_stastic, uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest)
{

    uint8 i = 0, j = 0;

    //��߱���?
    uint8 search_filds_l[8][2] = { {  0 } };
    uint8 index_l = 0;
    uint8 temp_l[8][2] = { {  0 } };
    uint8 center_point_l[2] = {  0 };
    uint8 l_data_statics;//ͳ�����?
    //����˸�����?
    static int8 seeds_l[8][2] = { {0,  1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,  0},{1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�����˳ʱ��?

    //�ұ߱���
    uint8 search_filds_r[8][2] = { {  0 } };
    uint8 center_point_r[2] = { 0 };//���������?
    uint8 index_r = 0;//�����±�
    uint8 temp_r[8][2] = { {  0 } };
    uint8 r_data_statics;//ͳ���ұ�
    //����˸�����?
    static int8 seeds_r[8][2] = { {0,  1},{1,1},{1,0}, {1,-1},{0,-1},{-1,-1}, {-1,  0},{-1, 1}, };
    //{-1,-1},{0,-1},{+1,-1},
    //{-1, 0},       {+1, 0},
    //{-1,+1},{0,+1},{+1,+1},
    //�������ʱ��?

    l_data_statics = *l_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������?
    r_data_statics = *r_stastic;//ͳ���ҵ��˶��ٸ��㣬��������ѵ�ȫ��������?

    //��һ�θ��������? ���ҵ������ֵ������?
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

        //��������ѭ��
    while (break_flag--)
    {

        //���?
        for (i = 0; i < 8; i++)//����8F����
        {
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //�����������䵽�Ѿ��ҵ��ĵ���
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y
        l_data_statics++;//������һ

        //�ұ�
        for (i = 0; i < 8; i++)//����8F����
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //�����������䵽�Ѿ��ҵ��ĵ���
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        index_l = 0;//�����㣬��ʹ��
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//�����㣬��ʹ��
            temp_l[i][1] = 0;//�����㣬��ʹ��
        }

        //����ж�?
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
                && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
            {
                temp_l[index_l][0] = search_filds_l[(i)][0];
                temp_l[index_l][1] = search_filds_l[(i)][1];
                index_l++;
                dir_l[l_data_statics - 1] = (i);//��¼��������
            }

            if (index_l)
            {
                //���������?
                center_point_l[0] = temp_l[0][0];//x
                center_point_l[1] = temp_l[0][1];//y
                for (j = 0; j < index_l; j++)
                {
                    if (center_point_l[1] > temp_l[j][1])
                    {
                        center_point_l[0] = temp_l[j][0];//x
                        center_point_l[1] = temp_l[j][1];//y
                    }
                }
            }

        }
        if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
            && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
            ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
                && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
        {
            //printf("���ν���ͬһ���㣬�˳�\n");
            break;
        }
        if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
            && my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1]) < 2
            )
        {
            //printf("\n���������˳�\n");
            *hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//ȡ����ߵ�?
            //printf("\n��y=%d���˳�\n",*hightest);
            break;
        }
        if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
        {
//            printf("\n�����߱��ұ߸��ˣ���ߵȴ��ұ�\n");
            continue;//�����߱��ұ߸��ˣ���ߵȴ��ұ�?
        }
        if (dir_l[l_data_statics - 1] == 7
            && (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//��߱��ұ߸����Ѿ�����������?
        {
            //printf("\n��߿�ʼ�����ˣ��ȴ��ұߣ��ȴ���?.. \n");
            center_point_l[0] = points_l[l_data_statics - 1][0];//x
            center_point_l[1] = points_l[l_data_statics - 1][1];//y
            l_data_statics--;
        }
        r_data_statics++;//������һ

        index_r = 0;//�����㣬��ʹ��
        for (i = 0; i < 8; i++)
        {
            temp_r[i][0] = 0;//�����㣬��ʹ��
            temp_r[i][1] = 0;//�����㣬��ʹ��
        }

        //�ұ��ж�
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//������һ
                dir_r[r_data_statics - 1] = (i);//��¼��������
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {

                //���������?
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }

            }
        }


    }


    //ȡ��ѭ������
    *l_stastic = l_data_statics;
    *r_stastic = r_data_statics;

}
/*
�������ƣ�void get_left(uint16 total_L)
����˵�����Ӱ�����߽�����ȡ��Ҫ�ı���?
����˵����
total_L ���ҵ��ĵ������?
�������أ���
�޸�ʱ�䣺2022��9��25��
��    ע��
example�� get_left(data_stastics_l );
 */
uint8 l_border[image_h];//��������
uint8 r_border[image_h];//��������
uint8 center_line[image_h];//��������
void get_left(uint16 total_L)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    //��ʼ��
    for (i = 0;i<image_h;i++)
    {
        l_border[i] = border_min;
    }
    h = image_h - 2;
    //���?
    for (j = 0; j < total_L; j++)
    {
        //printf("%d\n", j);
        if (points_l[j][1] == h)
        {
            l_border[h] = points_l[j][0]+1;
        }
        else continue; //ÿ��ֻȡһ���㣬û����һ�оͲ���¼
        h--;
        if (h == 0)
        {
            break;//�����һ���˳�?
        }
    }
}
/*
�������ƣ�void get_right(uint16 total_R)
����˵�����Ӱ�����߽�����ȡ��Ҫ�ı���?
����˵����
total_R  ���ҵ��ĵ������?
�������أ���
�޸�ʱ�䣺2022��9��25��
��    ע��
example��get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
    uint8 i = 0;
    uint16 j = 0;
    uint8 h = 0;
    for (i = 0; i < image_h; i++)
    {
        r_border[i] = border_max;//�ұ��߳�ʼ���ŵ����ұߣ�����߷ŵ�����ߣ�����������պ�����������߾ͻ����м䣬������ŵõ�������?
    }
    h = image_h - 2;
    //�ұ�
    for (j = 0; j < total_R; j++)
    {
        if (points_r[j][1] == h)
        {
            r_border[h] = points_r[j][0] - 1;
        }
        else continue;//ÿ��ֻȡһ���㣬û����һ�оͲ���¼
        h--;
        if (h == 0)break;//�����һ���˳�?
    }
}

//�������ͺ͸�ʴ����ֵ����
#define threshold_max   255*5//�˲����ɸ����Լ����������?
#define threshold_min   255*2//�˲����ɸ����Լ����������?
void image_filter(uint8(*bin_image)[image_w])//��̬ѧ�˲�������˵�������ͺ͸�ʴ��˼��
{
    uint16 i, j;
    uint32 num = 0;


    for (i = 1; i < image_h - 1; i++)
    {
        for (j = 1; j < (image_w - 1); j++)
        {
            //ͳ�ư˸�����������?
            num =
                bin_image[i - 1][j - 1] + bin_image[i - 1][j] + bin_image[i - 1][j + 1]
                + bin_image[i][j - 1] + bin_image[i][j + 1]
                + bin_image[i + 1][j - 1] + bin_image[i + 1][j] + bin_image[i + 1][j + 1];


            if (num >= threshold_max && bin_image[i][j] == 0)
            {

                bin_image[i][j] = 255;//��  ���Ը�ɺ궨�壬�������

            }
            if (num <= threshold_min && bin_image[i][j] == 255)
            {

                bin_image[i][j] = 0;//��

            }

        }
    }

}

/*
�������ƣ�void image_draw_rectan(uint8(*image)[image_w])
����˵������ͼ��һ���ڿ�
����˵����uint8(*image)[image_w] ͼ���׵�ַ
�������أ���
�޸�ʱ�䣺2022��9��8��
��    ע��
example�� image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[image_w])
{

    uint8 i = 0;
    for (i = 0; i < image_h; i++)
    {
        image[i][0] = 0;
        image[i][1] = 0;
        image[i][image_w - 1] = 0;
        image[i][image_w - 2] = 0;

    }
    for (i = 0; i < image_w; i++)
    {
        image[0][i] = 0;
        image[1][i] = 0;
        //image[image_h-1][i] = 0;

    }
}

/*
�������ƣ�void image_process(void)
����˵�������մ�������
����˵������
�������أ���
�޸�ʱ�䣺2022��9��8��
��    ע��
example�� image_process();
 */
void image_process(void)
{
    uint16 i;
    uint8 hightest = 0;//����һ������У�tip����������ָ����yֵ����С

    /* �������ߵ����õ� */
//    Get_image(mt9v03x_image);
    Get_Use_Image();  //ͼ��ѹ��
    turn_to_bin();

    /* ��ȡ�����߽� */
    image_filter(bin_image);//�˲�
    image_draw_rectan(bin_image);//Ԥ����
    //����
    data_stastics_l = 0;
    data_stastics_r = 0;
    if (get_start_point(image_h - 2))//�ҵ�����ˣ���ִ�а�����û�ҵ���һֱ��?
    {
//        printf("���ڿ�ʼ������\n");
        search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &hightest);
//        printf("�������ѽ���\n");
        // ����ȡ�ı߽�������ȡ���� �� ��������������õı���?
        get_left(data_stastics_l);
        get_right(data_stastics_r);
        //���������������Ҫ�ŵ�if����ȥ�ˣ���Ҫ�ŵ�if����ȥ�ˣ���Ҫ�ŵ�if����ȥ�ˣ���Ҫ����˵����

    }


    //��ʾͼ��   �ĳ����Լ��ľ��� �Ⱥ����㹻�����ˣ���ʾ�ص�����ʾ��ͦռ��Դ��
//    ips154_displayimage032_zoom(bin_image[0], image_w, image_h, image_w, image_h,0,0);
    tft180_show_gray_image(0, 0, bin_image[0], Image_W, Image_H, Image_W, Image_H, 0);

    //��������ѭ�����������߽��?
    for (i = 0; i < data_stastics_l; i++)
    {
//        ips154_drawpoint(points_l[i][0]+2, points_l[i][1], uesr_BLUE);//��ʾ���?
        tft180_draw_point(points_l[i][0]+2, points_l[i][1], RGB565_BLUE);//��ʾ���?
    }
    for (i = 0; i < data_stastics_r; i++)
    {
//        ips154_drawpoint(points_r[i][0]-2, points_r[i][1], uesr_RED);//��ʾ���?
        tft180_draw_point(points_r[i][0]-2, points_r[i][1], RGB565_RED);
    }

    for (i = hightest; i < image_h-1; i++)
    {
        center_line[i] = (l_border[i] + r_border[i]) >> 1;//������
        //�������������󣬲����ǲ��߻�����״̬����ȫ�����ʹ��һ����ߣ����������������ܸ����������?
        //��ȻҲ�ж�����ߵ��ҷ������Ǹ��˸о��ܷ�����������?
//        ips154_drawpoint(center_line[i], i, uesr_GREEN);//��ʾ���?��ʾ����
//        ips154_drawpoint(l_border[i], i, uesr_GREEN);//��ʾ���?��ʾ�����?
//        ips154_drawpoint(r_border[i], i, uesr_GREEN);//��ʾ���?��ʾ�ұ���

        tft180_draw_point(center_line[i], i, RGB565_GREEN);
        tft180_draw_point(l_border[i], i, RGB565_GREEN);
        tft180_draw_point(r_border[i], i, RGB565_GREEN);
    }


}





/*

���������?.0��***************����>*************xֵ���?
************************************************************
************************************************************
************************************************************
************************************************************
******************��������һ��ͼ��*************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
yֵ���?******************************************(188.120)

*/




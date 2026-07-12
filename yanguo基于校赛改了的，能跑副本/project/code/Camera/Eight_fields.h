#ifndef __EIGHT_FIELDS_H_
#define __EIGHT_FIELDS_H_

#include "qy_headfile.h"

////��������������������ֲ������ֲ��ʱ�����ɾ�����ĳ����Լ��ģ�
//typedef   signed          char int8;
//typedef   signed short     int int16;
//typedef   signed           int int32;
//typedef unsigned          char uint8;
//typedef unsigned short     int uint16;
//typedef unsigned           int uint32;

//��ɫ����  ��Ϊ������������ɫ���Ĳ�������ֱ�ӷ�����
#define uesr_RED     0XF800    //��ɫ
#define uesr_GREEN   0X07E0    //��ɫ
#define uesr_BLUE    0X001F    //��ɫ

//�궨��
#define image_h         MT9V03X_H / 2       //ͼ��߶�
#define image_w         MT9V03X_W / 2       //ͼ�����

#define white_pixel 255
#define black_pixel 0

#define bin_jump_num    1//�����ĵ���
#define border_max  image_w-2 //�߽����ֵ
#define border_min  1   //�߽���Сֵ
extern uint8 original_image[image_h][image_w];
extern uint8 bin_image[image_h][image_w];//ͼ������

extern uint8 l_border[image_h];//��������
extern uint8 r_border[image_h];//��������
extern uint8 center_line[image_h];//��������

extern void image_process(void); //ֱ�����жϻ�ѭ������ô˳���Ϳ���ѭ��ִ����
int16 my_abs(int16 value);
int16 limit_a_b(int16 x, int16 a, int16 b);
int16 limit1(int16 x, int16 y);

#endif /* CODE_CAMERA_SEARCH_LINE_EIGHTFIELDS_EIGHT_FIELDS_H_ */

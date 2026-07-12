#ifndef __SEARCH_LINE_H_
#define __SEARCH_LINE_H_

#include "qy_headfile.h"

#define SEAR_COL 1
#define SEARCH_IMAGE_W      Image_W             //���ߵĿ���
#define SEARCH_IMAGE_H      Image_H        //���ߵĸ߶�
#define BLACKPOINT          50             //���Ǻڵ�ֵ
#define WHITEPOINT          190             //���ǰ׵�ֵ
#define WHITEMAXMUL         125             //�ο�������׵�ٷֱ�
#define WHITEMINMUL         75              //�ο������С�׵�ٷֱ�

#define REFRENCE_ROW        10              //�ο���ͳ������
#define SEARCH_RANGE        10             //���߰뾶
#define STOPROW             1              //����ֹͣ��
#define CONTRASTOFFSET      1              //���߶Ա�ƫ��
extern int16 REFERENCE_CONTRAST;            //�ο��Աȶ�

#define MY_ABS(x)  ((x) < 0 ? -(x) : (x))   //�ڹ����ֵ

#define GRADIENT_THRESHOLD_DEF 200          //Ĭ���ݶ���ֵ
extern int16 GRADIENT_THRESHOLD;            //Sobel�ݶȼ���ֵ

/* ��ͼ��Ĳο��� */
extern uint8 reference_point[3];
/* ��̬�׵�����ֵ����Сֵ */
extern uint8 white_max_point;
extern uint8 white_min_point;

/* �ο��е�������ÿһ�е����� */
//extern uint8 reference_col;
extern uint8 reference_col_line[MT9V03X_H];

void get_reference_point(void);
void search_reference_col(void);
uint8 find_extreme_Value(uint8* arr, uint8 start, uint8 end, int mode);
int16 get_gradient_x(uint8 row, uint8 col);

#endif /* CODE_CAMERA_SEARCH_LINE_ZF_SEARCH_LINE_H_ */

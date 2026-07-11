#ifndef __SEARCH_LINE_H_
#define __SEARCH_LINE_H_

#include "qy_headfile.h"

#define SEAR_COL 1
#define SEARCH_IMAGE_W      Image_W             //搜线的宽度
#define SEARCH_IMAGE_H      Image_H        //搜线的高度
#define BLACKPOINT          50             //必是黑点值
#define WHITEPOINT          220             //必是黑点值
#define WHITEMAXMUL         125             //参考点的最大白点百分比
#define WHITEMINMUL         75              //参考点的最小白点百分比

#define REFRENCE_ROW        10              //参考点统计行数
#define SEARCH_RANGE        10             //搜线半径
#define STOPROW             1              //搜线停止行
#define CONTRASTOFFSET      1              //搜线对比偏移
extern int16 REFERENCE_CONTRAST;            //参考对比度

/* 本图像的参考点 */
extern uint8 reference_point[3];
/* 动态白点的最大值、最小值 */
extern uint8 white_max_point;
extern uint8 white_min_point;

/* 参考列的列数和每一行的数组 */
//extern uint8 reference_col;
extern uint8 reference_col_line[MT9V03X_H];

void get_reference_point(void);
void search_reference_col(void);
uint8 find_extreme_Value(uint8* arr, uint8 start, uint8 end, int mode);

#endif /* CODE_CAMERA_SEARCH_LINE_ZF_SEARCH_LINE_H_ */

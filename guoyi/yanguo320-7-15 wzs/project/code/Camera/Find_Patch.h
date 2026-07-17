#ifndef __FIND_PATCH_H_
#define __FIND_PATCH_H_

#include "qy_headfile.h"

/* 左右上角点位置 */
extern int16 Left_up_Patch;
extern int16 Right_up_Patch;

/* 左右下角点位置 */
extern int16 Left_dowm_Patch;
extern int16 Left_dowm_Patch_x;
extern int16 Right_dowm_Patch;
extern int16 Right_dowm_Patch_x;

/* 某行黑点的数量，用于丢线保护 */
extern uint16 Black_counts;

/* 丢线保护标志位 */
extern int16 lost_line_protect;

/*丢线数量*/
extern int16 Lost_Left_lost_nums;
extern int16 Lost_Right_lost_nums;

extern uint16 bmxian;

void Lost_connection_left(int16 start, int16 end);
int16 Find_left_up_point(int16 start, int16 end);
int16 Find_Right_up_point(int16 start, int16 end);
int16 Find_left_dowm_point(int16 start, int16 end);
int16 Find_Right_dowm_point(int16 start, int16 end);
int16 White_counts_weight(int16 p);
int16 White_counts_h(int16 p);
void Black_counts_weight(int16 p);

#endif 

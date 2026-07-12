#ifndef __LASER_H_/* CODE_LASER_H_ */
#define __LASER_H_/* CODE_LASER_H_ */
#include "zf_common_headfile.h"
#define SHOOT_TIME  1500
extern uint16 tar_i;
extern uint8 tar_flag;
extern uint8  tar_x;    //坐标x
extern uint8  tar_y;	 //坐标y
extern uint8 tar_th;    //环阈值

extern uint8 l_d;//靶环左边界
extern uint8 r_d;//靶环右边界
extern uint8 top_d;//靶环上边界
extern uint8 under_d;//靶环下边界
extern uint8 debug_stage;//调试  阶段变量
extern uint8 pre_find_offset;//预扫描偏移量


void laser_init(void);
void laser_on(void);
void laser_off(void);//这段属于是过度封装了


void Pre_Scan(void);//预扫描  确定偏移量
void Target_find(int8 midOffset);//寻找靶环


#endif /* CODE_LASER_H_ */


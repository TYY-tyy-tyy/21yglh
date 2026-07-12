#ifndef __RING_H_
#define __RING_H_
#include "zf_common_headfile.h"

extern uint8 ring_preMeet_flag;
extern uint8 first_meeting_flag;	//初见圆环标志位
extern uint8 ring_l,ring_r;	//圆环左右标志位
extern uint8 ring_enter_flag;//进入圆环标志位
extern uint8 ring_turn_flag;//转弯标志位
extern uint8 Out_flag;//出环状态的标志
extern uint8 Straighten_flag;//出环摆正标志位
extern uint8 ring_over_flag;//结束了

extern uint8 key_anlysis1,key_anlysis2,key_anlysis3;//测试

void Ring(void);
void draw_line(uint8 left_point, uint8 right_point, uint8 to_flag);//0是左  1是右
void connect_point(uint8 under,uint8 top,uint8 l_r);//0是左 1是右

uint8 Ring_Pre_Meet(void);
void Ring_First_meeting(void);
void Ring_Enter(void);
void Ring_Turing(void);
void Ring_Ring_Ring(void);
void Ring_Out(void);
void Ring_Straighten(void);
void Ring_Over(void);

uint8 isContinueLine(uint8* arr);
void repair_line_display(uint8 a,uint8 b);
#endif /* CODE_RING_H_ */


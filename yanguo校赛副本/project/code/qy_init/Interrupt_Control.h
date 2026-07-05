#ifndef __INTERRUPT_CONTROL_H_
#define __INTERRUPT_CONTROL_H_

#include "qy_headfile.h"

/* 速度模式 */
extern int Speed_Mode;     //速度模式--1为线性，2为匀速

/* 中点误差 */
extern int16 Image_error;

/* 停车标志位 */
extern int Stop_Car_flag;

/* 目标速度 */
extern int16 TargetSpeed;           //目标速度  190
extern int16 nowtargetSpeed;
extern int16 targetSpeed_min;

/* 速度环输出 */
extern int16 Speed_Left_Out;
extern int16 Speed_Right_Out;

/* 转向环输出 */
extern int16 Turn_Out;

/* 时间 */
extern uint32 time;

void Interrupt_CCU60_CH0(void);
void Speed_DecisionMaking(void);
void show_menu(void);
#endif /* CODE_INTERRUPT_CONTROL_INTERRUPT_CONTROL_H_ */

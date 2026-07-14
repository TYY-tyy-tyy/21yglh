#ifndef __INTERRUPT_CONTROL_H_
#define __INTERRUPT_CONTROL_H_

#include "qy_headfile.h"

#define Left_Out_Max  7000
#define Right_Out_Max 7000

#define Start_time    200
#define ACKERMAN_COEFF  0.385f //220 260 0.370

/* 转向环 */
#define MID_ERROR_MAX       10     //当前权重下的最大终点误差(用于速度决策)

/* 中点误差 */
extern int16 Image_error;

/* 停车标志位 */
extern uint8 Stop_Car_flag;

/* 目标速度 */
extern int16 TargetSpeed;           //目标速度  190
extern int16 nowtargetSpeed;
extern int16 targetSpeed_min;

/* 速度环输出 */
extern int16 Speed_Left_Out;
extern int16 Speed_Right_Out;

/* 转向环输出 */
extern int16 Turn_Out;

extern int16 variance_max;

/* 时间 */
extern uint16 time;

void Interrupt_CCU60_CH0(void);
void Speed_DecisionMaking(void);
void show_menu(void);
int16 LinearMap(int16 max_val, int16 min_val, int16 err, int16 err_limit);
#endif /* CODE_INTERRUPT_CONTROL_INTERRUPT_CONTROL_H_ */

#ifndef __MOTORS_H
#define __MOTORS_H

#include "qy_headfile.h"

/* 左电机 */
#define Left_Motor_PWM    PWMB_CH1_P74    // 对应 P7_4
#define Left_Motor_DIR    IO_P75        // 对应 P7_0

/* 右电机 */
#define Right_Motor_PWM   PWMB_CH3_P76    // 对应 P7_5
#define Right_Motor_DIR   IO_P77        // 对应 P7_1        

/* 左右电机最后输出的值---经过PID之后 */
extern int16 Left_Out;
extern int16 Right_Out;

void Motors_Init(void);
void Left_Motor_Speed(int16 Speed);
void Right_Motor_Speed(int16 Speed);

#endif

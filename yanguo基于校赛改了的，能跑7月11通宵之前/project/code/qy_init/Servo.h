#ifndef __SERVO_H_
#define __SERVO_H_

#include "qy_headfile.h"

#define SERVO_MOTOR_FREQ            (PWME_CH1P_PA0)                         // 定义主板上舵机对应引脚
#define Servo_PWM              		(300 )                                           // 定义主板上舵机频率  请务必注意范围 50-300

#define SERVO_CONTROL_L_MAX             (78 )                                           // 定义主板上舵机活动范围 角度
#define SERVO_CONTROL_R_MAX             (98)                                           // 定义主板上舵机活动范围 角度

/* 舵机中值 */
#define SERVO_MOTOR_Mid          (4550 )
/* 左边最大打角 */
#define SERVO_MOTOR_L_MAX           (4995 )//350
/* 右边最大打角 */
#define SERVO_MOTOR_R_MAX           (4105)//350

#define SERVO_CONTROL_DUTY(x)         ((float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_CONTROL_FREQ) * (0.5 + (float)(x) / 90.0))

void Servo_Init(void);
void Servo_Angle(float angle);

#endif /* CODE_TEST_TEST_H_ */

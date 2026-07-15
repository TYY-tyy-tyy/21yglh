#ifndef __BLDS_H_
#define __BLDS_H_

#include "qy_headfile.h"

//#define Blds_DIR_CH1             (IO_PA1)                                            // 电机方向输出端口
#define Blds_PWM_CH1             (PWMF_CH3_PA5)                                  // PWM输出端口

//#define Blds_DIR_CH2             (IO_PA3)                                            // 电机方向输出端口
#define Blds_PWM_CH2             (PWMF_CH4_PA7)                                  // PWM输出端口

/* 无刷PWM  1000~2000 */
extern int16 Blds_PWM_MAX;    //芯片最大PWM
extern int16 Blds_PWM_MIN;    //芯片最大PWM

/* 检测无刷开启标志位 */
extern int8 Blds_flag;

/* 无刷转速变量 */
extern int16 Blds_Speed;

void Blds_Init(void);
void Left_Blds_Speed(int16 Speed);
void Right_Blds_Speed(int16 Speed);
void Open_Blds(void);

#endif /* CODE_BLDS_BLDS_H_ */

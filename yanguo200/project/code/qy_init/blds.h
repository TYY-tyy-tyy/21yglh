#ifndef __BLDS_H_
#define __BLDS_H_

#include "qy_headfile.h"

//#define Blds_DIR_CH1             (IO_PA1)                                            // 电机方向输出端口
#define Blds_PWM_CH1             (PWME_CH1N_PA1)                                  // PWM输出端口

//#define Blds_DIR_CH2             (IO_PA3)                                            // 电机方向输出端口
#define Blds_PWM_CH2             (PWME_CH2N_PA3)                                  // PWM输出端口

#define Blds_ENCODER1_TIM        (TIM17_ENCODER)                                     // 编码器定时器
#define Blds_ENCODER1_PLUS       (TIM17_ENCODER_CH1_P80)                           // 编码器计数端口
#define Blds_ENCODER1_DIR        (IO_P44)                           // 编码器方向采值端口

#define Blds_ENCODER2_TIM        (TIM18_ENCODER)                                     // 编码器定时器
#define Blds_ENCODER2_PLUS       (TIM18_ENCODER_CH1_P90)                           // 编码器计数端口
#define Blds_ENCODER2_DIR        (IO_P46)                           // 编码器方向采值端口

/* 无刷PWM  1000~2000 */
extern int16 Blds_PWM_MAX;    //芯片最大PWM
extern int16 Blds_PWM_MIN;    //芯片最大PWM

/* 检测无刷开启标志位 */
extern int Blds_flag;

/* 无刷转速变量 */
extern int16 Blds_Speed;

void Blds_Init(void);
void Left_Blds_Speed(int16 Speed);
void Right_Blds_Speed(int16 Speed);
void Open_Blds(int flag);

#endif /* CODE_BLDS_BLDS_H_ */

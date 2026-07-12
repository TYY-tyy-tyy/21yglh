#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "zf_common_headfile.h"
extern int16 encoder_data_r;
extern int16 encoder_data_l;
extern int16 tar_speed;
#define  DIR_R              ( IO_P51 )
#define  PWM_R              ( PWMD_CH1_P50 )
                              
#define  DIR_L              ( IO_P53 )
#define  PWM_L              ( PWMD_CH3_P52 )

#define ENCODER_DIR_1                 	(PWMA_ENCODER)              // 带方向编码器对应使用的编码器接口 
#define ENCODER_DIR_PULSE_1            	(PWMA_ENCODER_CH1P_P60)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_1              	(PWMA_ENCODER_CH2P_P62)     // DIR 对应的引脚

#define ENCODER_DIR_2                 	(PWMC_ENCODER)              // 带方向编码器对应使用的编码器接口
#define ENCODER_DIR_PULSE_2       		(PWMC_ENCODER_CH1P_P40)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_2           	(PWMC_ENCODER_CH2P_P42)     // DIR 对应的引脚

//250
#define MAX_SPEED 100
#define MIN_SPEED 80


//后轮驱动代码
void Motor_Init(void);
void motor_test(void);
void Encoder_GetValue(void);
void Motor_Loop(void);
void Motor_control(pwm_channel_enum wheel,int16 speed);

extern float diff_kp;
//差速
void Motor_different(void);


#endif



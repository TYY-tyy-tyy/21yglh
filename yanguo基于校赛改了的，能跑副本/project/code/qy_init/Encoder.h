#ifndef __ENCODER_H_
#define __ENCODER_H_

#include "qy_headfile.h"

/* 左编码器（TIM4，P4.0/P4.2） */
#define LEFT_ENCODER_QUADDEC      PWMC_ENCODER
#define LEFT_ENCODER_QUADDEC_A    PWMC_ENCODER_CH1P_P40  // 对应P4.0（TIM4_CH1）
#define LEFT_ENCODER_QUADDEC_B    PWMC_ENCODER_CH2P_P42  // 对应P4.2（TIM4_CH2）

/* 右编码器（TIM6，P6.0/P6.2） */
#define RIGHT_ENCODER_QUADDEC     PWMA_ENCODER
#define RIGHT_ENCODER_QUADDEC_A   PWMA_ENCODER_CH1P_P60  // 对应P6.0（TIM6_CH1）
#define RIGHT_ENCODER_QUADDEC_B   PWMA_ENCODER_CH2P_P62  // 对应P6.2（TIM6_CH2）

typedef struct
{
    float LastP;
    float Now_P;
    float out;
    float Kg;
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
}KFP;//Kalman Filter parameter

/* 左右编码器的值 */
extern int16 Encoder_Left;
extern int16 Encoder_Right;

/* 左右编码器的平均值 */
extern int16 Encoder_Average;

/* 积分标志位 */
extern int16 Encoder_jifen_flag;

/* 当前积分值 */
extern int16 Enc_value;

/* 统计n个一米的值 */
extern int16 Enc_n;

/* 赛道长度 */
extern int16 Track_length[2];

void Encoder_Init(void);
int16 Left_Encoder_Get(void);
int16 Right_Encoder_Get(void);
void use_encoder_flag(void);
float kalmanFilter(KFP *kfp,float input);
void Count_Length(void);

#endif /* CODE_ENCODER_H_ */

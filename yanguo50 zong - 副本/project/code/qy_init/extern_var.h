#ifndef __EXTERN_VAR_H_
#define __EXTERN_VAR_H_

#include "qy_headfile.h"

/* 模式组合结构体 */
typedef struct
{
    int16 Mode_Speed_Max[5];
    int16 Mode_Speed_Min[5];
    float Mode_S_p[5];
    float Mode_S_i[5];
    float Mode_T_p[5];
    float Mode_T_GKD[5];
} MODE;

extern MODE mode;

/* 速度模式切换 */
extern int out_Mode;

/* 电压检测  */
extern uint16 ADC_Out;  //ADC输出
extern float battery_voltage; //电池电压

/* 中断标志位 */
extern uint8 pit_state_CCU6_0_CH0;

/* 丢线保护标志位 */
extern int Protection;

/* 图像开启标志 */
extern int image_flag;

/* 转向环PID输出值 */
extern int16 Turn_Out;
extern int16 Turn_Out_MAX;
extern int16 Speed_Out;
extern int16 Speed_Left_Out;                    //速度环输出
extern int16 Speed_Right_Out;
extern int16 Speed_Out;
extern int16 nowtargetSpeed;
/* 计时变量 */
extern uint16 use_time;

extern double img_data;

extern uint8 COM_QY;

extern int my_Speed;

extern uint8 far image_copy[MT9V03X_H][MT9V03X_W];
extern uint8 far image_copy_out[MT9V03X_H][MT9V03X_W];

void Interrupt(void);
void QQYY(void);

#endif /* CODE_EXTERN_VAR_EXTERN_VAR_H_ */

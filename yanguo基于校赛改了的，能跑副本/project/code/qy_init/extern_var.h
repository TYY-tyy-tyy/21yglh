#ifndef __EXTERN_VAR_H_
#define __EXTERN_VAR_H_

#include "qy_headfile.h"

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

extern int16 my_Speed;

extern uint8 far image_copy[MT9V03X_H][MT9V03X_W];
extern uint8 far image_copy_out[MT9V03X_H][MT9V03X_W];

void Interrupt(void);
void QQYY(void);

#endif /* CODE_EXTERN_VAR_EXTERN_VAR_H_ */

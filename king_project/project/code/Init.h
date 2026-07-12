#ifndef __Init_H_
#define __Init_H_
#include "zf_common_headfile.h"
extern uint8 motor_pit,k4;
extern uint32 _t;
extern volatile uint8 wifi_spi_get_data_buffer[256];
extern uint8 data_len;
extern int16 just_i,just_p,just_tar,just_angle;
void System_Init(void);

//卡尔曼滤波

//调参数用
void test_ss(void);
//按下按键 | wifi发送 我才开始
void Begin(void);
//蜂鸣器爆炸
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzero_O(void);
void Buzzer_test(uint8 count);
//测试代码
void o_O(const char dat[]);
void x_X(void);//用舵机
#endif

#ifndef __PID_H_
#define __PID_H_

#include "qy_headfile.h"

/* PID结构体 */
typedef struct
{
    /* 转向环PD */
    int16 Turn_KP;
	int16 Turn_KP1;
    int16 Turn_KI;
    int16 Turn_KD;
    int16 Turn_GKD;
    int16 Turn_current_error; //当前误差
    int16 Turn_last_error;  //上次误差
    int16 Turn_Out;  //转向环输出

    /* 速度环PI */
    int16 Speed_KP_L;
    int16 Speed_KI_L;
    int16 Speed_KD_L;
    int16 Speed_current_error_L;      //当前误差
    int16 Speed_Last_error_L;         //上一次的误差，用于增量式PID
    int16 Speed_Prev_error_L;         //前两次的误差，用于增量式PID
    int16 Speed_All_Error_L;          // 位置式速度环误差的积分
    int16 Speed_Out_L;                //速度环输出
    int16 Speed_Last_Out_L;           //上次速度环输出，用于滤波

    int16 Speed_KP_R;
    int16 Speed_KI_R;
    int16 Speed_KD_R;
    int16 Speed_current_error_R;
    int16 Speed_Last_error_R;
    int16 Speed_Prev_error_R;
    int16 Speed_All_Error_R;
    int16 Speed_Out_R;
    int16 Speed_Last_Out_R;

    int16 Speed_KP_Stop;
    int16 Speed_KI_Stop;
    int16 Speed_KD_Stop;
    int16 Speed_current_error_Stop;
    int16 Speed_Last_error_Stop;
    int16 Speed_Prev_error_Stop;
    int16 Speed_All_Error_Stop;
    int16 Speed_Out_Stop;
    int16 Speed_Last_Out_Stop;

    /* 角速度环PI */
    int16 Angle_KP;
    int16 Angle_KI;
    int16 Angle_KD;
    int16 Angle_current_error;
    int16 Angle_All_Error;
    int16 Angle_Out;
    int16 Angle_Last_Out;
} PID;

extern PID pid;

/* 电机差速的KP */
extern int16 KP_x_Increase;
extern int16 KP_x_Decrease;

void PID_Init(void);
int16 PID_Speed_Loc_L(int16 Speed_target, int16 Speed_current);
int16 PID_Speed_Loc_R(int16 Speed_target, int16 Speed_current);
int16 PID_Speed_Loc_Stop(int16 Speed_target, int16 Speed_current);
int16 PID_Speed_Inc_L(int16 Speed_target, int16 Speed_current);
int16 PID_Speed_Inc_R(int16 Speed_target, int16 Speed_current);
int16 PID_Turn_Loc(int16 Mid_Error);
int16 Angle_PID(int16 Angle_target, int16 Angle_current);
void PID_Setting(void);
void PID_DecisionMaking(void);

#endif /* CODE_PID_PID_H_ */

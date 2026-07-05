#ifndef __PID_H_
#define __PID_H_

#include "qy_headfile.h"

/* PID结构体 */
typedef struct
{
    /* 转向环PD */
    float Turn_KP;
	float Turn_KP1;
    float Turn_KI;
    float Turn_KD;
    float Turn_GKD;
    float Turn_current_error; //当前误差
    float Turn_last_error;  //上次误差
    float Turn_Out;  //转向环输出

    /* 速度环PI */
    float Speed_KP_L;
    float Speed_KI_L;
    float Speed_KD_L;
    int16 Speed_current_error_L;      //当前误差
    int16 Speed_Last_error_L;         //上一次的误差，用于增量式PID
    int16 Speed_Prev_error_L;         //前两次的误差，用于增量式PID
    int16 Speed_All_Error_L;          // 位置式速度环误差的积分
    int16 Speed_Out_L;                //速度环输出
    int16 Speed_Last_Out_L;           //上次速度环输出，用于滤波

    float Speed_KP_R;
    float Speed_KI_R;
    float Speed_KD_R;
    int16 Speed_current_error_R;
    int16 Speed_Last_error_R;
    int16 Speed_Prev_error_R;
    int16 Speed_All_Error_R;
    int16 Speed_Out_R;
    int16 Speed_Last_Out_R;

    float Speed_KP_Stop;
    float Speed_KI_Stop;
    float Speed_KD_Stop;
    int16 Speed_current_error_Stop;
    int16 Speed_Last_error_Stop;
    int16 Speed_Prev_error_Stop;
    int16 Speed_All_Error_Stop;
    int16 Speed_Out_Stop;
    int16 Speed_Last_Out_Stop;

    /* 角速度环PI */
    float Angle_KP;
    float Angle_KI;
    float Angle_KD;
    int16 Angle_current_error;
    int16 Angle_All_Error;
    int16 Angle_Out;
    int16 Angle_Last_Out;
} PID;

extern PID pid;

/* 控制距离的KP */
extern float KP_y;

/* 控制弯道的距离KP_y */
extern float KP_y_Ring;

/* 电机差速的KP */
extern float KP_x_Increase;
extern float KP_x_Decrease;

/* 基础PID值，用于调参时的初值 */
extern float T_p;
extern float kp_y;
extern float kp_x_up;
extern float kp_x_down;
extern float Stop_p;

/* 基础PID值，用于调参时的初值 */
extern float S_p;
extern float S_i;
extern float T_GKD;

void PID_Init(void);
int16 PID_Speed_Loc_L(int16 Speed_target, int Speed_current);
int16 PID_Speed_Loc_R(int16 Speed_target, int Speed_current);
int16 PID_Speed_Loc_Stop(int16 Speed_target, int Speed_current);
int16 PID_Speed_Inc_L(int16 Speed_target, int16 Speed_current);
int16 PID_Speed_Inc_R(int16 Speed_target, int16 Speed_current);
float PID_Turn_Loc(float Mid_Error);
int16 Angle_PID(int16 Angle_target, int Angle_current);
void PID_Setting(void);
void PID_DecisionMaking(void);

#endif /* CODE_PID_PID_H_ */

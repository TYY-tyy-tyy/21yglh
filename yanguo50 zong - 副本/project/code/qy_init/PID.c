#include "PID.h"

/* 位置式速度环积分限幅 */
#define    Speed_L_imax     8000
#define    Speed_R_imax     8000

PID pid;

/* 电机差速的KP */
float KP_x_Increase;
float KP_x_Decrease;

/* 基础PID值，用于调参时的初值 */
float Stop_p = 20;
float T_p = 11;//11
float kp_y = 0.9;
float kp_x_up = 2;
float kp_x_down = 16;

/* 基础PID值，用于调参时的初值 */
float S_p = 50;
float S_i = 0.3;
float T_GKD = 0.04;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介  PID参数初始化
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void PID_Init(void)
{
     /* 转向环PD */
     pid.Turn_KP = 0;
	 pid.Turn_KP1 = 0;
     pid.Turn_KI = 0;
     pid.Turn_KD = 0;
     pid.Turn_GKD = 0;
     pid.Turn_current_error = 0;
     pid.Turn_last_error = 0;
     pid.Turn_Out = 0;

     /* 速度环PI */
     pid.Speed_KP_L = 0;
     pid.Speed_KI_L = 0;
     pid.Speed_KD_L = 0;
     pid.Speed_current_error_L = 0;
     pid.Speed_Last_error_L = 0;
     pid.Speed_Prev_error_L = 0;
     pid.Speed_All_Error_L = 0; //位置式PI的误差积分
     pid.Speed_Out_L = 0;
     pid.Speed_Last_Out_L = 0; //用于滤波

     pid.Speed_KP_R = 0;
     pid.Speed_KI_R = 0;
     pid.Speed_KD_R = 0;
     pid.Speed_current_error_R = 0;
     pid.Speed_Last_error_R = 0;
     pid.Speed_Prev_error_R = 0;
     pid.Speed_All_Error_R = 0; //位置式PI的误差积分
     pid.Speed_Out_R = 0;
     pid.Speed_Last_Out_R = 0; //用于滤波

     pid.Speed_KP_Stop = 0;
     pid.Speed_KI_Stop = 0;
     pid.Speed_KD_Stop = 0;
     pid.Speed_current_error_Stop = 0;
     pid.Speed_Last_error_Stop = 0;
     pid.Speed_Prev_error_Stop = 0;
     pid.Speed_All_Error_Stop = 0; //位置式PI的误差积分
     pid.Speed_Out_Stop = 0;
     pid.Speed_Last_Out_Stop = 0; //用于滤波

     /* 角速度环PI */
     pid.Angle_KP = 0;
     pid.Angle_KI = 0;
     pid.Angle_KD = 0;
     pid.Angle_current_error = 0;
     pid.Angle_All_Error = 0;
     pid.Angle_Out = 0;
     pid.Angle_Last_Out = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介   速度环PI--位置式
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 PID_Speed_Loc_L(int16 Speed_target, int Speed_current)
{
    /* 一阶滤波系数(工程经验) */
//    float a = 0.3;

    /* 计算速度误差 */
    pid.Speed_current_error_L = Speed_target - Speed_current;

    /* 误差累加积分 */
    pid.Speed_All_Error_L += pid.Speed_current_error_L;

    /* 误差运算 */
    pid.Speed_Out_L = (pid.Speed_KP_L * pid.Speed_current_error_L) + (pid.Speed_KI_L * pid.Speed_All_Error_L);

    /* 速度环一阶低通滤波 */
//    pid.Speed_Out_L = (pid.Speed_Out_L * a) + ((1 - a) * pid.Speed_Last_Out_L);

     /* 更新“上次误差”用于滤波 */
//    pid.Speed_Last_Out_L = pid.Speed_Out_L;

    /* 积分限幅 */
    if(pid.Speed_All_Error_L > Speed_L_imax)   pid.Speed_All_Error_L = Speed_L_imax;
    else if(pid.Speed_All_Error_L < -Speed_L_imax) pid.Speed_All_Error_L = -Speed_L_imax;

    /* 返回结果 */
    return pid.Speed_Out_L;

}

int16 PID_Speed_Loc_R(int16 Speed_target, int Speed_current)
{
    /* 一阶滤波系数(工程经验) */
//    float a = 0.3;

    /* 计算速度误差 */
    pid.Speed_current_error_R = Speed_target - Speed_current;

    /* 误差累加积分 */
    pid.Speed_All_Error_R += pid.Speed_current_error_R;

    /* 误差运算 */
    pid.Speed_Out_R = (pid.Speed_KP_R * pid.Speed_current_error_R) + (pid.Speed_KI_R * pid.Speed_All_Error_R);

    /* 速度环一阶低通滤波 */
//    pid.Speed_Out_R = (pid.Speed_Out_R * a) + ((1 - a) * pid.Speed_Last_Out_R);

     /* 更新“上次误差”用于滤波 */
//    pid.Speed_Last_Out_R = pid.Speed_Out_R;

    /* 积分限幅 */
    if(pid.Speed_All_Error_R > Speed_R_imax)   pid.Speed_All_Error_R = Speed_R_imax;
    else if(pid.Speed_All_Error_R < -Speed_R_imax) pid.Speed_All_Error_R = -Speed_R_imax;

    /* 返回结果 */
    return pid.Speed_Out_R;

}

int16 PID_Speed_Loc_Stop(int16 Speed_target, int Speed_current)
{
    /* 一阶滤波系数(工程经验) */
//    float a = 0.3;

    /* 计算速度误差 */
    pid.Speed_current_error_Stop = Speed_target - Speed_current;

    /* 误差累加积分 */
    pid.Speed_All_Error_Stop += pid.Speed_current_error_Stop;

    /* 误差运算 */
    pid.Speed_Out_Stop = (pid.Speed_KP_Stop * pid.Speed_current_error_Stop) + (pid.Speed_KI_Stop * pid.Speed_All_Error_Stop);

    /* 速度环一阶低通滤波 */
//    pid.Speed_Out_Stop = (pid.Speed_Out_Stop * a) + ((1 - a) * pid.Speed_Last_Out_Stop);

     /* 更新“上次误差”用于滤波 */
//    pid.Speed_Last_Out_Stop = pid.Speed_Out_Stop;

    /* 积分限幅 */
    if(pid.Speed_All_Error_Stop > Speed_L_imax)   pid.Speed_All_Error_Stop = Speed_L_imax;
    else if(pid.Speed_All_Error_Stop < -Speed_L_imax) pid.Speed_All_Error_Stop = -Speed_L_imax;

    /* 返回结果 */
    return pid.Speed_Out_Stop;

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介   速度环PI--增量式
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 PID_Speed_Inc_L(int16 Speed_target, int16 Speed_current)
{
    /* 一阶滤波系数(工程经验) */
    float a = 0.3;

    /* 计算速度误差 */
    pid.Speed_current_error_L = Speed_target - Speed_current;//110 0

    /* 误差运算 */
    pid.Speed_Out_L = (pid.Speed_KP_L * (pid.Speed_current_error_L - pid.Speed_Last_error_L)) + (pid.Speed_KI_L * pid.Speed_current_error_L);

    /* 速度环一阶低通滤波 */
    pid.Speed_Out_L = (pid.Speed_Out_L * a) + ((1 - a) * pid.Speed_Last_Out_L);

    /* 更新“上次误差”用于滤波 */
    pid.Speed_Last_Out_L = pid.Speed_Out_L;

    /* 更新上次误差 */
    pid.Speed_Last_error_L = pid.Speed_current_error_L;

    /* 限幅 */
//    if(pid.Speed_Out > 2000)   pid.Speed_Out = 2000;
//    else if(pid.Speed_Out <= 0) pid.Speed_Out = 0;
//    if(pid.Speed_Out <= 0) pid.Speed_Out = 0;

    /* 返回结果 */
    return pid.Speed_Out_L;

}

int16 PID_Speed_Inc_R(int16 Speed_target, int16 Speed_current)
{
    /* 一阶滤波系数(工程经验) */
    float a = 0.3;

    /* 计算速度误差 */
    pid.Speed_current_error_R = Speed_target - Speed_current;//110 0

    /* 误差运算 */
    pid.Speed_Out_R = (pid.Speed_KP_R * (pid.Speed_current_error_R - pid.Speed_Last_error_R)) + (pid.Speed_KI_R * pid.Speed_current_error_R);

    /* 速度环一阶低通滤波 */
    pid.Speed_Out_R = (pid.Speed_Out_R * a) + ((1 - a) * pid.Speed_Last_Out_R);

    /* 更新“上次误差”用于滤波 */
    pid.Speed_Last_Out_R = pid.Speed_Out_R;

    /* 更新上次误差 */
    pid.Speed_Last_error_R = pid.Speed_current_error_R;

    /* 限幅 */
//    if(pid.Speed_Out > 2000)   pid.Speed_Out = 2000;
//    else if(pid.Speed_Out <= 0) pid.Speed_Out = 0;
//    if(pid.Speed_Out <= 0) pid.Speed_Out = 0;

    /* 返回结果 */
    return pid.Speed_Out_R;

}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介   转向环PD--位置式
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
float PID_Turn_Loc(float Mid_Error)
{
    /* 计算转向误差 */
    pid.Turn_current_error = (120 / White_Column_MID + 1) * Mid_Error;

    /* 误差运算得出打角 */
    pid.Turn_Out = (pid.Turn_KP * pid.Turn_current_error)+((pid.Turn_current_error *abs(pid.Turn_current_error))*pid.Turn_KP1 ) + (pid.Turn_current_error - pid.Turn_last_error) * pid.Turn_KD + (imu963ra_gyro_z * pid.Turn_GKD);

    /* 记下上次误差值 */
    pid.Turn_last_error = pid.Turn_current_error;

    /* 限幅 */
//    if(pid.Turn_Out > 600)   pid.Turn_Out = 600;
//    else if(pid.Turn_Out < -600) pid.Turn_Out = -600;

    /* 返回结果 */
    return pid.Turn_Out;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介   偏航角角速度环PI
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
int16 Angle_PID(int16 Angle_target, int Angle_current)
{
    /* 计算速度误差 */
    pid.Angle_current_error = Angle_target - Angle_current;

    /* 更新“上次误差”用于滤波 */
    pid.Angle_Last_Out = pid.Angle_current_error;

    /* 误差累加积分 */
    pid.Angle_All_Error += pid.Angle_current_error;

    /* 误差运算 */
    pid.Angle_Out = (pid.Angle_KP * pid.Angle_current_error) + (pid.Angle_KI * pid.Angle_All_Error);


    /* 限幅 */


    /* 返回结果 */
    return pid.Angle_Out;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介   PID决策
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void PID_DecisionMaking(void)
{
    /* 停车----------------------------------------------------- */
    pid.Speed_KP_Stop = 0;
    pid.Speed_KI_Stop = 0;

    /* 速度环---------------------------------------------------- */
    pid.Speed_KP_L = pid.Speed_KP_R = 13.0;
    pid.Speed_KI_L = pid.Speed_KI_R = 0.50;//0.3
    /*************/

    /* 转向环---------------------------------------------------- */
    /* 圆环--170速 */
    if((Find_Right_FLAG != Right_0) || (Find_Left_FLAG != Left_0))
    {
        pid.Turn_KP = 13.5;//365
        pid.Turn_GKD = 0.00;//0.1

        pid.Turn_KD = 0;
    }
    /* 正常转向 */
    else
    {
        /**** 转向环 ****/
        pid.Turn_KP = 13.5;//320
        pid.Turn_GKD = 0;//0

        pid.Turn_KD = 0;//-0.05
        /*************/
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介   按键调节
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
//void PID_Setting(void)
//{
//     /* 转向环 */
//    pid.Turn_KP = T_p;//320
//    pid.Turn_GKD = T_GKD;//0
//	
//    /* 差速的KP */
//    KP_x_Increase = kp_x_up;     //*
//    KP_x_Decrease = kp_x_down;    //*

//    pid.Speed_KP_Stop = Stop_p;  //*
//	 pid.Turn_KD = 0;//0
//}

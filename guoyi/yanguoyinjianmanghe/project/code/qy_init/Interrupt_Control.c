#include "Interrupt_Control.h"

#define Left_Out_Max  7000
#define Right_Out_Max 7000

#define Start_time    200

/* 转向环 */
int Turn_PWM = 0;                   //最终转向的PWM

/* 停车标志位 */
int Stop_Car_flag = 0;

/* 转向环 */
#define MID_ERROR_MAX       10     //当前权重下的最大终点误差(用于速度决策)

int16 Turn_Out = 0;                 //转向环输出
int16 Turn_Out_MAX = 0;             //最大转向输出

/* 速度环 */
int16 TargetSpeed = 130;           //目标速度  190

int16 nowtargetSpeed;
int16 targetSpeed_min = 130;    //弯道速度

int16 Speed_Left_Out;                    //速度环输出
int16 Speed_Right_Out;

/* 速度模式 */
int Speed_Mode = 2;     //速度模式--1为线性，2为匀速

uint32 time = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     CCU60_CH0中断----控制中断
// 参数说明
// 返回参数
// 使用示例
// 备注信息     中点误差处理、转向环、速度环、电机控制
//------------------------------------------------------------------------------------------------------------------
void Interrupt_CCU60_CH0(void)
{
//	Test_Speed();
//	return;
	/* 时间变量自增 */
	if(time <= 15000) 
	{
		time++;
	}
	else
	{
		COM_QY = 0;
	}
	/* 转向环 ---------------------------------------------------------*/
	/* 处理陀螺仪积分标志位 */
	use_gyro_flag();

	/* 速度环 --------------------------------------------------------*/
	/* 读取编码器的值 */
	Encoder_Left = Left_Encoder_Get();
	Encoder_Right = Right_Encoder_Get();

	/* 平均值 */
	Encoder_Average = (Encoder_Left + Encoder_Right) / 2;

	/* 用于计算赛道长度 */
	Count_Length();

	/* 处理积分标志位 */
	use_encoder_flag();

//	/* 停车状态机 */
	Stop_Car();

	/* 非停车状态时正常控制，若停车 */
	if(Stop_Car_flag == 0)
	{
		/* 速度决策 */
		Speed_DecisionMaking();
	}
	/* 转向环输出 */
	Turn_Out = PID_Turn_Loc(Image_error);  //中点误差扔进转向环PID

	/* 得到舵机最后输出的值 */
	Turn_PWM = Turn_Out + SERVO_MOTOR_Mid; //4200~4900
	/* 转向环限幅 */
	if(Turn_PWM >= SERVO_MOTOR_L_MAX) Turn_PWM = SERVO_MOTOR_L_MAX;
	else if(Turn_PWM <= SERVO_MOTOR_R_MAX) Turn_PWM = SERVO_MOTOR_R_MAX;
	/* 速度环串转向环 -------------------------------------------------- */
	/* 左右轮闭环输出  */
	if(COM_QY == 1)
	{
		

		if(Image_error < 0 )
		{
			Speed_Left_Out  = PID_Speed_Loc_L(nowtargetSpeed - (Turn_Out * KP_x_Decrease),Encoder_Left);
			Speed_Right_Out = PID_Speed_Loc_R(nowtargetSpeed + (Turn_Out * KP_x_Increase),Encoder_Right);
		}
		else if(Image_error > 0)
		{
			Speed_Left_Out  = PID_Speed_Loc_L(nowtargetSpeed - (Turn_Out * KP_x_Increase),Encoder_Left);
			Speed_Right_Out = PID_Speed_Loc_R(nowtargetSpeed + (Turn_Out * KP_x_Decrease),Encoder_Right);
		}
		else if(Image_error == 0)
		{
			/* 舵机回归中值 */
//			Turn_PWM = SERVO_MOTOR_Mid;
			Speed_Left_Out = PID_Speed_Loc_L(nowtargetSpeed,Encoder_Left);
			Speed_Right_Out = PID_Speed_Loc_R(nowtargetSpeed,Encoder_Right);
	   }
	}
	else
	{
//		Turn_PWM = SERVO_MOTOR_Mid;
		Speed_Left_Out = 0;
		Speed_Right_Out = 0;
	}

	/* 速度环并转向环 -------------------------------------------------- */
	Left_Out = Speed_Left_Out;
	Right_Out = Speed_Right_Out;

	/* 限幅 */
	if(Left_Out >= Left_Out_Max) Left_Out = Left_Out_Max;
	else if(Left_Out <= -Left_Out_Max) Left_Out = -Left_Out_Max;
	if(Right_Out >= Right_Out_Max) Right_Out = Right_Out_Max;
	else if(Right_Out <= -Right_Out_Max) Right_Out = -Right_Out_Max;

////	/* 终点线停车 */
//	End_Line_Stop_Car();

////	/* 丢线保护 */
//	Lost_Line_Protect();
	
	/* 控制 ------------------------------------------------------------*/
	/* 舵机 */
	pwm_set_duty(SERVO_MOTOR_FREQ,Turn_PWM);
	
	/* 电机控制 */
	Left_Motor_Speed(Left_Out);
	Right_Motor_Speed(Right_Out);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     速度决策
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Speed_DecisionMaking(void)
{
    /* 速度决策--弯道、圆环降速 */
    if(((Find_Right_FLAG > Right_0) && (Find_Right_FLAG < Right_6)) || ((Find_Left_FLAG > Left_0) && (Find_Left_FLAG < Left_6)))
//    if((Find_Right_FLAG != Right_0) || (Find_Left_FLAG != Left_0))
    {
//		pid.Turn_KP =10.5;
        nowtargetSpeed = TargetSpeed;
        if(((Encoder_Average >= TargetSpeed) && (Find_Right_FLAG == Right_1)) || ((Encoder_Average >= TargetSpeed) && (Find_Left_FLAG == Left_1)))
        {
            pid.Speed_KP_L = pid.Speed_KP_R = 28.9;
            pid.Speed_KI_L = pid.Speed_KI_R = 2.25;//0.3
        }

    }
    /* 弯道速度 */
    else if((White_Column_MID < 105) || (Find_Left_FLAG == Left_6) || (Find_Right_FLAG == Right_6))
    {
			pid.Turn_GKD =0.001;
			KP_x_Increase = 0.002;
			KP_x_Decrease = 0.002;
			pid.Turn_KP =10;
//        nowtargetSpeed = targetSpeed_min;
    }
    /* 直道速度 */
    else
    {
         /* 线性加速模式 */
        if(Speed_Mode == 1)
        {
            if(nowtargetSpeed < targetSpeed_min) nowtargetSpeed = targetSpeed_min;
            nowtargetSpeed = nowtargetSpeed + 3;
            if(nowtargetSpeed >= TargetSpeed) nowtargetSpeed = TargetSpeed;
        }
        /* 匀速模式 */
        else if(Speed_Mode == 2)
        {
			pid.Turn_GKD =0.001;
			KP_x_Increase = 0.001;
			KP_x_Decrease = 0.001;
			pid.Turn_KP =8.5;
            nowtargetSpeed = TargetSpeed;
        }
	  }
}

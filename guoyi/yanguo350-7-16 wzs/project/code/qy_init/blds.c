#include "blds.h"

// 电调支持50hz-300hz的控制频率
		// 50Hz的控制频率 ，从0%到100%占空比为500到1000
		// 100Hz的控制频率，从0%到100%占空比为1000到2000
		// 200Hz的控制频率，从0%到100%占空比为2000到4000
		// 300Hz的控制频率，从0%到100%占空比为3000到6000
/* 无刷PWM  1000~2000 */

int16 MY_Blds_PWM_MAX = 4500;    //芯片最大PWM

int16 Blds_PWM_MAX = 5500;    //芯片最大PWM
int16 Blds_PWM_MIN = 3000;    //芯片最大PWM

/* 检测无刷开启标志位 */
int8 Blds_flag = 0;

/* 无刷转速变量 */
int16 Blds_Speed = 1000;

uint16 Blds_time = 0;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷初始化
// 参数说明
//
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Blds_Init(void)
{
    pwm_init(Blds_PWM_CH1, 300, 0);                                                 // PWM 通道1 初始化频率100Hz 占空比初始为0

    pwm_init(Blds_PWM_CH2, 300, 0);                                                 // PWM 通道2 初始化频率100Hz 占空比初始为0
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     左无刷驱动
// 参数说明     Speed           PWM占空比 （1000-2000）
//
// 返回参数     void
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Left_Blds_Speed(int16 Speed)
{
    /* 限幅 */
    if(Speed > Blds_PWM_MAX) Speed = Blds_PWM_MAX;
    else if(Speed < Blds_PWM_MIN) Speed = Blds_PWM_MIN;

    pwm_set_duty(Blds_PWM_CH2,Speed);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     右无刷驱动
// 参数说明     Speed           PWM占空比 （1000-2000）
//
// 返回参数     void
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Right_Blds_Speed(int16 Speed)
{
    /* 限幅 */
    if(Speed > Blds_PWM_MAX) Speed = Blds_PWM_MAX;
    else if(Speed < Blds_PWM_MIN) Speed = Blds_PWM_MIN;

    pwm_set_duty(Blds_PWM_CH1,Speed);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     检测是否开启无刷
// 参数说明
//
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Open_Blds(void)
{
	uint16 Blds_Speed_num = 0;
	if(COM_QY == 1)
	{
		if(Blds_time < 1000)
		{
			Blds_time++;
			Blds_Speed_num = (MY_Blds_PWM_MAX - Blds_PWM_MIN)/1000;
			if(Blds_Speed < MY_Blds_PWM_MAX) Blds_Speed = Blds_Speed + Blds_Speed_num;
			
			/* 限幅 */
			if(Blds_Speed < Blds_PWM_MIN) Blds_Speed = Blds_PWM_MIN;
			else if(Blds_Speed > MY_Blds_PWM_MAX) Blds_Speed = MY_Blds_PWM_MAX;
		}
		else
		{
			COM_QY = 2;
		}
		Left_Blds_Speed(Blds_Speed);
        Right_Blds_Speed(Blds_Speed);
	}
//	else if(COM_QY == 2)
//	{
//		if(TargetSpeed_R > TargetSpeed_L + nowtargetSpeed)
//		{
//			Left_Blds_Speed(MY_Blds_PWM_MAX);
//			Right_Blds_Speed(MY_Blds_PWM_MAX + 500);
//		}
//		else if(TargetSpeed_L > TargetSpeed_R + nowtargetSpeed)
//		{
//			Left_Blds_Speed(MY_Blds_PWM_MAX + 500);
//			Right_Blds_Speed(MY_Blds_PWM_MAX);
//		}
//		else
//		{
//		Left_Blds_Speed(MY_Blds_PWM_MAX);
//		Right_Blds_Speed(MY_Blds_PWM_MAX);
//		}
//	}
	else if(COM_QY == 0)
	{
		Blds_time = 0;
		Blds_Speed_num = 0;
		Left_Blds_Speed(0);
        Right_Blds_Speed(0);
	}
}

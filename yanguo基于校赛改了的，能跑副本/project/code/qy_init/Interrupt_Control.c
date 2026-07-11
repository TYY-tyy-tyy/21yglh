#include "Interrupt_Control.h"
 
/* 转向环 */
int16 Turn_PWM = 0;                   //最终转向的PWM

/* 停车标志位 */
uint8 Stop_Car_flag = 0;

int16 Turn_Out = 0;                 //转向环输出
int16 Turn_Out_MAX = 0;             //最大转向输出



int16 nowtargetSpeed;
int16 targetSpeed_min ;    //弯道速度

int16 Speed_Left_Out;                    //速度环输出
int16 Speed_Right_Out;

/* 速度模式 */
uint8 Speed_Mode = 2;     //速度模式--1为线性，2为匀速

uint16 time = 0;
// 菜单参数
uint8 menu_cursor = 0;         // 0=方案1  1=方案2
uint8 select_plan = 0;         // 当前使用方案

// 方案1
int16  TargetSpeed_1 = 180;
int16  Turn_KP_1    = 11;
int16  Turn_GKD_1   = 0.005;

// 方案2
int16  TargetSpeed_2 = 250;
int16  Turn_KP_2    = 12.5;
int16  Turn_GKD_2   = 0.005;
float t;
int16 L = 20;
int16 K = 15;
float diff;
int16 y;
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
	nowtargetSpeed = my_Speed;
	T_KP = get_y(Image_error);//40 44
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
	if(COM_QY == 2)
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
	t = tan((Turn_PWM - 4550) * 0.001176f);
    diff = ACKERMAN_COEFF * t;
	
	/* 速度环串转向环 -------------------------------------------------- */
	/* 左右轮闭环输出  */
	if(COM_QY == 2)
	{
		Speed_Left_Out  = PID_Speed_Inc_L(nowtargetSpeed * (1.0f - diff), Encoder_Left);
        Speed_Right_Out = PID_Speed_Inc_R(nowtargetSpeed * (1.0f + diff), Encoder_Right);
    }
    else
    {
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
    if((Find_Left_FLAG >= Left_1) || (Find_Right_FLAG >= Right_1))
    {
        pid.Turn_KP = Ring_T_KP;//44 47
        nowtargetSpeed = my_Speed/10*9;
    }
    else if(White_Column_MID > 110 )
    {
        pid.Turn_KP = W_T_KP;//20
        nowtargetSpeed = my_Speed;
    }
	 else if((White_Column_MID > 110) && (Image_error<=-20 || Image_error>=20))
    {
        pid.Turn_KP = 60;//20
        nowtargetSpeed = my_Speed/10*9;
		
    }
//	else if(ring_preMeet_flag == 1
//		||(Left_Lost_Line_count>= 10 && Right_Lost_Line_count <= 10)
//		||(Left_Lost_Line_count <= 10 && Right_Lost_Line_count >= 10))
//	{
//      // zero_cross_row 附近减小方向变化率，避免突变
//		pid.Turn_KP = 60;//20
//		if(abs(pid.Turn_last_error) > 5)
//		{
//		   nowtargetSpeed = my_Speed/10*9;  // 限幅
//		}
//	}
    else
    {
        pid.Turn_KP = T_KP;     
        pid.Turn_KP1 = T_KP1;
        nowtargetSpeed = my_Speed / 10*9;
    }
}

//==================== 图像下方菜单 ====================
void show_menu(void)
{
    tft180_show_string(0, 65, "Plan:");
    
    if(menu_cursor == 0)
        tft180_show_string(40, 65, ">1");
    else
        tft180_show_string(40, 65, " 1");
        
    if(menu_cursor == 1)
        tft180_show_string(60, 65, ">2");
    else
        tft180_show_string(60, 65, " 2");

    tft180_show_string(0, 80, "Speed:");
    tft180_show_string(0, 95, "KP:");
    tft180_show_string(MT9V03X_W / 2, 0, "GKD:");

    if(select_plan == 1)
    {
        tft180_show_int16(45, 80, TargetSpeed_1);
        tft180_show_float(24, 95, Turn_KP_1, 2, 2);
		tft180_show_float(MT9V03X_W / 2, 16, Turn_GKD_1, 1, 3);
    }
    else
    {
        tft180_show_int16(45, 80, TargetSpeed_2);
        tft180_show_float(24, 95, Turn_KP_2, 2, 2);
        tft180_show_float(MT9V03X_W / 2, 16, Turn_GKD_2, 1, 3);
    }

    if(COM_QY == 1)
        tft180_show_string(0, 110, "RUN");
    else
        tft180_show_string(0, 110, "STOP");
}

int clamp_x(int16 x)
{
    if (x < -52) return -52;
    if (x > 52)  return 52;
    return x;
}
int get_y(int16 x)
{
    int16 x_val = clamp_x(x);
    int16 abs_x = abs(x_val);
    // 整数计算：y = 46 + (3 \* |x|) / 52
    int16 y = 51 + (5 * abs_x) / 52;//48 
    return y;
}

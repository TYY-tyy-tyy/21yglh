#include "Find_End_Line.h"

/* 突变点 */
int Mutation_count = 0;

/* 终点线标志位 */
int End_line_Flag = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找终点线
// 参数说明
// 返回参数
// 使用示例
// 备注信息     用于丢线保护
//------------------------------------------------------------------------------------------------------------------
void Find_End_Line(void)
{
	int H;
	int W;
	/* 变量清零 */
	Mutation_count = 0;
	/* 扫三行取平均值 */
	for(H = (Image_H / 2); H <= ((Image_H / 2) + 3); H++)
	{
		for(W = 1; W < Image_W - 1; W++)
		{
			if((image_two_value[H][W] == IMG_WHITE) && (image_two_value[H][W + 1] == IMG_BLACK) && (image_two_value[H][W + 2] == IMG_BLACK))
			{
				 Mutation_count++;
			}
		}
	}
	/* 扫了3行所以取平均值 */
	Mutation_count /= 3;

	if((Mutation_count >= 5) && (time >= 1000))
	{
	  End_line_Flag = 1;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     终点停车
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void End_Line_Stop_Car(void)
{
    if((End_line_Flag == 1) && (Stop_Car_flag == 0))
    {
           /* 保护(停车)标志置1 */
           Stop_Car_flag = 1;

           /* 无刷标志位置0 */
           Blds_flag = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     停车状态机
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Stop_Car(void)
{
    /* 停车状态机 */
    if(Stop_Car_flag == 1)
    {
        /* 过段距离再停车 */
        if((Encoder_jifen_L >= 6000) && (Encoder_jifen_R >= 6000))
        {
            /* 积分标志位置0 */
            Encoder_jifen_flag = 0;

            Stop_Car_flag = 2;
        }
        else
        {
            /* 积分标志位置1 */
            Encoder_jifen_flag = 1;
        }
    }
    else if(Stop_Car_flag == 2)
    {

        /* 目标速度给0 */
        nowtargetSpeed = 0;

        /* 让车摆正误差给0 */
        Turn_Out = 0;
//		pwm_set_duty(Servo_PWM,SERVO_MOTOR_Mid);
        /* 左右轮闭环输出  */
        Speed_Left_Out = PID_Speed_Loc_Stop(nowtargetSpeed,Encoder_Left);
        Speed_Right_Out = PID_Speed_Loc_Stop(nowtargetSpeed,Encoder_Right);
    }
}

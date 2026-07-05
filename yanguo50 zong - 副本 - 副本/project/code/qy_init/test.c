#include "test.h"

void Test_Speed(void)
{
    /* 时间变量自增 */
    if(time <= 3000) time++;


//    if(Diswitch_Key_3 == 0) pid.Speed_KP_L = pid.Speed_KP_R = data_uart;//65
//    else pid.Speed_KI_L = pid.Speed_KI_R = data_uart;//0.2

    /* 读取编码器的值 */
    Encoder_Left = Left_Encoder_Get();
    Encoder_Right = Right_Encoder_Get();

//    if((time > 500) && (time < 1000)) targetSpeed = 160;
//    else if(time <= 500) targetSpeed = 240;
//    else targetSpeed = 240;

//    if(time > 300) targetSpeed = 0;
//    else targetSpeed = 150;
    nowtargetSpeed = my_Speed;
	
	if(COM_QY == 1)
	{
		Speed_Left_Out = PID_Speed_Loc_L(nowtargetSpeed,Encoder_Left);
		Speed_Right_Out = PID_Speed_Loc_R(nowtargetSpeed,Encoder_Right);
	}
	else
	{
		Speed_Left_Out = 0;
		Speed_Right_Out = 0;
	}

    Left_Motor_Speed(Speed_Left_Out);
    Right_Motor_Speed(Speed_Right_Out);

//    if(time <= 500)
//    {
//        Left_Motor_Speed(1000);
//        Right_Motor_Speed(1000);
//    }
//    else
//    {
//        Left_Motor_Speed(Speed_Left_Out);
//        Right_Motor_Speed(Speed_Right_Out);
//    }


}

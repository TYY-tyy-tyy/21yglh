#include "Find_Ring.h"

/* ------------------------------------------------ */
/* 陀螺仪积分角度 */
int16 Gyro_In_Left = 70;//70
int16 Gyro_Out_Left = 60;

int16 Gyro_In_Right = 70;
int16 Gyro_Out_Right = 80;

/* 环的数量 */
int16 Left_Ring_num = 5;
int16 Right_Ring_num = 5;

/* 出入环积分 */
uint16 Left_Enc_In = 6000;
uint16 Left_Enc_Out = 4000;
uint16 Right_Enc_In = 6000;
uint16 Right_Enc_Out = 4000;

/* ------------------------------------------------ */

/* 单行白点数量 */
int16 White_Nums = 0;

/* 借存储坐标变量 */
int16 top[2] = {0, 0};
int16 bot[2] = {0, 0};

/* 脉冲积分 */
uint16 Encoder_jifen_L;
uint16 Encoder_jifen_R;

/* 陀螺仪角度记录变量 */
int16 gyro_angle_start;
int16 gyro_angle_end;

uint16 Ring_time = 0;

/* 进环的次数 */
int8 Left_time = 0;
int8 Right_time = 0;

/* 入环后直行延时脉冲数（防止提前转弯） */
uint16 Left_Enc_Delay = 600;
uint16 Right_Enc_Delay = 600;

/* -----------------------左圆环------------------------- */
/* 标志位 */
int8 Find_Left_FLAG = 0;

/* 左边局部丢线数量 */
int8 Left_local_LostNums = 0;

/* -----------------------右圆环------------------------- */
/* 标志位 */
int8 Find_Right_FLAG = 0;

/* 右边局部丢线数量 */
int8 Right_local_LostNums = 0;

uint8 r_con,l_con;

uint8 ring_preMeet_flag=0;

uint8 Ring_in_local_flag = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找左圆环
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Find_Left_Ring(void)
{
    /* 判断是否为圆环 */
    if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0) && (Left_time < Left_Ring_num) && Ring_time < 2)
    {
        if((Left_dowm_Patch >= 20)//(Left_dowm_Patch >= 20)
			&& (Right_dowm_Patch == 0) && (Left_local_LostNums >= 13) 
			&& (Left_Lost_Line_count <= 40) && (Right_Lost_Line_count <= 10) 
//			&& (Right_Lost_Line_count == Right_local_LostNums)
			&& (pid.Turn_last_error > -30) && (pid.Turn_last_error < 30) 
//			&& (ring_preMeet_flag == 1)
			&& (White_Column_MID >= 100) && (White_Nums > 85))
        {
            //若是，则进入圆环标志位1
//			COM_QY = 0;
            Find_Left_FLAG = Left_1;

            angle_ringR = 0;

            //编码器积分标志位置1
            Encoder_jifen_flag = 1;

            //陀螺仪积分标志位置1
            gyro_jifen_flag = 1;

            //蜂鸣器响起
            Buzzer_ON();
//			all_on();

            //数据清零
            White_Nums = 0;
            gyro_angle_start = 0;
            gyro_angle_end = 0;
			Ring_in_local_flag = 0;
//			Left_Patch_Init();
        }
//        else
//        {
//            //避免大拐弯时的误判圆环
//            White_Nums = White_counts_weight(30);
//            //统计局部的丢线数量
//            Left_local_LostNums = Count_Left_Lost(40,25);
//            //避免十字误判圆环
//            Right_dowm_Patch = Find_Right_dowm_point(58,20);
//        }

    }
    /* 状态一 */
    else if(Find_Left_FLAG == Left_1)
    {
        //当左右轮积分大于1500时
//		if((pid.Turn_last_error < -20) || (pid.Turn_last_error > 20))
//		{
//			Find_Left_FLAG = Left_0;
//		}
		Buzzer_OFF();
		if(Left_dowm_Patch >= 40)
		{
			if(((Encoder_jifen_L + Encoder_jifen_R) / 2) > Left_Enc_In/10*11)
			{
//				COM_QY = 0;
				//标志位更新
				Find_Left_FLAG = Left_2;

				//编码器积分标志位置0
				Encoder_jifen_flag = 0;

				//陀螺仪积分标志位置1
				gyro_jifen_flag = 1;

				//关闭蜂鸣器
				Buzzer_ON();
			}
		}
		else if(Left_dowm_Patch < 40)
		{
			if(((Encoder_jifen_L + Encoder_jifen_R) / 2) > Left_Enc_In)
			{
//				COM_QY = 0;
				//标志位更新
				Find_Left_FLAG = Left_2;

				//编码器积分标志位置0
				Encoder_jifen_flag = 0;

				//陀螺仪积分标志位置1
				gyro_jifen_flag = 1;

				//关闭蜂鸣器
				Buzzer_ON();
			}
		}
    }
    /* 状态二 */
    else if(Find_Left_FLAG == Left_2)
    {
		Buzzer_OFF();
        White_Nums = White_counts_weight(80);
        if (White_Nums > 135)
        {
//			COM_QY = 0;
            Find_Left_FLAG = Left_3;

            //数据清零
            White_Nums = 0;

            //重新启动编码器积分用于直行延时
            Encoder_jifen_flag = 1;
			Buzzer_ON();
        }
    }
    /* 状态三 */
    else if (Find_Left_FLAG == Left_3)
    {
//		if (Ring_in_local_flag == 0)
//		{
//			if (((Encoder_jifen_L + Encoder_jifen_R) / 2) > Left_Enc_Delay)
//			{
//				Ring_in_local_flag = 1;        // 直行够了，进入转弯子状态

//				Encoder_jifen_flag = 0;        // 关编码器积分
//				gyro_jifen_flag = 1;           // 开陀螺仪积分（开始计转弯角度）

//				gyro_angle_dif = 0;
//				angle_ringR = 0;
//			}
//		}
        //转向够角度后停止拉线,且左丢线数小于10时
        if (angle_ringR > 70)
        {
//			COM_QY = 0;
            Find_Left_FLAG = Left_4;

            /* 蜂鸣器关闭 */
            Buzzer_OFF();

            /* 变量清零 */
            gyro_angle_dif = 0;

            /* 陀螺仪标志位置1 */
            gyro_jifen_flag = 1;
			
			/* 编码器积分标志位置1 */
            Encoder_jifen_flag = 0;
        }
        else
        {
//			Find_Left_FLAG = Left_0;
            /* 拉线进环 */
//			Right_Patch_Init();
			/* 拉线进环 */
			bot[0] = Right_Coordinates[110];
            bot[1] = 110;

            top[0] = 80;
            top[1] = 1;

            Patch_line_Right(bot, top);

            //蜂鸣器响起
            Buzzer_ON();
//			all_on();
        }
     }
     /* 状态四 */
     else if(Find_Left_FLAG == Left_4)
     {
         //转向够角度后停止拉线,且左丢线数小于10时
        if (angle_ringR > 240 && Right_Lost_Line_count > 5)
        {
//			COM_QY = 0;
            Find_Left_FLAG = Left_5;

            //蜂鸣器响起
			Buzzer_ON();

            /* 变量清零 */
            gyro_angle_dif = 0;

            /* 陀螺仪标志位置1 */
            gyro_jifen_flag = 1;
			
			/* 编码器积分标志位置1 */
             Encoder_jifen_flag = 0;
        }
        else
        {
            /* 拉线进环 */
//			Left_Patch_Init();

            //蜂鸣器关闭
//             Buzzer_OFF();
        }
     }
     /* 状态五 */
     else if(Find_Left_FLAG == Left_5)
     {
         //转向够角度后停止拉线
         if (Right_Lost_Line_count < 5)
         {
            Find_Left_FLAG = Left_6;

            //蜂鸣器响起
			Buzzer_OFF();

            /* 变量清零 */
            gyro_angle_dif = 0;

            /* 陀螺仪标志位置1 */
            gyro_jifen_flag = 1;
			
			/* 编码器积分标志位置1 */
             Encoder_jifen_flag = 1;
         }
         else
         {
             /* 拉线出环 */
             bot[0] = Right_Coordinates[110];
             bot[1] = 110;

             top[0] = 80;
             top[1] = 1;

             Patch_line_Right(bot, top);

             //蜂鸣器响起
             Buzzer_ON();
//			 all_on();
         }
     }
     /* 状态六 */
     else if(Find_Left_FLAG == Left_6)
     {
         /* 蜂鸣器响起 */
		 Ring_time = 100;
         Buzzer_ON();

         //当左右轮积分大于1500时
         if(((Encoder_jifen_L + Encoder_jifen_R)/2) > Left_Enc_Out)
         {
             Find_Left_FLAG = Left_0;

             Left_time++;

             /* 编码器积分标志位置0 */
             Encoder_jifen_flag = 0;

             //关闭蜂鸣器
             Buzzer_OFF();
         }
		 else
		 {
			 /* 拉线出环 */
             bot[0] = Left_Coordinates[110];
             bot[1] = 110;

             top[0] = 90;
             top[1] = 20;
			 Patch_line_Left(bot, top);
		 }
     }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找右圆环
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Find_Right_Ring(void)
{
    /* 判断是否为圆环 */
    if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0) && (Right_time < Right_Ring_num) && Ring_time < 2)
    {
        if((Right_dowm_Patch >= 20) 
			&& (Left_dowm_Patch == 0) && (Right_local_LostNums >= 13) 
			&& (Right_Lost_Line_count <= 40) && (Left_Lost_Line_count <= 10) 
//			&& (Right_Lost_Line_count == Right_local_LostNums)
			&& (pid.Turn_last_error > -30) && (pid.Turn_last_error < 30) 
//			&& (ring_preMeet_flag == 1)
			&& (White_Column_MID >= 100) && (White_Nums > 85))
        {
//			COM_QY = 0;
            //若是，则进入圆环标志位1
            Find_Right_FLAG = Right_1;
			
			angle_ringR = 0;

            //编码器积分标志位置1
            Encoder_jifen_flag = 1;

            //陀螺仪积分标志位置1
            gyro_jifen_flag = 1;

            //蜂鸣器响起
            Buzzer_ON();
//			all_on();

            //数据清零
            White_Nums = 0;
        }
    }
    /* 状态一 */
    else if(Find_Right_FLAG == Right_1)
    {
//		COM_QY = 0;
        //当左右轮积分大于1500时
//		if((pid.Turn_last_error < -20) || (pid.Turn_last_error > 20))
//		{
//			Find_Right_FLAG = Right_0;
//			Buzzer_OFF();
//		}
		Buzzer_OFF();
		if(Right_dowm_Patch >= 40)
		{
			if(((Encoder_jifen_L + Encoder_jifen_R)/2 > Right_Enc_In/10*11))
			{
					//标志位更新
					Find_Right_FLAG = Right_2;

					//编码器积分标志位置0
					Encoder_jifen_flag = 0;

					//陀螺仪积分标志位置1
					gyro_jifen_flag = 1;

					//关闭蜂鸣器
					Buzzer_ON();
			}
		}
		else if(Right_dowm_Patch < 40)
		{
			if(((Encoder_jifen_L + Encoder_jifen_R)/2) > Right_Enc_In)
			{
					//标志位更新
					Find_Right_FLAG = Right_2;

					//编码器积分标志位置0
					Encoder_jifen_flag = 0;

					//陀螺仪积分标志位置1
					gyro_jifen_flag = 1;

					//关闭蜂鸣器
					Buzzer_ON();
			}
		}

    }
    /* 状态二 */
    else if(Find_Right_FLAG == Right_2)
    {
//		COM_QY = 0;
		Buzzer_OFF();
        White_Nums = White_counts_weight(80);
        if (White_Nums > 135)
        {
            Find_Right_FLAG = Right_3;

            //
            Encoder_jifen_flag = 1;
			
			Buzzer_ON();

            //数据清零
            White_Nums = 0;
        }
    }
    /* 状态三 */
    else if (Find_Right_FLAG == Right_3)
    {
//		COM_QY = 0;
        //转向够角度后停止拉线,且左丢线数小于10时
        if (angle_ringR < -70)
        {
//			COM_QY = 0;
            Find_Right_FLAG = Right_4;

            /* 蜂鸣器关闭 */
            Buzzer_OFF();

            /* 变量清零 */
            gyro_angle_dif = 0;

            /* 陀螺仪标志位置1 */
            gyro_jifen_flag = 1;
			
			/* 编码器积分标志位置1 */
             Encoder_jifen_flag = 0;
        }
        else
        {
            /* 拉线进环 */
			
			Left_Patch_Init();
			
//            bot[0] = 30;
//            bot[1] = 110;

//            top[0] = 186;
//            top[1] = 10;

//            Patch_line_Left(bot, top);

            //蜂鸣器响起
            Buzzer_ON();
//			all_on();
        }
     }
     /* 状态四 */
     else if(Find_Right_FLAG == Right_4)
     {
//		 COM_QY = 0;
		if (angle_ringR < -240 && Left_Lost_Line_count > 5)
		{
//			COM_QY = 0;
			Find_Right_FLAG = Right_5;

			/* 蜂鸣器关闭 */
			Buzzer_ON();

			/* 变量清零 */
			gyro_angle_dif = 0;

			/* 陀螺仪标志位置1 */
			gyro_jifen_flag = 1;

			/* 编码器积分标志位置1 */
			 Encoder_jifen_flag = 0;
		}
//		else
//		{
//			Right_Patch_Init();
//		}
     }
     /* 状态五 */
     else if(Find_Right_FLAG == Right_5)
     {
//		 COM_QY = 0;
         //转向够角度后停止拉线
         if (Left_Lost_Line_count < 5)
         {
			Find_Right_FLAG = Right_6;

			/* 蜂鸣器关闭 */
			Buzzer_OFF();

			/* 变量清零 */
			gyro_angle_dif = 0;

			/* 陀螺仪标志位置1 */
			gyro_jifen_flag = 1;

			/* 编码器积分标志位置1 */
			Encoder_jifen_flag = 1;
         }
         else
         {
             /* 拉线出环 */
//			 Left_Patch_Init();
			 
             bot[0] = Left_Coordinates[110];
             bot[1] = 110;

             top[0] = Left_Coordinates[10];
             top[1] = 40;

             Patch_line_Left(bot, top);

             //蜂鸣器响起
             Buzzer_ON();
         }
     }
     /* 状态六 */
     else if(Find_Right_FLAG == Right_6)
     {
//		 COM_QY = 0;
         /* 蜂鸣器响起 */
		Buzzer_ON();
		Ring_time = 100;

         //当左右轮积分大于1500时
         if(((Encoder_jifen_L + Encoder_jifen_R)/2) > Right_Enc_Out)
         {
             Find_Right_FLAG = Right_0;

             Right_time++;

             /* 编码器积分标志位置0 */
             Encoder_jifen_flag = 0;

             //关闭蜂鸣器
             Buzzer_OFF();
         }
         else
         {
//			Right_Patch_Init();
			bot[0] = Right_Coordinates[110];
			bot[1] = 110;

			top[0] = Right_Coordinates[10];
			top[1] = 10;

             Patch_line_Right(bot, top);
         }
     }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找圆
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Find_Ring(void)
{
	if(Ring_time > 0)
	{
		Ring_time --;
	}
//    if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
//    {
		r_con=Right_ContinueLine();
		l_con=Left_ContinueLine();

		if(r_con != l_con)
		{
			ring_preMeet_flag=1;
		}
		else
		{
			ring_preMeet_flag=0;
		}
		
        //避免大拐弯时的误判圆环
        White_Nums = White_counts_weight(40);

        //统计局部的丢线数量
        Left_local_LostNums = Count_Left_Lost(110,50);
        Right_local_LostNums = Count_Right_Lost(110,50);//40 25

        //避免十字误判圆环
        Left_dowm_Patch = Find_left_dowm_point(110,20);
        Right_dowm_Patch = Find_Right_dowm_point(110,20);//58 20
//    }
	
	Find_Right_Ring();
	Find_Left_Ring();
}

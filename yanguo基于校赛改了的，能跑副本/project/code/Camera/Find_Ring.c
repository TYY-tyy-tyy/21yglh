#include "Find_Ring.h"

/* ------------------------------------------------ */
/* 陀螺仪积分角度 */
int16 Gyro_In_Left = 70;//70
int16 Gyro_Out_Left = 80;

int16 Gyro_In_Right = 70;
int16 Gyro_Out_Right = 80;

/* 环的数量 */
int16 Left_Ring_num = 5;
int16 Right_Ring_num = 5;

/* 出入环积分 */
uint16 Left_Enc_In = 4000;
uint16 Left_Enc_Out = 15000;
uint16 Right_Enc_In = 4000;
uint16 Right_Enc_Out = 15000;

uint8 ring_preMeet_flag=0;

/* ------------------------------------------------ */

/* 单行白点数量 */
int8 White_Nums = 0;

/* 脉冲积分 */
uint16 Encoder_jifen_L;
uint16 Encoder_jifen_R;

/* 陀螺仪角度记录变量 */
int16 gyro_angle_start;
int16 gyro_angle_end;

/* 进环的次数 */
int8 Left_time = 0;
int8 Right_time = 0;

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
	if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
	{
		if((Right_dowm_Patch == 0) && (Left_dowm_Patch >= 40
			&& (Left_Lost_Line_count >= 20) && (Right_Lost_Line_count <= 5) )
			&& (White_Column_MID >= 90) && ring_preMeet_flag == 1)
		{
			//若是，则进入圆环标志位1
			Find_Right_FLAG = Right_1;
			
			Encoder_jifen_L = 0;
			Encoder_jifen_R = 0;
			angle_ringR = 0;

			//编码器积分标志位置1
			Encoder_jifen_flag = 1;

			//蜂鸣器响起
			Buzzer_ON();
			
			/* 拉线 */
			Left_Patch_Init();
		}
	}
	/* 状态一 */
	else if(Find_Left_FLAG == Left_1)
	{
		//当左右轮积分大于1500时
		if(((Encoder_jifen_L + Encoder_jifen_R) / 2) > Left_Enc_In)
		{
			//标志位更新
			if(White_Column_MID >= 80)
			{
				Find_Left_FLAG = Left_2;
				/* 拉线进环 */
				if(Left_up_Patch < 80)
				{
					Left_up_Patch = 80;
				}
				Right_Patch_Init();

				/* 蜂鸣器响起 */
				Buzzer_ON();
			}
			else
			{
				Find_Left_FLAG = Left_0;
			}

			//编码器积分标志位置0
			Encoder_jifen_flag = 0;

			//关闭蜂鸣器
			Buzzer_OFF();
		}
		else
		{
			Left_Patch_Init();
		}
	}
	/* 状态二 */
	else if(Find_Left_FLAG == Left_2)
	{
		//转向够角度后停止拉线,且左丢线数小于10时
		if (angle_ringR > 60)
		{
			Find_Left_FLAG = Left_3;

			//蜂鸣器关闭
			Buzzer_OFF();
		}
		else
		{
			/* 拉线进环 */
			if(Left_up_Patch < 80)
			{
				Left_up_Patch = 80;
			}
			Right_Patch_Init();

			/* 蜂鸣器响起 */
			Buzzer_ON();
		}
	}
	/* 状态三 */
	else if (Find_Left_FLAG == Left_3)
	{
		if (angle_ringR > 220)
		{
			 Find_Left_FLAG = Left_4;
		}
	}
	/* 状态四 */
	else if(Find_Left_FLAG == Left_4)
	{
		//转向够角度后停止拉线
		if (angle_ringR > 300)
		{
			Find_Left_FLAG = Left_5;

			/* 编码器积分标志位置1 */
			Encoder_jifen_flag = 1;

			/* 蜂鸣器响起 */
			Buzzer_ON();
			Left_Patch_Init();
		}
		else
		{
			/* 拉线出环 */
			Right_Patch_Init();

			//蜂鸣器响起
			Buzzer_ON();
		}
	}
	/* 状态五 */
	else if(Find_Left_FLAG == Left_5)
	{
		//当左右轮积分大于1500时
		if(((Encoder_jifen_L + Encoder_jifen_R) / 2) > Left_Enc_Out)
		{
			Find_Left_FLAG = Left_0;

			/* 编码器积分标志位置0 */
			Encoder_jifen_flag = 0;

			//关闭蜂鸣器
			Buzzer_OFF();
		}
		else
		{
			if(White_Column_MID <= 40)
			{
				Find_Left_FLAG = Left_0;

				/* 编码器积分标志位置0 */
				Encoder_jifen_flag = 0;

				//关闭蜂鸣器
				Buzzer_OFF();
			}
			else
			{
				/* 蜂鸣器响起 */
				Buzzer_ON();
				Left_Patch_Init();
			}
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
	if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
	{
		if((Right_dowm_Patch >= 40) && (Left_dowm_Patch == 0)
			&& (Right_Lost_Line_count >= 20) && (Left_Lost_Line_count <= 5)
			&& (White_Column_MID >= 90) && ring_preMeet_flag == 1)
		{
//			COM_QY = 0;
			//若是，则进入圆环标志位1
			Find_Right_FLAG = Right_1;
			
			Encoder_jifen_L = 0;
			Encoder_jifen_R = 0;
			angle_ringR = 0;

			//编码器积分标志位置1
			Encoder_jifen_flag = 1;

			//蜂鸣器响起
			Buzzer_ON();
			
			Right_Patch_Init();
		}
	}
	/* 状态一 */
	else if(Find_Right_FLAG == Right_1)
	{
		//当左右轮积分大于1500时
		if(((Encoder_jifen_L + Encoder_jifen_R )/2) > Right_Enc_In)
		{
//			COM_QY = 0;
			if(White_Column_MID >= 70)
			{
				Find_Right_FLAG = Right_2;
				/* 拉线进环 */
				if(Right_up_Patch < 80)
				{
					Right_up_Patch = 80;
				}
				Left_Patch_Init();

				/* 蜂鸣器响起 */
				Buzzer_ON();
			}
			else
			{
//				COM_QY = 0;
				Find_Right_FLAG = Right_0;
			}
			//编码器积分标志位置0
			Encoder_jifen_flag = 0;

			//关闭蜂鸣器
			Buzzer_OFF();
		}
		else
		{
			Right_Patch_Init();
		}
	}
	/* 状态二 */
	else if(Find_Right_FLAG == Right_2)
	{
		if (angle_ringR < -60)
		{
			Find_Right_FLAG = Right_3;
			
			/* 蜂鸣器关闭 */
			Buzzer_OFF();
		}
		else
		{
			/* 拉线进环 */
			if(Right_up_Patch < 80)
			{
				Right_up_Patch = 80;
			}
			Left_Patch_Init();

			//蜂鸣器响起
			Buzzer_ON();
		}
	}
	/* 状态三 */
	else if (Find_Right_FLAG == Right_3)
	{
		if (angle_ringR < -220)
		{
			 Find_Right_FLAG = Right_4;
		}
	}
	/* 状态四 */
	else if(Find_Right_FLAG == Right_4)
	{
		//转向够角度后停止拉线
		if (angle_ringR < -300)
		{
			Find_Right_FLAG = Right_5;

			/* 编码器积分标志位置1 */
			Encoder_jifen_flag = 1;

			/* 蜂鸣器响起 */
			Buzzer_ON();
			Right_Patch_Init();
		}
		else
		{
			/* 拉线出环 */
			Left_Patch_Init();
			//蜂鸣器响起
			Buzzer_ON();
		}
	}
	/* 状态五 */
	else if(Find_Right_FLAG == Right_5)
	{
		//当左右轮积分大于1500时
		if(((Encoder_jifen_L + Encoder_jifen_R) / 2) > Right_Enc_Out)
		{
			Find_Right_FLAG = Right_0;

			/* 编码器积分标志位置0 */
			Encoder_jifen_flag = 0;

			//关闭蜂鸣器
			Buzzer_OFF();
		}
		else
		{
			/* 蜂鸣器响起 */
			Buzzer_ON();
			Right_Patch_Init();
		}
	}
}

uint8 r_con,l_con;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找圆
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Find_Ring(void)
{
//	if((Find_Right_FLAG == Right_0) && (Find_Left_FLAG == Left_0))
//	{
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
		Left_local_LostNums = 0;
		Right_local_LostNums = 0;
		Left_dowm_Patch = 0;
		Right_dowm_Patch = 0;
		
		//避免大拐弯时的误判圆环
		White_Nums = White_counts_weight(90);
		
		//避免十字误判圆环
		Left_dowm_Patch = Find_left_dowm_point(110,50);
		Right_dowm_Patch = Find_Right_dowm_point(110,50);
		//统计局部的丢线数量
//        Left_local_LostNums = Count_Left_Lost(100,30);
//        Right_local_LostNums = Count_Right_Lost(100,30);
//	}
	Find_Right_Ring();
	Find_Left_Ring();
}

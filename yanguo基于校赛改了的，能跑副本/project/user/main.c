/*********************************************************************************************************************
* STC32G144K Opensourec Library 即（STC32G144K 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2025 SEEKFREE 逐飞科技
*
* 本文件是STC32G144K开源库的一部分
*
* STC32G144K 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK FOR C251
* 适用平台          STC32G144K
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者           备注
* 2025-11-20        大W            first version
********************************************************************************************************************/
#include "qy_headfile.h"

uint16 use_time,i = 0;       //计时变量     3ms多处理一帧

/* 速度模式切换 */
int out_Mode = 1;

MODE mode;

/* 丢线保护标志位 */
int Protection = 0;

/* 图像开启标志 */
int image_flag = 0;

uint8 COM_QY = 0;

int my_Speed = 0;
extern int16 TargetSpeed;
extern uint16 time;
uint16 qy_time = 0;
uint8 far image_copy[MT9V03X_H][MT9V03X_W];
uint8 far image_copy_out[MT9V03X_H][MT9V03X_W];
int count1 = 0;
// 方案1
extern int16  TargetSpeed_1 ;
extern float  Turn_KP_1;
extern float  Turn_GKD_1;

// 方案2
extern int16  TargetSpeed_2 ;
extern float  Turn_KP_2;
extern float  Turn_GKD_2;
extern uint8 menu_cursor;         
extern uint8 select_plan;
void main(void)
{
    clock_init(SYSTEM_CLOCK_96M); 				// 时钟配置及系统初始化<务必保留>
    debug_init();                       		// 调试串口信息初始化

    // 此处编写用户代码 例如外设初始化代码等
	/* 所有功能初始化 */
    All_Init();
    // 此处编写用户代码 例如外设初始化代码等
	
//	Left_Motor_Speed(2000);
////	Right_Motor_Speed(2000);
	pid.Speed_KP_L = pid.Speed_KP_R = 15;//正常值：6.8；一次超调值：13
	pid.Speed_KI_L = pid.Speed_KI_R = 3.2;//正常值：1.9；一次超调值：3.2
	pid.Turn_KP =9;
//	pid.Turn_KP1 =0;
	pid.Turn_KD =0;
//	pid.Turn_GKD =0.005;

	while(1)
	{
//		printf("%f,%f,%f\n",imu.acc.angle[imu_X],imu.acc.angle[imu_Y],imu.gyro.angle[imu_Z]);
//		printf("%d,%d,%d,%d,%f,%f\n",Encoder_Left,Encoder_Right,nowtargetSpeed,Speed_Left_Out,pid.Speed_KI_R,pid.Speed_KP_R);
//		printf("%d,%d,%d,%d,%f,%f\n",Image_error,Turn_Out,nowtargetSpeed,Speed_Left_Out,pid.Speed_KI_R,pid.Speed_KP_R);
//		tft180_show_int16(Image_W,0,COM_QY);
		if(COM_QY == 0)
		{
			tft180_show_int16(MT9V03X_W / 2,0,Image_error);
			tft180_show_int16(MT9V03X_W / 2,16,Right_dowm_Patch);
			tft180_show_int16(MT9V03X_W / 2,32,Left_dowm_Patch);
			tft180_show_int16(MT9V03X_W / 2,48,Right_Lost_Line_count);
			tft180_show_int16(MT9V03X_W / 2,64,Left_Lost_Line_count);
			tft180_show_int16(MT9V03X_W / 2,80,White_Column_MID);
			tft180_show_int16(MT9V03X_W / 2,96,r_con);
			tft180_show_int16(MT9V03X_W / 2,112,l_con);
			tft180_show_float(0,MT9V03X_H / 2,pid.Turn_KP,3,2);
			tft180_show_float(0,MT9V03X_H / 2 + 16,pid.Turn_KD,3,2);
			tft180_show_float(0,MT9V03X_H / 2 + 32,nowtargetSpeed,3,2);
		}
//==================== 菜单按键 ====================
		
		
//	if(COM_QY == 0)  // 停车才能调参
//	{
//		// KEY4 切换方案1 / 方案2
//		if(Get_Key_4())
//		{
//	//						menu_cursor = !menu_cursor;
//	//						select_plan = menu_cursor + 1;
//			select_plan+=1;
//			menu_cursor+=1;
//			if(select_plan > 1)
//			{
//				select_plan = 0;
//			}
//			if(menu_cursor > 1)
//			{
//				menu_cursor = 0;
//			}
//		}

//		// KEY1 参数 +
//		if(Get_Key_1())
//		{
//			if(select_plan == 1)
//			{
//				Turn_KP_1    += 0.25;
//			}
//			else
//			{
//				TargetSpeed_2 += 5;
//				Turn_KP_2    += 0.25;
//				Turn_GKD_2   += 0.001;
//			}
//		}

//		// KEY2 参数 -
//		if(Get_Key_5())
//		{
//			if(select_plan == 1)
//			{
//				Turn_KP_1    -= 0.25;
//			}
//			else
//			{
//				Turn_KP_2    -= 0.25;
//			}
//		}
//		if(select_plan == 1)
//		{
//			TargetSpeed = TargetSpeed_1;
//			pid.Turn_KP = Turn_KP_1;
//			pid.Turn_GKD = Turn_GKD_1;
//			KP_x_Increase = 0.002;
//			KP_x_Decrease = 0.002;
//		}
//		else
//		{
//			TargetSpeed = TargetSpeed_2;
//			pid.Turn_KP = Turn_KP_2;
//			pid.Turn_GKD = Turn_GKD_2;
//			KP_x_Increase = 0.002;
//			KP_x_Decrease = 0.002;
//		}
//		show_menu();
//	}
//	if(Get_Key_3())
//	{
//		if(COM_QY == 0)
//		{
//			COM_QY = 1;
//			time = 0;
//		}
//		else if(COM_QY == 1)
//		{
//			pid.Speed_All_Error_L = 0;
//			pid.Speed_All_Error_R = 0;
//			my_Speed = TargetSpeed;
//			COM_QY = 0; 
//		}
//	}


		if(Get_Key_3())
		{
			if(COM_QY == 0)
			{
				COM_QY = 1;
				time = 0;
			}
			else if(COM_QY == 1)
			{
				pid.Speed_All_Error_L = 0;
				pid.Speed_All_Error_R = 0;
				my_Speed = 200 ;
				COM_QY = 0; 
			}
		}
		if(Get_Key_4())
		{
			my_Speed += 20;
			if(my_Speed > 300)
			{
				my_Speed = 50;
			}
		}
		if(Get_Key_5())
		{
			pid.Turn_KP += 0.5;
	//		pid.Speed_KI_L = pid.Speed_KI_R = pid.Speed_KI_R + 0.1;
		}
		if(Get_Key_1())
		{
			pid.Turn_KD += 0.5;
	//		pid.Speed_KP_L = pid.Speed_KP_R = pid.Speed_KP_R + 0.1;
		}
	
		/* 图像处理 */
		if(mt9v03x_finish_flag)
		{
			printf("%d\n",qy_time);
			qy_time = 0;
			printf("%d\n",qy_time);
			memcpy(image_copy_out[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
//			LowerCameraExposure();
			get_reference_point();      //获取图像差比和参考点
			search_reference_col();
			Find_Boundry_LongWhiteCol(); //找边界搜线
			if(count1 > 2000)
			{
				Black_counts_weight(80);      //丢线保护
			}
//			seekfree_assistant_camera_send();
			printf("%d\n",qy_time);
			if(COM_QY == 0)
			{
				tft180_show_gray_image(0,0, image_copy_out[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 2, MT9V03X_H / 2, 0);
			}
			printf("%d\n",qy_time);
			mt9v03x_finish_flag = 0;
		}
	}
}

void Interrupt(void)
{
	 /* 获取陀螺仪数据 */
	gyroscope_get_gyro();
//    get_imu_data();

//    /* 陀螺仪处理 */
//    imu_task();

//    /* PID决策 */
//    PID_DecisionMaking();
	
	Mid_Error_Processing();
	Interrupt_CCU60_CH0();
	if(COM_QY == 1)
	{
	count1++;
	}
	else
	{
	count1=0;
	}
}

void QQYY(void)
{
	if(mt9v03x_finish_flag)
	{
		qy_time ++;
	}
	else
	{
//		qy_time = 0;
	}
}

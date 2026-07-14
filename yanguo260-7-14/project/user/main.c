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
#include "main.h"

uint16 use_time,i = 0;       //计时变量     3ms多处理一帧

uint8 COM_QY = 0;

int16 my_Speed = 260;//220 260 290

uint16 qy_time = 0;
uint16 qy_time1 = 0;
uint8 time_flag = 0;
uint8 far image_copy[MT9V03X_H][MT9V03X_W];
uint8 far image_copy_out[MT9V03X_H][MT9V03X_W];
uint16 count1 = 0;

uint8 data_buffer[21];

int16 T_KP,T_KP1,T_GKD,SP_KP,SP_KI,Ring_T_KP,W_T_KP;

void main(void)
{
    clock_init(SYSTEM_CLOCK_96M); 				// 时钟配置及系统初始化<务必保留>
    debug_init();                       		// 调试串口信息初始化

    // 此处编写用户代码 例如外设初始化代码等
	/* 所有功能初始化 */
    All_Init();
	
	pid.Speed_KP_L = pid.Speed_KP_R = 170;//正常值：125；一次超调值：170  220
	pid.Speed_KI_L = pid.Speed_KI_R = 35;//正常值：25；一次超调值：35  90
	Ring_T_KP = 65;
	W_T_KP = 35;
	T_KP = 45;//40 44
	T_KP1 =5;
	pid.Turn_KD = 0;
	T_GKD = -10;
	while(1)
	{
//		printf("%f,%f,%f\n",imu.acc.angle[imu_X],imu.acc.angle[imu_Y],imu.gyro.angle[imu_Z]);
//		printf("%d,%d,%d,%d,%d,%d\n",Encoder_Left,Encoder_Right,nowtargetSpeed,Speed_Left_Out,pid.Speed_KI_R,pid.Speed_KP_R);
//		printf("%d,%d,%d,%d,%f,%f\n",Image_error,Turn_Out,nowtargetSpeed,Speed_Left_Out,pid.Speed_KI_R,pid.Speed_KP_R);
//		printf("%d,%d,%d,%d,%d,%d\n",
//		Right_dowm_Patch,Left_dowm_Patch,Right_Lost_Line_count,Left_Lost_Line_count,Right_local_LostNums,Left_local_LostNums);
//		wireless_uart_send_byte(0xAA);
//		wireless_uart_send_byte((uint8)avl_gyro_z);
//		wireless_uart_send_byte((uint8)Right_dowm_Patch);
//		wireless_uart_send_byte((uint8)Left_dowm_Patch);
//		wireless_uart_send_byte((uint8)Right_Lost_Line_count);
//		wireless_uart_send_byte((uint8)Left_Lost_Line_count);
//		wireless_uart_send_byte((uint8)White_Column_MID);
//		wireless_uart_send_byte((uint8)White_Nums);
//		wireless_uart_send_byte((uint8)Right_local_LostNums);
//		wireless_uart_send_byte((uint8)Left_local_LostNums);
//		wireless_uart_send_byte((uint8)Find_Right_FLAG);
//		wireless_uart_send_byte((uint8)Find_Left_FLAG);
//		wireless_uart_send_byte((uint8)angle_ringR);
//		wireless_uart_send_byte(0xFF);
		
		if(COM_QY == 0)
		{
			menu_all();
		}
		else
		{
			if(Get_Key_3())
			{
				pid.Speed_All_Error_L = 0;
				pid.Speed_All_Error_R = 0;
				my_Speed = 200 ;
				COM_QY = 0; 
			}
		}
	
		/* 图像处理 */
		if(mt9v03x_finish_flag)
		{
//			time_flag = 1;
//			printf("%d\n",qy_time);
//			qy_time = 0;
//			qy_time1 = 0;
//			printf("%d\n",qy_time);
			memcpy(image_copy_out[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
			LowerCameraExposure();
			get_reference_point();      //获取图像差比和参考点
			search_reference_col();
			Find_Boundry_LongWhiteCol(); //找边界搜线
//			Extract_Road_Features();
			if(count1 > 0)
			{
				Black_counts_weight(100);      //丢线保护
			}
//			seekfree_assistant_camera_send();
//			printf("%d\n",qy_time);
//			printf("%d\n",qy_time1);
//			if(COM_QY == 0)
//			{
//				tft180_show_gray_image(0,0, image_copy_out[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 2, MT9V03X_H / 2, 0);
//			}
			mt9v03x_finish_flag = 0;
//			printf("%d\n",qy_time);
//			printf("%d,%d\n",Image_error,White_Column_MID);
//			time_flag = 0;
		}
	}
}

void Interrupt(void)
{
	 /* 获取陀螺仪数据 */
	gyroscope_get_gyro();
	
//    /* PID决策 */
//    PID_DecisionMaking();
	
	Mid_Error_Processing();
	Interrupt_CCU60_CH0();
	if(Diswitch_Key_2)
	{
		Open_Blds();
	}
	else
	{
		if(COM_QY == 1)
		{
			COM_QY = 2;
		}
	}
	if(COM_QY == 2)
	{
		count1++;
	}
	else if(COM_QY == 0)
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
	if(time_flag)
	{
		qy_time1 ++;
	}
}

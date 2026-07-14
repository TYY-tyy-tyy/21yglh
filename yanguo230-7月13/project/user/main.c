/*********************************************************************************************************************
* STC32G144K Opensourec Library ����STC32G144K ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2025 SEEKFREE ��ɿƼ�
*
* ���ļ���STC32G144K��Դ���һ����
*
* STC32G144K ��Դ�� ���������
* �����Ը���������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù�������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ����֤Э�� ������������Ϊ���İ汾
* ��������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ����֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          MDK FOR C251
* ����ƽ̨          STC32G144K
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����           ��ע
* 2025-11-20        ��W            first version
********************************************************************************************************************/
#include "main.h"

uint16 use_time,i = 0;       //��ʱ����     3ms�ദ��һ֡

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
    clock_init(SYSTEM_CLOCK_96M); 				// ʱ�����ü�ϵͳ��ʼ��<��ر���>
    debug_init();                       		// ���Դ�����Ϣ��ʼ��

    // �˴���д�û����� ���������ʼ�������
	/* ���й��ܳ�ʼ�� */
    All_Init();
	
	pid.Speed_KP_L = pid.Speed_KP_R = 170;//����ֵ��125��һ�γ���ֵ��170  220
	pid.Speed_KI_L = pid.Speed_KI_R = 35;//����ֵ��25��һ�γ���ֵ��35  90
	Ring_T_KP = 55;
	W_T_KP = 35;
	T_KP = 50;//40 44
	T_KP1 =2;
	pid.Turn_KD = 0;
	T_GKD = -8;
	while(1)
	{
//		printf("%f,%f,%f\n",imu.acc.angle[imu_X],imu.acc.angle[imu_Y],imu.gyro.angle[imu_Z]);
//		printf("%d,%d,%d,%d,%d,%d\n",Encoder_Left,Encoder_Right,nowtargetSpeed,Speed_Left_Out,pid.Speed_KI_R,pid.Speed_KP_R);
//		printf("%d,%d,%d,%d,%f,%f\n",Image_error,Turn_Out,nowtargetSpeed,Speed_Left_Out,pid.Speed_KI_R,pid.Speed_KP_R);
//		printf("%d,%d,%d,%d,%d,%d\n",
//		Right_dowm_Patch,Left_dowm_Patch,Right_Lost_Line_count,Left_Lost_Line_count,Right_local_LostNums,Left_local_LostNums);
		wireless_uart_send_byte(0xAA);
		wireless_uart_send_byte((uint8)(Image_error >> 8));       // Image_error 高字节
		wireless_uart_send_byte((uint8)(Image_error & 0xFF));     // Image_error 低字节
		wireless_uart_send_byte((uint8)(Turn_Out >> 8));          // Turn_Out 高字节
		wireless_uart_send_byte((uint8)(Turn_Out & 0xFF));        // Turn_Out 低字节
		wireless_uart_send_byte((uint8)avl_gyro_z);
		wireless_uart_send_byte((uint8)Right_dowm_Patch);
		wireless_uart_send_byte((uint8)Left_dowm_Patch);
		wireless_uart_send_byte((uint8)Right_Lost_Line_count);
		wireless_uart_send_byte((uint8)Left_Lost_Line_count);
		wireless_uart_send_byte((uint8)White_Column_MID);
		wireless_uart_send_byte((uint8)White_Nums);
		wireless_uart_send_byte((uint8)Right_local_LostNums);
		wireless_uart_send_byte((uint8)Left_local_LostNums);
		wireless_uart_send_byte((uint8)Find_Right_FLAG);
		wireless_uart_send_byte((uint8)Find_Left_FLAG);
		wireless_uart_send_byte((uint8)angle_ringR);
		wireless_uart_send_byte(0xFF);
		
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
	
		/* ͼ���� */
		if(mt9v03x_finish_flag)
		{
//			time_flag = 1;
//			printf("%d\n",qy_time);
//			qy_time = 0;
//			qy_time1 = 0;
//			printf("%d\n",qy_time);
			memcpy(image_copy_out[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
			LowerCameraExposure();
			get_reference_point();      //��ȡͼ���ȺͲο���
			search_reference_col();
			Find_Boundry_LongWhiteCol(); //�ұ߽�����
//			Extract_Road_Features();
			if(count1 > 0)
			{
				Black_counts_weight(100);      //���߱���
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
	 /* ��ȡ���������� */
	gyroscope_get_gyro();
	
//    /* PID���� */
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

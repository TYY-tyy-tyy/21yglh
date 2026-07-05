/*********************************************************************************************************************
* STC32G144K Opensourec Library ï¿½ï¿½ï¿½ï¿½STC32G144K ï¿½ï¿½Ô´ï¿½â£©ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ï¿½ï¿½Ú¹Ù·ï¿½ SDK ï¿½Ó¿ÚµÄµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½
* Copyright (c) 2025 SEEKFREE ï¿½ï¿½É¿Æ¼ï¿?*
* ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ï¿½STC32G144Kï¿½ï¿½Ô´ï¿½ï¿½ï¿½Ò»ï¿½ï¿½ï¿½ï¿?*
* STC32G144K ï¿½ï¿½Ô´ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?* ï¿½ï¿½ï¿½ï¿½ï¿½Ô¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½á·¢ï¿½ï¿½ï¿½ï¿?GPLï¿½ï¿½GNU General Public Licenseï¿½ï¿½ï¿½ï¿½ GNUÍ¨ï¿½Ã¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¤ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
* ï¿½ï¿½ GPL ï¿½Äµï¿½3ï¿½æ£¨ï¿½ï¿½ GPL3.0ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ñ¡ï¿½ï¿½Ä£ï¿½ï¿½ÎºÎºï¿½ï¿½ï¿½ï¿½Ä°æ±¾ï¿½ï¿½ï¿½ï¿½ï¿½Â·ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½Þ¸ï¿½ï¿½ï¿½
*
* ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½Ä·ï¿½ï¿½ï¿½ï¿½ï¿½Ï£ï¿½ï¿½ï¿½ï¿½ï¿½Ü·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ã£ï¿½ï¿½ï¿½ï¿½ï¿½Î´ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ÎºÎµÄ±ï¿½ï¿½?* ï¿½ï¿½ï¿½ï¿½Ã»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ô»ï¿½ï¿½Êºï¿½ï¿½Ø¶ï¿½ï¿½ï¿½Í¾ï¿½Ä±ï¿½Ö¤
* ï¿½ï¿½ï¿½ï¿½Ï¸ï¿½ï¿½ï¿½ï¿½Î¼ï¿?GPL
*
* ï¿½ï¿½Ó¦ï¿½ï¿½ï¿½ï¿½ï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½ï¿½Í¬Ê±ï¿½Õµï¿½Ò»ï¿½ï¿?GPL ï¿½Ä¸ï¿½ï¿½ï¿½
* ï¿½ï¿½ï¿½Ã»ï¿½Ð£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½<https://www.gnu.org/licenses/>
*
* ï¿½ï¿½ï¿½ï¿½×¢ï¿½ï¿½ï¿½ï¿½
* ï¿½ï¿½ï¿½ï¿½Ô´ï¿½ï¿½Ê¹ï¿½ï¿½ GPL3.0 ï¿½ï¿½Ô´ï¿½ï¿½ï¿½ï¿½Ö¤Ð­ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½ï¿½ï¿½Ä°æ±¾
* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó¢ï¿½Ä°ï¿½ï¿½ï¿½ libraries/doc ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½Âµï¿½ GPL3_permission_statement.txt ï¿½Ä¼ï¿½ï¿½ï¿½
* ï¿½ï¿½ï¿½ï¿½Ö¤ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ libraries ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½Âµï¿½ LICENSE ï¿½Ä¼ï¿½
* ï¿½ï¿½Ó­ï¿½ï¿½Î»Ê¹ï¿½Ã²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Þ¸ï¿½ï¿½ï¿½ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ë±£ï¿½ï¿½ï¿½ï¿½É¿Æ¼ï¿½ï¿½Ä°ï¿½È¨ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?*
* ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½          
* ï¿½ï¿½Ë¾ï¿½ï¿½ï¿½ï¿½          ï¿½É¶ï¿½ï¿½ï¿½É¿Æ¼ï¿½ï¿½ï¿½ï¿½Þ¹ï¿½ï¿½?* ï¿½æ±¾ï¿½ï¿½Ï¢          ï¿½é¿´ libraries/doc ï¿½Ä¼ï¿½ï¿½ï¿½ï¿½ï¿½ version ï¿½Ä¼ï¿½ ï¿½æ±¾Ëµï¿½ï¿½
* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½          MDK FOR C251
* ï¿½ï¿½ï¿½ï¿½Æ½Ì¨          STC32G144K
* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½          https://seekfree.taobao.com/
*
* ï¿½Þ¸Ä¼ï¿½Â¼
* ï¿½ï¿½ï¿½ï¿½              ï¿½ï¿½ï¿½ï¿½           ï¿½ï¿½×¢
* 2025-11-20        ï¿½ï¿½W            first version
********************************************************************************************************************/
#include "qy_headfile.h"

uint16 use_time,i = 0;       //ï¿½ï¿½Ê±ï¿½ï¿½ï¿½ï¿½     3msï¿½à´¦ï¿½ï¿½Ò»Ö¡

/* ï¿½Ù¶ï¿½Ä£Ê½ï¿½Ð»ï¿½ */
int out_Mode = 1;

MODE mode;

/* ï¿½ï¿½ï¿½ß±ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾Î» */
int Protection = 0;

/* Í¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ö¾ */
int image_flag = 0;

uint8 COM_QY = 0;

int my_Speed = 0;
extern int16 TargetSpeed;
extern uint16 time;
uint16 qy_time = 0;
// Ô­ image_copy Î´Ê¹ÓÃ£¬ÒÑÉ¾³ý¡£22KB ÓÃÓÚµÖÏûÐÂÔöµÄ image_dma_buf2
// ==================== Ë«»º³åDMA·½°¸ ====================
// image_copy_out¸ÄÎªÖ¸Õë£¬Ö±½ÓÖ¸ÏòDMA¸ÕÍê³ÉµÄ°²È«»º³åÇø£¬ÎÞÐèmemcpy
uint8 far image_copy[MT9V03X_H][MT9V03X_W];
image_copy_out_ptr_t image_copy_out;
// µÚ¶þDMA»º³åÇø£ºDMA½»ÌæÐ´Èëmt9v03x_imageºÍ´Ë´¦
uint8 xdata image_dma_buf2[MT9V03X_H][MT9V03X_W];
// dma_target_sel: VSYNC ISRÖÐ·­×ª£¬Ö¸Ê¾ÏÂÒ»Ö¡DMAÐ´ÄÄ¸ö»º³å (0=mt9v03x_image, 1=image_dma_buf2)
volatile uint8 dma_target_sel = 0;
// dma_done_sel: DMAÍê³ÉISRÖÐ¼ÇÂ¼¸ÕÍê³ÉµÄ»º³å (0=mt9v03x_image, 1=image_dma_buf2)
volatile uint8 dma_done_sel = 0;
	volatile uint8 processing_busy = 0;
int count1 = 0;
// ï¿½ï¿½ï¿½ï¿½1
extern int16  TargetSpeed_1 ;
extern float  Turn_KP_1;
extern float  Turn_GKD_1;

// ï¿½ï¿½ï¿½ï¿½2
extern int16  TargetSpeed_2 ;
extern float  Turn_KP_2;
extern float  Turn_GKD_2;
extern uint8 menu_cursor;         
extern uint8 select_plan;
void main(void)
{
    clock_init(SYSTEM_CLOCK_96M); 				// Ê±ï¿½ï¿½ï¿½ï¿½ï¿½Ã¼ï¿½ÏµÍ³ï¿½ï¿½Ê¼ï¿½ï¿½<ï¿½ï¿½Ø±ï¿½ï¿½ï¿?
    debug_init();                       		// ï¿½ï¿½ï¿½Ô´ï¿½ï¿½ï¿½ï¿½ï¿½Ï¢ï¿½ï¿½Ê¼ï¿½ï¿½

    // ï¿½Ë´ï¿½ï¿½ï¿½Ð´ï¿½Ã»ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	/* ï¿½ï¿½ï¿½Ð¹ï¿½ï¿½Ü³ï¿½Ê¼ï¿½ï¿½ */
    All_Init();
    // ï¿½Ë´ï¿½ï¿½ï¿½Ð´ï¿½Ã»ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	
//	Left_Motor_Speed(2000);
////	Right_Motor_Speed(2000);
	pid.Speed_KP_L = pid.Speed_KP_R = 15;//ï¿½ï¿½ï¿½ï¿½Öµï¿½ï¿½6.8ï¿½ï¿½Ò»ï¿½Î³ï¿½ï¿½ï¿½Öµï¿½ï¿½13
	pid.Speed_KI_L = pid.Speed_KI_R = 3.2;//ï¿½ï¿½ï¿½ï¿½Öµï¿½ï¿½1.9ï¿½ï¿½Ò»ï¿½Î³ï¿½ï¿½ï¿½Öµï¿½ï¿½3.2
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
//==================== ï¿½Ëµï¿½ï¿½ï¿½ï¿½ï¿½ ====================
		
		
//	if(COM_QY == 0)  // Í£ï¿½ï¿½ï¿½ï¿½ï¿½Üµï¿½ï¿½ï¿½
//	{
//		// KEY4 ï¿½Ð»ï¿½ï¿½ï¿½ï¿½ï¿½1 / ï¿½ï¿½ï¿½ï¿½2
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

//		// KEY1 ï¿½ï¿½ï¿½ï¿½ +
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

//		// KEY2 ï¿½ï¿½ï¿½ï¿½ -
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
	
		/* Í¼ï¿½ï¿½ï¿½ï¿½ */
		if(mt9v03x_finish_flag)
		{
			{
				uint8 done_snapshot = dma_done_sel;  // ¿ìÕÕDMAÍê³ÉµÄ»º³åÇø
				if(done_snapshot == 0)
				{
					image_copy_out = (image_copy_out_ptr_t)mt9v03x_image;
					buf_locked = 1;  // Ëø¶¨buf0£¬ISRÇ¿ÖÆDMAÐ´buf1
				}
				else
				{
					image_copy_out = (image_copy_out_ptr_t)image_dma_buf2;
					buf_locked = 2;  // Ëø¶¨buf1£¬ISRÇ¿ÖÆDMAÐ´buf0
				}
			}
			// ÏÖÔÚISRÎÞÂÛ´¥·¢¶àÉÙ´Î£¬¶¼²»»áÅö±»Ëø¶¨µÄ»º³åÇø
			
//			LowerCameraExposure();
			get_reference_point();      //»ñÈ¡Í¼ÏñºÍ²Î¿¼µã
			search_reference_col();
			Find_Boundry_LongWhiteCol(); //ï¿½Ò±ß½ï¿½ï¿½ï¿½ï¿½ï¿½
			if(count1 > 2000)
			{
				Lost_Line_Protect(80);      //ï¿½ï¿½ï¿½ß±ï¿½ï¿½ï¿½
			}
//			memcpy(image_copy[0], image_copy_out[0], MT9V03X_IMAGE_SIZE);
			seekfree_assistant_camera_send();
//			printf("%d\n",qy_time);
			if(COM_QY == 0)
			{
				tft180_show_gray_image(0,0, image_copy_out[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 2, MT9V03X_H / 2, 0);
			}
			buf_locked = 0;
			mt9v03x_finish_flag = 0;
		}
	}
}

void Interrupt(void)
{
	 /* ï¿½ï¿½È¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ */
	gyroscope_get_gyro();
//    get_imu_data();

//    /* ï¿½ï¿½ï¿½ï¿½ï¿½Ç´ï¿½ï¿½ï¿½ */
//    imu_task();

//    /* PIDï¿½ï¿½ï¿½ï¿½ */
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

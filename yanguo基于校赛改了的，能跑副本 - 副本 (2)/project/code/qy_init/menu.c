#include "memu.h"

uint8 menu_num = 0;

void menu_all(void)
{
	uint8 menu_all_num = 0;
	Get_Key();
	if(Diswitch_Key_1)
	{
		if(COM_QY == 0)
		{
			tft180_show_gray_image(0,0, image_copy_out[0], MT9V03X_W, MT9V03X_H, MT9V03X_W / 2, MT9V03X_H / 2, 0);
		}
	}
	if(menu_num == 0)
	{
		menu_0();
	}
	else if(menu_num == 1)
	{
		menu_1();
	}
	else if(menu_num == 2)
	{
		menu_2();
	}
	else if(menu_num == 3)
	{
		menu_3();
	}
	else if(menu_num == 4)
	{
		menu_4();
	}
}

void menu_0(void)
{
	uint8 menu_all_num = 0;
	if(Key_1)
	{
		menu_all_num ++;
	}
	else if(Key_2)
	{
		menu_all_num --;
	}
	else if(Key_5)
	{
		if(menu_all_num == 0)
		{
			menu_num = 1;
		}
		else if(menu_all_num == 1)
		{
			menu_num = 2;
		}
		else if(menu_all_num == 2)
		{
			menu_num = 3;
		}
		else if(menu_all_num == 3)
		{
			menu_num = 4;
		}
	}
	if(menu_all_num < 0)
	{
		menu_all_num = 2;
	}
	else if(menu_all_num > 2)
	{
		menu_all_num = 0;
	}
	tft180_show_int16(MT9V03X_W / 2,0,Diswitch_Key_1);
	tft180_show_int16(MT9V03X_W / 2,16,Diswitch_Key_2);
	tft180_show_int16(MT9V03X_W / 2,32,Diswitch_Key_3);//22
	tft180_show_string(0,MT9V03X_H / 2 + menu_all_num*16,">");
	tft180_show_string(8,MT9V03X_H / 2,"speed");
	tft180_show_string(8,MT9V03X_H / 2 + 16,"img");
	tft180_show_string(8,MT9V03X_H / 2 + 32,"PID");
	tft180_show_string(8,MT9V03X_H / 2 + 32,"run");
}

void menu_1(void)//speed
{
	uint8 menu_all_num = 0;
	if(Key_3)
	{
		menu_all_num ++;
	}
	else if(Key_4)
	{
		menu_all_num --;
	}
	else if(Key_3)
	{
		if(menu_all_num == 0)	
		{
			my_Speed += 10;
		}
		else if(menu_all_num == 1)
		{
			pid.Speed_KI_L = pid.Speed_KP_R = pid.Speed_KP_R + 1;
		}
		else if(menu_all_num == 2)
		{
			pid.Speed_KP_L = pid.Speed_KP_R = pid.Speed_KP_R + 1;
		}
	}
	else if(Key_4)
	{
		if(menu_all_num == 0)	
		{
			my_Speed -= 10;
		}
		else if(menu_all_num == 1)
		{
			pid.Speed_KI_L = pid.Speed_KP_R = pid.Speed_KP_R - 1;
		}
		else if(menu_all_num == 2)
		{
			pid.Speed_KP_L = pid.Speed_KP_R = pid.Speed_KP_R - 1;
		}
	}
	else if(Key_5)
	{
		menu_num = 0;
	}
	if(menu_all_num < 0)
	{
		menu_all_num = 2;
	}
	else if(menu_all_num > 2)
	{
		menu_all_num = 0;
	}
	tft180_show_string(0,MT9V03X_H / 2 + menu_all_num*16,">");
	tft180_show_string(8,MT9V03X_H / 2,"Speed");
	tft180_show_string(8,MT9V03X_H / 2 + 16,"KI");
	tft180_show_string(8,MT9V03X_H / 2 + 32,"KP");
	tft180_show_int16(56,MT9V03X_H / 2,my_Speed);
	tft180_show_int16(56,MT9V03X_H / 2 + 16,pid.Speed_KI_L);
	tft180_show_int16(56,MT9V03X_H / 2 + 32,pid.Speed_KP_L);
}

void menu_2(void)//img
{
	uint8 menu_all_num = 0;
	uint16 set_time = 500;
	if(Key_3)
	{
		menu_all_num ++;
	}
	else if(Key_4)
	{
		menu_all_num --;
	}
	else if(Key_3)
	{
		if(menu_all_num == 0)	
		{
			REFERENCE_CONTRAST ++;
		}
		else if(menu_all_num == 1)
		{
			set_time +=10;
			mt9v03x_set_exposure_time(set_time);
		}
		else if(menu_all_num == 2)
		{
			
		}
	}
	else if(Key_4)
	{
		if(menu_all_num == 0)	
		{
			REFERENCE_CONTRAST --;
		}
		else if(menu_all_num == 1)
		{
			set_time -=10;
			mt9v03x_set_exposure_time(set_time);
		}
		else if(menu_all_num == 2)
		{
			
		}
	}
	else if(Key_5)
	{
		menu_num = 0;
	}
	if(menu_all_num < 0)
	{
		menu_all_num = 2;
	}
	else if(menu_all_num > 1)
	{
		menu_all_num = 0;
	}
	tft180_show_string(0,MT9V03X_H / 2 + menu_all_num*16,">");
	tft180_show_string(8,MT9V03X_H / 2,"dbd");
	tft180_show_string(8,MT9V03X_H / 2 + 16,"bgsj");
//	tft180_show_string(8,MT9V03X_H / 2 + 32,"ys");
	tft180_show_int16(56,MT9V03X_H / 2,REFERENCE_CONTRAST);
	tft180_show_int16(56,MT9V03X_H / 2 + 16,set_time);
	tft180_show_int16(MT9V03X_W / 2,0,Image_error);
	tft180_show_int16(MT9V03X_W / 2,16,Right_dowm_Patch);
	tft180_show_int16(MT9V03X_W / 2,32,Left_dowm_Patch);//22
	tft180_show_int16(MT9V03X_W / 2,48,Right_Lost_Line_count);
	tft180_show_int16(MT9V03X_W / 2,64,Left_Lost_Line_count);//32
	tft180_show_int16(MT9V03X_W / 2,80,White_Column_MID);
	tft180_show_int16(MT9V03X_W / 2,96,Right_local_LostNums);
	tft180_show_int16(MT9V03X_W / 2,112,Left_local_LostNums);//31
}

void menu_3(void)//PID
{
	uint8 menu_all_num = 0;
	if(Key_3)
	{
		menu_all_num ++;
	}
	else if(Key_4)
	{
		menu_all_num --;
	}
	else if(Key_3)
	{
		if(menu_all_num == 0)	
		{
			pid.Turn_KP += 1;
		}
		else if(menu_all_num == 1)
		{
			pid.Turn_KP1 += 1;
		}
		else if(menu_all_num == 2)
		{
			pid.Turn_GKD += 1;
		}
		else if(menu_all_num == 3)
		{
			pid.Turn_KD += 1;
		}
	}
	else if(Key_4)
	{
		if(menu_all_num == 0)	
		{
			pid.Turn_KP -= 1;
		}
		else if(menu_all_num == 1)
		{
			pid.Turn_KP1 -= 1;
		}
		else if(menu_all_num == 2)
		{
			pid.Turn_GKD -= 1;
		}
		else if(menu_all_num == 3)
		{
			pid.Turn_KD -= 1;
		}
	}
	else if(Key_5)
	{
		menu_num = 0;
	}
	if(menu_all_num < 0)
	{
		menu_all_num = 2;
	}
	else if(menu_all_num > 3)
	{
		menu_all_num = 0;
	}
	tft180_show_string(0,MT9V03X_H / 2 + menu_all_num*16,">");
	tft180_show_string(8,MT9V03X_H / 2,"T_KP");
	tft180_show_string(8,MT9V03X_H / 2 + 16,"T_KP1");
	tft180_show_string(8,MT9V03X_H / 2 + 32,"T_GKD");
	tft180_show_string(8,MT9V03X_H / 2 + 48,"T_KD");
	tft180_show_int16(56,MT9V03X_H / 2,pid.Turn_KP);
	tft180_show_int16(56,MT9V03X_H / 2 + 16,pid.Turn_KP1);
	tft180_show_int16(56,MT9V03X_H / 2 + 32,pid.Turn_GKD);
	tft180_show_int16(56,MT9V03X_H / 2 + 48,pid.Turn_KD);
}

void menu_4(void)//RUN
{
	if(Key_3)
	{
		if(COM_QY == 0)
		{
			COM_QY = 1;
			count1 = 0;
		}
		else if(COM_QY == 1)
		{
			pid.Speed_All_Error_L = 0;
			pid.Speed_All_Error_R = 0;
			my_Speed = 200 ;
			COM_QY = 0; 
		}
	}
	tft180_show_int16(MT9V03X_W / 2,0,Image_error);
	tft180_show_int16(MT9V03X_W / 2,16,Right_dowm_Patch);
	tft180_show_int16(MT9V03X_W / 2,32,Left_dowm_Patch);//22
	tft180_show_int16(MT9V03X_W / 2,48,Right_Lost_Line_count);
	tft180_show_int16(MT9V03X_W / 2,64,Left_Lost_Line_count);//32
	tft180_show_int16(MT9V03X_W / 2,80,White_Column_MID);
	tft180_show_int16(MT9V03X_W / 2,96,Right_local_LostNums);
	tft180_show_int16(MT9V03X_W / 2,112,Left_local_LostNums);//31
	tft180_show_int16(48,MT9V03X_H / 2,Find_Right_FLAG);
	tft180_show_int16(48,MT9V03X_H / 2 + 16,Find_Left_FLAG);
	tft180_show_int16(48,MT9V03X_H / 2 + 32,Encoder_jifen_L);
	tft180_show_int16(48,MT9V03X_H / 2 + 48,White_Nums);
	tft180_show_int16(0,MT9V03X_H / 2,pid.Turn_KP);
	tft180_show_int16(0,MT9V03X_H / 2 + 16,pid.Turn_KD);
	tft180_show_int16(0,MT9V03X_H / 2 + 32,angle_ringR);
}

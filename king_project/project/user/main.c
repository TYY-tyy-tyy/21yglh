 /*********************************************************************************************************************
* STC32G144K Opensourec Library 即（STC32G144K 开源库）是一个基于官方 SDK 接口的第三方开源库
* 修改记录                               天天都在改
* 日期              作者                   备注
* 2026-1-23      川普大帅哥            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
// **************************** 代码区域 ****************************
uint8 ControlFlag=0;  //发车是 1   图传是0
uint32 time=0;
void main(void)
{
	System_Init();
	//laser_on();
    while(1)
    {
		
	    //motor_test();
		//tft180_show_string(0,64,"VBAT:");
		//tft180_show_float(32,64,Vbat*0.008826,4,6);
				
		//test_ss();	//wifiSPI的收发信息	
		//tft_image_tset();
		
		//Key_Anlysis();//按键
		
		//Out_servo=seekfree_assistant_parameter[0];

		//printf("%d, %d\r\n",imu660ra_gyro_z,encoder_data_l);
		//printf("%d\r\n",imu660ra_gyro_z);
//		tar_speed = seekfree_assistant_parameter[0];
//		pid_lf.kp=seekfree_assistant_parameter[1];
//		pid_lf.ki=seekfree_assistant_parameter[2];
//		pid_lf.kd=seekfree_assistant_parameter[3];		
		//seekfree_assistant_data_analysis();
		
		//图像
		//差比和只是过去了家人们 
		if(mt9v03x_finish_flag){
			//memcpy(image_copy[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
			get_reference_point(mt9v03x_image[0]);
			Search_reference_col(mt9v03x_image[0]);
			Search_line(mt9v03x_image[0]);
			//Ring();
			Fitted_Midline();
			if(!ControlFlag){
				seekfree_assistant_camera_send();	
			}
				
			mt9v03x_finish_flag=0;
		}
		
//		if(mt9v03x_finish_flag){
//			memcpy(base_image[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);
//			Threshold=otsuThreshold(base_image[0]);
//			base_image_threshold(Threshold);
//			
//			if(!ControlFlag){
//				seekfree_assistant_camera_send();	
//			}
//			
//			mt9v03x_finish_flag=0;
//		}
		
		
		
		
		
		///laser_off();
		
				//laser_on();
				//image_send_seekfree();//逐飞助手发送图像
//         if(mt9v03x_finish_flag)
//        {
//            mt9v03x_finish_flag = 0;
//			//iiiii=otsu_fast((const uint8 *)mt9v03x_image);
//			get_reference_point(mt9v03x_image[0]);
//			Search_reference_col(mt9v03x_image[0]);
//			Search_line(mt9v03x_image[0]);
//			Ring();
//			Fitted_Midline();
//			
//			
//			//Pre_Scan();
//			Target_find(pre_find_offset);
//			ips200_show_gray_image(0,0,(const uint8*)mt9v03x_image,188,120,188,120,tar_th);
//			
//			ips200_show_string(0,120,"Find_offset:");
//			ips200_show_int8(90,120,pre_find_offset);
//			
//			
//			ips200_show_string(0,136,"L R T U:");
//			ips200_show_uint8(70,136,l_d);
//			ips200_show_uint8(102,136,r_d);
//			ips200_show_uint8(134,136,top_d);
//			ips200_show_uint8(166,136,under_d);

//			ips200_show_string(0,152,"Th S:");
//			ips200_show_uint8(48,152,tar_th);
//			ips200_show_uint8(96,152,debug_stage);

//			ips200_show_string(0,168,"X  Y:");
//			ips200_show_uint8(50,168,tar_x);
//			ips200_show_uint8(82,168,tar_y);


//			if(tar_flag)
//			{
//				isp_200_draw_col(tar_x,120,0,RGB565_BLUE);
//				isp_200_draw_row(tar_y,188,0,RGB565_BLUE);
//			}
//		}
				//gpio_set_level(IO_P65, 1);			
				//image_send_seekfree();			
				//motor_test();							

		
//				system_delay(1000);
				
		
//				if(ring_l)			
//					Buzzer_On();			
//				else 					
//					Buzzer_Off();				
										
 

	
    }
}



//
void pit_handler(void)
{
	if(ControlFlag){
		imu660ra_get_gyro();
		Encoder_GetValue();
		
		Motor_Loop();
	}Servo_Loop();
	time++;
	if(time==2000){
		//controlFlag=0;
	}
	
}




#include "All_Init.h"

void All_Init(void)
{
    /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ */
    Key_Init();

	/* ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿?*/
    Servo_Init();
	
    /* ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿?*/
    Motors_Init();

    /* ï¿½ï¿½Ë¢ï¿½ï¿½Ê¼ï¿½ï¿½ */
//    Blds_Init();

    /* PIDï¿½ï¿½Ê¼ï¿½ï¿½ */
    PID_Init();

    /* TFTï¿½ï¿½Ä»ï¿½ï¿½Ê¼ï¿½ï¿½ */
    tft180_set_dir(TFT180_CROSSWISE); //ï¿½ï¿½ï¿½ï¿½Ä£Ê½
    tft180_init();

    /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ */
    Encoder_Init();

    /* ï¿½ï¿½ï¿½ï¿½LEDï¿½ï¿½Ê¼ï¿½ï¿½ */
//    gpio_init(LED, GPO, GPIO_LOW, GPO_PUSH_PULL);

    /* ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½ */
    Buzzer_Init();
	
	/* ï¿½ï¿½ï¿½ï¿½Ê³ï¿½Ê¼ï¿½ï¿?*/
	laser_init();

    /* ledï¿½ï¿½Ê¼ï¿½ï¿½ */
//    gpio_init(led, GPO, GPIO_LOW, GPO_PUSH_PULL);

    /* ï¿½ï¿½ï¿½ï¿½Í·ï¿½ï¿½Ê¼ï¿½ï¿½ */
    while(1)
    {
        if(mt9v03x_init())
            tft180_show_string(0, 16, "mt9v03x reinit.");
//			printf("\r\n mt9v03x_init error, try again.");
        else
            break;
    }
//	system_delay_ms(1000);
//	mt9v03x_set_exposure_time(500);
//	system_delay_ms(1000);
		// ==================== Ë«»º³åDMA·½°¸£º³õÊ¼»¯image_copy_outÖ¸Õë ====================
	image_copy_out = (image_copy_out_ptr_t)mt9v03x_image; // ³õÊ¼Ö¸Ïò¿â»º³åÇø
	seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);
	seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy_out[0], MT9V03X_W, MT9V03X_H);
	 
    /* ï¿½ï¿½ï¿½ï¿½ï¿½Ç³ï¿½Ê¼ï¿½ï¿½ */
    //ï¿½ï¿½ï¿½ï¿½ï¿½Ç³ï¿½Ê¼ï¿½ï¿½
    #if IMU_TYPE
         imu963ra_init();
    #else
         imu660ra_init();
    #endif

//    gyro_data_init();

//    //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ¯ï¿½ï¿½
//    gyroOffsetInit();

    /* ï¿½Ð¶Ï³ï¿½Ê¼ï¿½ï¿½ */
	pit_ms_init(TIM1_PIT,10,Interrupt);
	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ý³ï¿½Ê¼ï¿½ï¿½
	null_drift_calculate();
	
	pit_us_init(TIM0_PIT,100,QQYY);
}

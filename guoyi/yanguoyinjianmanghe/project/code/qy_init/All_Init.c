#include "All_Init.h"

void All_Init(void)
{
    /* 按键初始化 */
//    Key_Init();

	/* 舵机初始化 */
//    Servo_Init();
	
    /* 电机初始化 */
    Motors_Init();

    /* 无刷初始化 */
//    Blds_Init();

    /* PID初始化 */
//    PID_Init();

    /* TFT屏幕初始化 */
//    tft180_set_dir(TFT180_CROSSWISE); //横屏模式
//    tft180_init();

    /* 编码器初始化 */
//    Encoder_Init();

    /* 红外LED初始化 */
//    gpio_init(LED, GPO, GPIO_LOW, GPO_PUSH_PULL);

    /* 蜂鸣器初始化 */
//    Buzzer_Init();
	
	/* 激光笔初始化 */
//	laser_init();

    /* led初始化 */
//    gpio_init(led, GPO, GPIO_LOW, GPO_PUSH_PULL);

    /* 摄像头初始化 */
//    while(1)
//    {
//        if(mt9v03x_init())
////            tft180_show_string(0, 16, "mt9v03x reinit.");
//			printf("\r\n mt9v03x_init error, try again.");
//        else
//            break;
//    }
//	system_delay_ms(500);
//	mt9v03x_set_exposure_time(100);
//	seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);
//	seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, image_copy_out[0], MT9V03X_W, MT9V03X_H);
	 
    /* 陀螺仪初始化 */
    //陀螺仪初始化
//    #if IMU_TYPE
//         imu963ra_init();
//    #else
//         imu660ra_init();
//    #endif

//    //陀螺仪数据初始化
//	null_drift_calculate();
//    gyro_data_init();

//    //计算陀螺仪数据漂移
//    gyroOffsetInit();
}

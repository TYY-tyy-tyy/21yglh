#include "blds.h"

// 电调支持50hz-300hz的控制频率
		// 50Hz的控制频率 ，从0%到100%占空比为500到1000
		// 100Hz的控制频率，从0%到100%占空比为1000到2000
		// 200Hz的控制频率，从0%到100%占空比为2000到4000
		// 300Hz的控制频率，从0%到100%占空比为3000到6000
/* 无刷PWM  1000~2000 */

int16 Blds_PWM_MAX = 1700;    //芯片最大PWM
int16 Blds_PWM_MIN = 1000;    //芯片最大PWM

/* 检测无刷开启标志位 */
int Blds_flag = 0;

/* 无刷转速变量 */
int16 Blds_Speed = 1000;
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     无刷初始化
// 参数说明
//
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Blds_Init(void)
{
    pwm_init(Blds_PWM_CH1, 100, 0);                                                 // PWM 通道1 初始化频率100Hz 占空比初始为0
//    gpio_init(Blds_DIR_CH1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚
    encoder_dir_init(Blds_ENCODER1_TIM, Blds_ENCODER1_PLUS, Blds_ENCODER1_DIR);                // 初始化编码器采值引脚及定时器

    pwm_init(Blds_PWM_CH2, 100, 0);                                                 // PWM 通道2 初始化频率100Hz 占空比初始为0
//    gpio_init(Blds_DIR_CH2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                          // 初始化电机方向输出引脚
    encoder_dir_init(Blds_ENCODER2_TIM, Blds_ENCODER2_PLUS, Blds_ENCODER2_DIR);                // 初始化编码器采值引脚及定时器
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     左无刷驱动
// 参数说明     Speed           PWM占空比 （1000-2000）
//
// 返回参数     void
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Left_Blds_Speed(int16 Speed)
{
    /* 限幅 */
    if(Speed > Blds_PWM_MAX) Speed = Blds_PWM_MAX;
    else if(Speed < Blds_PWM_MIN) Speed = Blds_PWM_MIN;

    pwm_set_duty(Blds_PWM_CH2,Speed);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     右无刷驱动
// 参数说明     Speed           PWM占空比 （1000-2000）
//
// 返回参数     void
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Right_Blds_Speed(int16 Speed)
{
    /* 限幅 */
    if(Speed > Blds_PWM_MAX) Speed = Blds_PWM_MAX;
    else if(Speed < Blds_PWM_MIN) Speed = Blds_PWM_MIN;

    pwm_set_duty(Blds_PWM_CH1,Speed);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     检测是否开启无刷
// 参数说明
//
// 返回参数
// 使用示例
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void Open_Blds(int flag)
{
    if(flag == 1)
    {
        /* 缓慢增加转速 */
        if(Blds_Speed < Blds_PWM_MAX) Blds_Speed = Blds_Speed + 1;

        /* 限幅 */
        if(Blds_Speed < Blds_PWM_MIN) Blds_Speed = Blds_PWM_MIN;
        else if(Blds_Speed > Blds_PWM_MAX) Blds_Speed = Blds_PWM_MAX;

        Left_Blds_Speed(Blds_Speed);
        Right_Blds_Speed(Blds_Speed);
    }
    else
    {
        /* 初始化转速变量 */
        Blds_Speed = Blds_PWM_MIN;

        Left_Blds_Speed(0);
        Right_Blds_Speed(0);
    }
}

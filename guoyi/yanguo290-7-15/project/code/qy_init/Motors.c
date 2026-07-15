#include "Motors.h"

int16 Left_Out = 0;
int16 Right_Out = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     有刷电机初始化
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Motors_Init(void)
{
    /* 左电机 */
    gpio_init(Left_Motor_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(Left_Motor_PWM, 17000, 0);

    /* 右电机 */
    gpio_init(Right_Motor_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(Right_Motor_PWM, 17000, 0);
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     修改左电机
// 参数说明     Speed           PWM占空比 （0-10000）
//
// 返回参数     void
// 使用示例    Left_Rear_Fans_Speed(50);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------

void Left_Motor_Speed(int16 Speed)
{
    /* 限幅 */
    if(Speed > 5000) Speed = 5000;
    else if(Speed < -5000) Speed = -5000;

    if(Speed > 0)
    {
        gpio_set_level(Left_Motor_DIR, GPIO_HIGH);
        pwm_set_duty(Left_Motor_PWM,Speed);
    }
    else if(Speed < 0)
    {
        gpio_set_level(Left_Motor_DIR, GPIO_LOW);
        pwm_set_duty(Left_Motor_PWM,-Speed);
    }
    else if(Speed == 0)
    {
        gpio_set_level(Left_Motor_DIR, GPIO_LOW);
        pwm_set_duty(Left_Motor_PWM, Speed);
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     修改左电机
// 参数说明     Speed           PWM占空比 （0-10000）
//
// 返回参数     void
// 使用示例    Left_Rear_Fans_Speed(50);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------

void Right_Motor_Speed(int16 Speed)
{
    /* 限幅 */
    if(Speed > 5000) Speed = 5000;
    else if(Speed < -5000) Speed = -5000;

    if(Speed > 0)
    {
        gpio_set_level(Right_Motor_DIR, GPIO_LOW);
        pwm_set_duty(Right_Motor_PWM,Speed);
    }
    else if(Speed < 0)
    {
        gpio_set_level(Right_Motor_DIR, GPIO_HIGH);
        pwm_set_duty(Right_Motor_PWM,-Speed);
    }
    else if(Speed == 0)
    {
        gpio_set_level(Right_Motor_DIR, GPIO_HIGH);
        pwm_set_duty(Right_Motor_PWM, Speed);
    }
}

#include "Servo.h"

void Servo_Init(void)
{
    /* 舵机PWM引脚初始化 */
    pwm_init(SERVO_MOTOR_FREQ, Servo_PWM, SERVO_MOTOR_Mid);
}

void Servo_Angle(float angle)
{
    uint32 Servo_duty = 0;
    Servo_duty = (float)PWM_DUTY_MAX / (1000.0 / (float)SERVO_MOTOR_FREQ) * (1 + angle / 120.0);
    pwm_set_duty(Servo_PWM, Servo_duty);
}

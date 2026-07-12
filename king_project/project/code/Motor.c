#include "Motor.h"

#define MAX_DUTY            ( 20 )                             
int8 duty = 0;
int8 dir = 1;

#define ENCODER_DIR_1                 (PWMA_ENCODER)              // 带方向编码器对应使用的编码器接口 
#define ENCODER_DIR_PULSE_1           (PWMA_ENCODER_CH1P_P60)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_1             (PWMA_ENCODER_CH2P_P62)     // DIR 对应的引脚

#define ENCODER_DIR_2                 	(PWMC_ENCODER)              // 带方向编码器对应使用的编码器接口
#define ENCODER_DIR_PULSE_2       		(PWMC_ENCODER_CH1P_P40)     // PULSE 对应的引脚
#define ENCODER_DIR_DIR_2           	(PWMC_ENCODER_CH2P_P42)     // DIR 对应的引脚

int16 encoder_data_r=0;
int16 encoder_data_l=0;

void Motor_Init(void)
{
	gpio_init(DIR_L, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(DIR_R, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

}


//
//左轮正转1秒反转1秒蜂鸣器鸣叫，右轮正转1秒反转1秒蜂鸣器鸣叫
#define M_TEST  1000
void motor_test(void)
{
		gpio_set_level(DIR_L, GPIO_HIGH);  
		pwm_set_duty (PWM_L, M_TEST);
		system_delay_ms(1000);
		gpio_set_level(DIR_L, GPIO_LOW);  
		system_delay_ms(1000);
		
		pwm_set_duty (PWM_L, 0);
		gpio_set_level(IO_P65, 1);system_delay_ms(200);
		gpio_set_level(IO_P65, 0);
		
		gpio_set_level(DIR_R, GPIO_HIGH);  
		pwm_set_duty (PWM_R, M_TEST);
		system_delay_ms(1000);
		gpio_set_level(DIR_R, GPIO_LOW);  
		system_delay_ms(1000);
		pwm_set_duty (PWM_R, 0);
		gpio_set_level(IO_P65, 1);system_delay_ms(200);
		gpio_set_level(IO_P65, 0);

}
void Motor_control(pwm_channel_enum wheel,int16 speed)
{
	if(speed>=0){
		if(wheel==PWM_L){
			gpio_set_level(DIR_L, GPIO_HIGH);                                    
			pwm_set_duty(PWM_L, speed);		
		}
		else {
			gpio_set_level(DIR_R, GPIO_HIGH);                                    
			pwm_set_duty(PWM_R, speed);	
		}            
	}
	else {
		if(wheel==PWM_L){
			gpio_set_level(DIR_L, GPIO_LOW);                                    
			pwm_set_duty(PWM_L, -speed);		
		}
		else {
			gpio_set_level(DIR_R, GPIO_LOW);                                    
			pwm_set_duty(PWM_R, -speed);	
		}              
	}
}

void Encoder_GetValue(void)
{
    encoder_data_r = -encoder_get_count(ENCODER_DIR_1);                  // 获取编码器计数
    encoder_data_l = encoder_get_count(ENCODER_DIR_2);
	
	
    encoder_clear_count(ENCODER_DIR_1);                                		// 清空编码器计数
    encoder_clear_count(ENCODER_DIR_2);  
}

int16 tar_speed=120;
int16 diff_left,diff_right;
float diff_kp=-0;//0.30.25
void Dream_speed(void)
{						
    uint16 tp_turn;
	int8 delta = (int8)(err_sum - Mid_Col);
    // 计算舵机偏离中值的程度
    if (Out_servo >  SERVO_CENTER)//左拐
    {
        tp_turn = Out_servo - SERVO_CENTER;
		diff_left  = (int16)(- delta * diff_kp);
		diff_right = (int16)( 0.76f *  delta * diff_kp);
    }
    else			//右拐
    {
        tp_turn = SERVO_CENTER - Out_servo;
		delta = -delta;   // 转为正偏差量 (Mid_Col - err_sum)
		diff_left  = (int16)(   0.76f* delta * diff_kp);
		diff_right = (int16)(-  delta * diff_kp);
    }

    // 限幅，防止超过最大转向量
    if (tp_turn > MAX_TURN)
    {
        tp_turn = MAX_TURN;
    }

    // 直道：tp_turn = 0        → tar_speed = MAX_SPEED
    // 弯道：tp_turn 越大      → tar_speed 越低
    // 最大转向：tp_turn=MAX_TURN → tar_speed = MIN_SPEED
    tar_speed = MAX_SPEED -
                (uint32)(MAX_SPEED - MIN_SPEED) * tp_turn / MAX_TURN;
}


void Motor_Loop(void)
{
	//Dream_speed();
	Increment_PIDf(&pid_lf,(float)tar_speed,encoder_data_l);//+diff_left
	Increment_PIDf(&pid_rf,(float)tar_speed,encoder_data_r);//+diff_right
	
	Motor_control(PWM_L,(int16)pid_lf.out);
	Motor_control(PWM_R,(int16)pid_rf.out);	
}

